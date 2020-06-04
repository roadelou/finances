#include "error.h"

int main(int argc, char const *argv[]) {
  Error testSucces = error(0, "Test of success.");
  printf("%s\n", "Testing warn in case of success");
  warn(testSucces);
  printf("%s\n", "Testing fail in case of success");
  fail(testSucces);

  Error testFail = error(-1, "Test of failure.");
  printf("%s\n", "Testing warn in case of failure");
  warn(testFail);
  printf("%s\n", "Testing fail in case of failure");
  fail(testFail);
}
