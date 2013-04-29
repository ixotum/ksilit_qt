/*******************************************************************************
  FILE: dbg.cpp
  AUTOR: ixotum
  DATE: 2011-08-26 16:34:02
  DESCRIPTION: Implementation of debug routines.
*******************************************************************************/

#include <stdarg.h>
#include <stdio.h>

#include "dbg.h"
#include "defines.h"

/*******************************************************************************
  NAME: logprint
  DESCRIPTION: The function for string output to log file.
  ARGUMENTS:
  Input:
    char *string - string for log printing
    ... - various arguments
  Output:
    void
  RETURN VALUE: int
    0 - if successful
    ERROR_OPENING_FILE - Can not open log file
*******************************************************************************/
int logprint(const char *string, ...){
  va_list argList;
  FILE *fd_out;

  va_start(argList, string);

  fd_out = fopen(KSILIT_LOG_FILE_NAME, "a+");
  if (NULL == fd_out){
    printf("Error opening file: %s", KSILIT_LOG_FILE_NAME);
    return ERROR_OPENING_FILE;
  }

  vfprintf(fd_out, string, argList);

  fclose(fd_out);
  va_end(argList);

  return 0;
}

