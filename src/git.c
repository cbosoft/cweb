#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "server.h"
#include "request.h"
#include "git.h"
#include "cJSON.h"

#define GIT_DIR "/home/git/"


cJSON *dir_contents(char *path)
{
  fprintf(stderr, "%s\n", path);
  cJSON *rv = cJSON_CreateArray();

  DIR *d;
  struct dirent *dir;
  d = opendir(path);
  cJSON *file, *directory;
  char *nextpath;

  if (d) {
    while ((dir = readdir(d)) != NULL ) {

      if ((strcmp(".", dir->d_name) == 0) || (strcmp("..", dir->d_name) == 0)) {
        continue;
      }
      
      switch (dir->d_type) {
        case DT_REG:
          file = cJSON_CreateString(dir->d_name);
          cJSON_AddItemToArray(rv, file);
          break;
        case DT_DIR:
          nextpath = calloc(strlen(path)+strlen(dir->d_name)+2, sizeof(char));
          sprintf(nextpath, "%s/%s", path, dir->d_name);
          directory = dir_contents(nextpath);
          free(nextpath);
          cJSON_AddItemToArray(rv, directory);
          break;
        default:
          break;
      }
    }
    closedir(d);
  }

  return rv;
}


void git(conndat_t *cd, struct request_t * req)
{
  const char * response = "In progress.";
  send_response(cd, RESP_200, "text/plain", (char*)response, strlen(response));
  return;

  // list files in git directory
  // send result as json obj back

  cJSON *json = cJSON_CreateObject();
  cJSON *repolist = cJSON_CreateArray();

  DIR *d;
  struct dirent *dir;
  d = opendir(GIT_DIR);
  if (d) {
    while ((dir = readdir(d)) != NULL ) {

      if (dir->d_type == DT_DIR) {

        if (dir->d_name[0] != '.') {
          cJSON *repo = cJSON_CreateObject();
          cJSON_AddStringToObject(repo, "name", dir->d_name);

          char *path = calloc(256, sizeof(char));
          sprintf(path, "%s/%s", GIT_DIR, dir->d_name);
          cJSON *repofiles = dir_contents(path);
          free(path);

          cJSON_AddItemToObject(repo, "files", repofiles);
          cJSON_AddItemToArray(repolist, repo);
        }
      }

    }
    closedir(d);
  }

  cJSON_AddItemToObject(json, "repos", repolist);

  char *json_str = cJSON_Print(json);

  cJSON_Delete(json);

  send_response(cd, RESP_200, "text/json", json_str, strlen(json_str));
}
