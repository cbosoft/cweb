#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "util.h"

void split_on_string(char *in, char *on, char ***out, int *n)
{
  (*out) = malloc(1*sizeof(char *));
  int reslen = 1;
  
  int l = strlen(in), itmidx = 0, residx = 0, matchidx = 0, matchlen = strlen(on);
  (*out)[residx] = calloc(256, sizeof(char));
  
  char ch;
  for (int i = 0; i < l; i++) {
    ch = in[i];

    if (ch == on[matchidx]) {
      matchidx ++;

      if (matchidx == matchlen) {
        matchidx = 0;
        itmidx = 0;
        residx ++;
        reslen ++;
        (*out) = realloc((*out), reslen*sizeof(char *));
        (*out)[residx] = calloc(256, sizeof(char));
      }
    }
    else {
      (*out)[residx][itmidx] = ch;
      itmidx ++;
    }
    
    // TODO: stretch to accomodate longer splits
    if (itmidx > 254)
      ferr("split", "overflow you lazy programmer you");

  }

  (*n) = reslen;

}



void split_on_newline(char *in, char ***out, int *n)
{
  (*out) = malloc(1*sizeof(char *));
  int reslen = 1;
  
  int l = strlen(in), itmidx = 0, residx = 0;
  (*out)[residx] = calloc(256, sizeof(char));

  char cr = (char)13, lf = (char)10;
  
  for (int i = 0; i < l; i++) {

    if (in[i] == cr && in[i+1] == lf) {
      i++;
    }

    if (in[i] == cr || in[i] == lf) {
        itmidx = 0;
        residx ++;
        reslen ++;
        (*out) = realloc((*out), reslen*sizeof(char *));
        (*out)[residx] = calloc(256, sizeof(char));
    }
    else {
      (*out)[residx][itmidx] = in[i];
      itmidx ++;
    }
    
    // TODO: stretch to accomodate longer splits
    if (itmidx > 254)
      ferr("split", "overflow you lazy programmer you");

  }

  (*n) = reslen;

}


char *infer_newline(char *s)
{
  int len = strlen(s);
  for (int i = 0; i < len-1; i++) {

    if (s[i] == (char)13 && s[i+1] == (char)10) {
      return "\r\n";
    }
    else if (s[i] == (char)13) {
      return "\r";
    }
    else if (s[i] == (char)10) {
      return "\n";
    }

  }

  return NULL;
}
