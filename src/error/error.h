#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#include <stdlib.h> // For exit
#include <stdio.h>  // For logging errors
#include <string.h> // For string manipulations

#ifndef ERROR_MSG_LEN
#define ERROR_MSG_LEN 100
#endif

struct Error {
  int code;                 // The error code. 0 is considered a success
  char msg[ERROR_MSG_LEN];  // A short message explainign the source of the error
};

typedef struct Error Error;

/*
A function to simply create an error. Supports printf style format string and NULL message
*/
Error error(const int code, const char *msg, ...);

/*
A shortcut for error(0, NULL)
*/
Error ok(void);

/*
A function to concisely log an error to stderr without exiting. Will not log if code is 0.
*/
int warn(const Error e);

/*
A function to exit because of an error. Will not exit if the code of the Error is 0.
*/
int fail(const Error e);

#endif /* end of include guard: ERROR_INCLUDED */
