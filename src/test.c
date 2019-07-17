#include <stdio.h>
#include <string.h>

#include "util.h"


int main(void)
{ 
  char * alpha = "a b c";
  char * numer = "1, 2, 3, 4";

  char ** alpha_split;
  int alpha_n;
  char ** numer_split;
  int numer_n;

  split_on_string(alpha, " ", &alpha_split, &alpha_n);
  split_on_string(numer, ", ", &numer_split, &numer_n);

  fprintf(stderr, "%s\n", alpha);
  for (int i = 0; i < alpha_n; i++) fprintf(stderr, "%s\n", alpha_split[i]);
  fprintf(stderr, "%s\n", numer);
  for (int i = 0; i < numer_n; i++) fprintf(stderr, "%s\n", numer_split[i]);

  char * cr = "abc\rdef\rghi";
  char * lf = "abc\ndef\nghi";
  char * crlf = "abc\r\ndef\r\nghi";

  char * cr_nl = infer_newline(cr);
  char * lf_nl = infer_newline(lf);
  char * crlf_nl = infer_newline(crlf);

  fprintf(stderr, "cr: \n");
  for(int i = 0; i < strlen(cr_nl); i++) fprintf(stderr, " %d\n", (int)cr_nl[i]);
  fprintf(stderr, "lf: \n");
  for(int i = 0; i < strlen(lf_nl); i++) fprintf(stderr, " %d\n", (int)lf_nl[i]);
  fprintf(stderr, "crlf: \n");
  for(int i = 0; i < strlen(crlf_nl); i++) fprintf(stderr, " %d\n", (int)crlf_nl[i]);


}
