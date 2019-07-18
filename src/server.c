/**
 * webserver.c -- A webserver written in C
 * 
 * Test with curl (if you don't have it, install it):
 * 
 *    curl -D - http://localhost:3490/
 *    curl -D - http://localhost:3490/d20
 *    curl -D - http://localhost:3490/date
 * 
 * You can also test the above URLs in your browser! They should work!
 * 
 * Posting Data:
 * 
 *    curl -D - -X POST -H 'Content-Type: text/plain' -d 'Hello, sample data!' http://localhost:3490/save
 * 
 * (Posting data is harder to test from a browser.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>

#include "error.h"
#include "net.h"
#include "mime.h"
#include "cache.h"
#include "io.h"
#include "util.h"
#include "request.h"
#include "server.h"
#include "git.h"

/**
 * Send an HTTP response
 *
 * header:       "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK", etc.
 * content_type: "text/plain", etc.
 * body:         the data to send.
 * 
 * Return the value from the send() function.
 */
int send_response(conndat_t *cd, char *header, char *content_type, void *body, int content_length)
{
    const int max_response_size = 262144;
    char response[max_response_size];

    timestamp("%s : replying %s", cd->addr, header);

    // Build HTTP response and store it in response
    sprintf(response, "%s\n\n%s", header, (char *)body);
    int response_length = strlen(response);

    // Send it all!
    int rv = send(cd->fd, response, response_length, 0);

    if (rv < 0) {
        perror("send");
    }

    return rv;
}




/**
 * Send a /d20 endpoint response
 */
void get_d20(conndat_t *cd)
{
  int random = rand() % 20 + 1;

  char *res = calloc(200, sizeof(char));
  sprintf(res, "%d\n", random);

  timestamp("%s : D20 roll: %s", cd->addr, res);

  send_response(cd, RESP_200, "text/plain", (void *)res, strlen(res));
}




/**
 * Send a 404 response
 */
void resp_404(conndat_t *cd)
{
    char filepath[4096];
    char *mime_type;

    // Fetch the 404.html file
    snprintf(filepath, sizeof filepath, "%s/404.html", SERVER_FILES);
    char * filedata = (char *)read_file(filepath);

    if (filedata == NULL) {
      send_response(cd, RESP_404, "text/html", 
          HTML_HEADER("404")
          "<h1>404</h1>"
          "<p>not found</p>"
          HTML_FOOTER, 
          48);
      return;
    }

    mime_type = mime_type_get(filepath);
    send_response(cd, RESP_404, mime_type, filedata, strlen(filedata));
}

/**
 * Read and return a file from disk or cache
 */
void get_file(conndat_t *cd, struct cache *cache, char *request_path)
{
  
  // TODO: caching
  // if in cache, read from there, if not try from disk

  if (strcmp(request_path, "/") == 0)
    request_path = "/index.html";

  char *fpath = calloc(100, sizeof(char));
  sprintf(fpath, "./%s", request_path);
  char *contents = (char *)read_file(fpath);
  
  if (contents == NULL) {
    resp_404(cd);
    return;
  }

  send_response(cd, RESP_200, "text/html", (void *)contents, strlen(contents));

  free(contents);

}

/**
 * Search for the end of the HTTP header
 * 
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */
char *find_start_of_body(char *header)
{
    ///////////////////
    // IMPLEMENT ME! // (Stretch)
    ///////////////////

  return NULL;
}

/**
 * Handle HTTP request and send response
 */
void handle_http_request(conndat_t *cd, struct cache *cache)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];

    // Read request
    int bytes_recvd = recv(cd->fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        timestamp("%s : recv error, could not read request.");
        return;
    }

    request[bytes_recvd] = '\0';
    
    struct request_t *req = parse_request(request);

    if (strcmp(req->method, "GET") == 0) {
      timestamp("%s : GET %s\n", cd->addr, req->path);

      get_file(cd, cache, req->path);
    }
    else if (strcmp(req->method, "POST") == 0) {
      timestamp("%s : POST %s \"%s\"\n", cd->addr, req->path, req->body);

      git(cd, req);
    }
    else {
      timestamp("%s : unknown command received \"%s\"\n", cd->addr, req->method);
    }

    free_request_t(req);
    free(req);

}

/**
 * Main
 */
int main(void)
{
    int newfd;  // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

    struct cache *cache = cache_create(10, 0);

    // Get a listening socket
    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0)
        ferr("main","fatal error getting listening socket\n");

    if (fork()) {
      fprintf(stdout, "cweb server forked to background, logging to log.txt.\n");
      exit(0);
    }

    FILE *logfp = fopen("log.txt", "w");
    fclose(logfp);

    timestamp("cweb started. waiting for connections on port %s.", PORT);

    while(1) {
        socklen_t sin_size = sizeof their_addr;

        // Parent process will block on the accept() call until someone
        // makes a new connection:
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }

        // Print out a message that we got the connection
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);

        timestamp("request from %s", s);
        
        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.

        conndat_t cd;
        cd.fd = newfd;
        cd.addr = s;

        handle_http_request(&cd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}

