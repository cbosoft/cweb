#pragma once

#define PORT "80"  // the port users will be connecting to

#define SERVER_FILES "./serverfiles"
#define SERVER_ROOT "./serverroot"

#define RESP_200 "HTTP/1.1 200 OK"
#define RESP_204 "HTTP/1.1 204 NO CONTENT"
#define RESP_404 "HTTP/1.1 404 NOT FOUND"

#define HTML_HEADER(TITLE) \
  "<html>"\
  "<head>"\
  "<link rel=\"stylesheet\" href=\"style.css\">"\
  "<title>"TITLE"</title>"\
  "</head>"\
  "<body>" \
  "<div class=\"centre\">"

#define HTML_FOOTER \
    "<div class=\"footer\" style=\"text-align: center;\">"\
    "  <p>"\
    "  <a href=\"index.html\">home</a>&nbsp<a href=\"https://github.com/cbosoft\">github</a>"\
    "  </p>"\
    "</div>"\
    "</div>"\
  "</body>"\
  "</html>"

typedef struct conndat_t {
  int fd;
  const char *addr;
} conndat_t;

int send_response(conndat_t *cd, char *header, char *content_type, void *body, int content_length);
// vim:ft=c
