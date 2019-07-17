#pragma once

struct request_t {
  char *method;
  char *path;
  char *http;
  char *host;
  char *useragent;

  int content_length;
  char *content_type;
  char *body;

};

struct request_t *parse_request(char *request);
void free_request_t(struct request_t *req);
