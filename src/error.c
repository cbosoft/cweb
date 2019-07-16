#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "error.h"


void ferr(const char * source, const char * fmt, ...)
{
  char mesg[MESGLEN] = {0};

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, MESGLEN-1, fmt, ap);
  va_end(ap);

  fprintf(stderr, "  "BG_RED"FATAL ERROR!"RESET" in "FG_BLUE"%s"RESET" %s", source, mesg);
  if (errno)
    fprintf(stderr, " (%d) %s", errno, strerror(errno));
  fprintf(stderr, "\n");

  exit(1);
}





void timestamp(const char *fmt, ...)
{

  char mesg[MESGLEN] = {0};

  va_list ap;

  va_start(ap, fmt);
  vsnprintf(mesg, MESGLEN-1, fmt, ap);
  va_end(ap);

  time_t rt;
  struct tm *info;
  char timestr[MESGLEN] = {0};

  time(&rt);
  info = localtime(&rt);
  strftime(timestr, MESGLEN-1, "%x %X", info);

  fprintf(stdout, "(%s) %s\n", timestr, mesg);
}
