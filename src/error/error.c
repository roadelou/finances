#include "error.h"

Error error(const int code, const char *msg, ...) {
  Error e; // Allocating memory
  e.code = code;

  if (msg == NULL) {
    strncpy(e.msg, "(no message)", ERROR_MSG_LEN);
  } else {
    va_list args;
    va_start(args, msg);

    vsnprintf(e.msg, ERROR_MSG_LEN, msg, args);

    va_end(args);
  }

  return e;
}

Error ok(void) { return error(0, NULL); }

int warn(const Error e) {
  /*
  Log is outputed in yellow
  */
  if (e.code != 0) {
    return fprintf(stderr, "\033[93m! (%d) %s\033[m\n", e.code, e.msg);
  } else {
    return e.code; // == 0
  }
}

int fail(const Error e) {
  /*
  Fail is outputed in red
  */
  if (e.code != 0) {
    fprintf(stderr, "\033[91mX (%d) %s\033[m\n", e.code, e.msg);
    exit(e.code);
  } else {
    return e.code; // == 0
  }
}
