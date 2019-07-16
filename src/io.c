#include <stdlib.h>
#include <stdio.h>

#include "io.h"

const char *read_file(const char *path)
{
  FILE *fp = fopen(path, "r");

  if (fp == NULL)
    return NULL;

  int nc = 0;
  char ch;
  for (ch = fgetc(fp); ch != EOF; nc++) ch = fgetc(fp);

  fseek(fp, 0, SEEK_SET);

  char *contents = calloc(nc+1, sizeof(char));
  
  int i = 0;
  while (1) {
    ch = fgetc(fp);
    
    if (ch == EOF)
      break;

    contents[i] = ch;
    i++;
  }

  return contents;
}
