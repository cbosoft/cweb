#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "server.h"
#include "util.h"
#include "reschoose.h"
#include "error.h"



void reschoose(conndat_t *cd, struct request_t *req)
{
  char **pairs;
  int npairs;

  split_on_string(req->body, "&", &pairs, &npairs);

  char **kv;
  int nkv;

  double *resistors = NULL;
  int nresistors;
  double ratio = 0;
  int number = -1;

  for (int i = 0; i < npairs; i++) {
    split_on_string(pairs[i], "=", &kv, &nkv);

    if (nkv != 2) {
      timestamp("not key-value pair (%d): \"%s\"", nkv, pairs[i]);
      return;
    }
    
    char *key = kv[0];
    char *value = kv[1];

    if (strcmp(key, "resistorlist") == 0) {
      char ** resistorlist;
      split_on_string(value, "%2C", &resistorlist, &nresistors);
      
      resistors = malloc(nresistors*sizeof(double));
      for (int i = 0; i < nresistors; i++)
        resistors[i] = atof(resistorlist[i]);
    }
    else if (strcmp(key, "ratio") == 0) {
      ratio = atof(value);
    }
    else if (strcmp(key, "number") == 0) {
      number = atoi(value);
    }
    else {
      timestamp("unknown key \"%s\"", key);
    }

  }


  if (resistors == NULL) {
    timestamp("get_resistors: resistors not specified");
    return;
  }

  if (ratio == 0.0) {
    timestamp("get_resistors: ratio cannot be zero/it wasn't set.");
    return;
  }

  if (number < 0) {
    timestamp("get_resistors: number of resistors cannot be negative/it wasn't set.");
    return;
  }

  char res[1000] = {0};

  snprintf(res, 1000, 
      HTML_HEADER("Resister Comination Results")
      "    <h1>Result</h1>");
  // snprintf(res, 1000, 
  //     HTML_HEADER("Resister Comination Results")
  //     "    <h1>Result</h1>\n"
  //     "    <p>number = %d, ratio = %f</p>\n"
  //     "    <a href=\"reschoose.html\">back</a>\n"
  //     HTML_FOOTER, 
  //     number, ratio);



  double lb = ratio*0.9;
  double ub = ratio*1.1;

  int nsolutions = 0;

  char solutions[1000] = {0};

  strcat(solutions, "<ul>");
  for (int i = 0; i < nresistors; i++) {
    for (int j = i+1; j < nresistors; j++) {
      double ri = resistors[i];
      double rj = resistors[j];
      double rat = ri/rj, ratinv = rj/ri;
      if ((rat >lb && rat < lb) ||  (ratinv > lb && ratinv < ub)) {
        char sol[100] = {0};
        sprintf(sol, "<li>%.0f&Omega; and %.0f&Omega;</li>", ri, rj);
        strcat(solutions, sol);
        nsolutions ++;
      }
    }
  }
  strcat(solutions, "</ul>");

  char nsolstr[100] = {0};
  if (nsolutions) {
    sprintf(nsolstr, "<h2>%d %s found</h2>", nsolutions, (nsolutions == 1)?"solution was":"solutions were");
    strcat(res, nsolstr);
    strcat(res, solutions);
  }
  else {
    sprintf(nsolstr, "<h2>No solutions were found</h2>");
    strcat(res, nsolstr);
  }

  strcat(res, "<a href=\"reschoose.html\">back</a>");
  strcat(res, HTML_FOOTER);

  free(resistors);

  send_response(cd, RESP_200, "text/html", (void *)res, strlen(res));

}
