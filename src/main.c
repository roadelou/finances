#include "finances.h"

// Temporary functions
static Error interaction_loop(FILE *dest);
static Error report(FILE *src);

/*
For now this file only contains a big main to interact with the user.
*/

int main(int argc, char const *argv[]) {
  if (argc == 1) {
    return 0;
  }
  // else...

  if (argc == 3) {
    FILE *file = fopen(argv[1], "w");
    interaction_loop(file);
  }
  else {
    FILE *file = fopen(argv[1], "r");
    report(file);
  }
  return 0;
}

Error interaction_loop(FILE *dest) {
  printf("%s\n", "Welcome to your finances tool.");

  while (1) {
    printf("%s: ", "Input reason of next bill");
    char buffer[100];
    int request_code = scanf("%s\n", (char *) &buffer);
    if (request_code == EOF) {
      break;
    }
    else {
      printf("%s: ", "Input amount of next bill");
      float amount;
      int float_code = scanf("%f\n", &amount);
      if (float_code == EOF) {
        break;
      }
      // else...

      Bill *bill = bill_new(amount, buffer);
      free(buffer);
      bill_to_file(bill, dest);
      bill_free(bill);
      printf("%s\n", "Bill successfully added");
    }
  }

  return ok();
}

static Error report(FILE *src) {
  while (!feof(src)) {
    printf("%s\n", "");
    Bill allocated;
    bill_from_file(&allocated, src);
    bill_print(&allocated);
    bill_free(&allocated);
  }

  return ok();
}
