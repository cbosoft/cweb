#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "request.h"
#include "error.h"
#include "util.h"

struct request_t * parse_request(char *request_str)
{
  char **lines;
  int nlines;
  split_on_newline(request_str, &lines, &nlines);

  char **words;
  int nwords;
  split_on_string(request_str, " ", &words, &nwords);

  struct request_t * req = malloc(sizeof(struct request_t));

  // defaults
  req->method = NULL;
  req->path = NULL;
  req->http = NULL;
  req->body = NULL;
  req->content_type = NULL;
  req->useragent = NULL;
  req->host = NULL;
  
  req->method = calloc(strlen(words[0])+1, sizeof(char));
  strcpy(req->method, words[0]);
  
  req->path = calloc(strlen(words[1])+1, sizeof(char));
  strcpy(req->path, words[1]);

  req->http = calloc(strlen(words[2])+1, sizeof(char));
  strcpy(req->http, words[2]);

  int reading_headers = 1;

  req->content_length = 0;

  for (int i = 1; i < nlines; i++) {

    if (reading_headers) {
      if (strlen(lines[i]) == 0) {
        reading_headers = 0;
        req->body = calloc(req->content_length+1, sizeof(char));
        continue;
      }
      
      split_on_string(lines[i], " ", &words, &nwords);
      
      if (strcmp(words[0], "User-Agent:") == 0) {
      }
      else if (strcmp(words[0], "Accept:") == 0) {
      }
      else if (strcmp(words[0], "Accept-Language:") == 0) {
      }
      else if (strcmp(words[0], "Accept-Encoding:") == 0) {
      }
      else if (strcmp(words[0], "Content-Type:") == 0) {
        req->content_type = calloc(strlen(words[1])+1, sizeof(char));
        strcpy(req->content_type, words[1]);
      }
      else if (strcmp(words[0], "Content-Length:") == 0) {
        req->content_length = atoi(words[1]);
      }
    }
    else {
      strcat(req->body, lines[i]);
      if (i < nlines-1)
        strcat(req->body, "\n");
    }
  }

  return req;
}


void free_request_t(struct request_t *req)
{
  free(req->method);
  free(req->path);
  free(req->http);
  
  if (req->content_type != NULL)
    free(req->content_type);

  if (req->body != NULL)
    free(req->body);

  if (req->useragent != NULL)
    free(req->useragent);

  if (req->host != NULL)
    free(req->host);

}
