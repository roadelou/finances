#include "finances.h"

// Temporary functions
static Error interaction_loop(FILE *dest);
static Error report(FILE *src, const int output);
static Error monthly(FILE *src);

/*
For now this file only contains a big main to interact with the user.
*/

int main(int argc, char const *argv[]) {
  if (argc == 1) {
    return 0;
  }
  // else...

  FILE *file;
  Error status;
  if (argc == 3) {
    if (argv[2][0] == 'i') {
      file = fopen(argv[1], "r");
      // First, we have to read the file in order to get to its end and to
      // update the serial index.
      status = report(file, 0);
      fail(status);
      fclose(file);

      // Then we start the inteaction loop.
      file = fopen(argv[1], "a");
      status = interaction_loop(file);
    } else if (argv[2][0] == 'm') {
      file = fopen(argv[1], "r");
      status = monthly(file);
      fail(status);
    } else {
      status = error(-1, "Unknown option %c", argv[2][0]);
    }
  } else {
    file = fopen(argv[1], "r");
    status = report(file, 1);
  }
  fclose(file);
  fail(status);
  return 0;
}

// dest must be open in append mode
Error interaction_loop(FILE *dest) {
  Error status;

  printf("%s\n", "Welcome to your finances tool.");

  while (1) {
    printf("%s: ", "Input reason of next bill");
    char buffer[100];
    char *read_status = fgets(buffer, 100, stdin);
    if (read_status == NULL) {
      break;
    }
    // else...

    // We have to remove the \n character at the end of the buffer.
    int index = strlen(buffer);
    buffer[index - 1] = 0; // Null termination

    printf("%s: ", "Input amount of next bill");
    char buffer_amount[100];
    float amount;
    char *float_status = fgets(buffer_amount, 100, stdin);
    if (float_status == NULL) {
      break;
    }
    // else...

    // We have to remove the \n character at the end of the buffer.
    index = strlen(buffer_amount); // >=1 because of the \n
    buffer_amount[index - 1] = 0;

    int scan_status = sscanf(buffer_amount, "%f", &amount);
    if (scan_status == 0) {
      return error(-1, "Invalid input %s is not a float", buffer_amount);
    }

    Bill *bill = bill_new(amount, buffer);
    status = bill_to_file(bill, dest);
    fail(status);
    bill_free(bill);
    printf("%s\n", "Bill successfully added");
  }

  putchar('\n'); // For the style
  return ok();
}

static Error report(FILE *src, const int output) {
  Error status;

  time_t now = time(NULL);
  int now_month = gmtime(&now)->tm_mon; // The current month

  while (1) {
    int is_eof = fgetc(src);
    if (is_eof == EOF) {
      break;
    } else {
      ungetc(is_eof, src);
    }
    Bill allocated;
    status = bill_from_file(&allocated, src);
    fail(status);

    if (output) {
      int bill_month = gmtime(&(allocated.date))->tm_mon; // bill month
      if (bill_month == now_month) {
        putchar('\n');
        bill_print(&allocated);
      }
    }
    free(allocated.reason);
  }

  return ok();
}

static Error monthly(FILE *src) {
  Error status;
  float total = 0;

  time_t now = time(NULL);
  struct tm *now_tm = gmtime(&now);
  int now_month = now_tm->tm_mon; // The current month
  int now_day = now_tm->tm_mday;  // The current day of the month

  int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  while (1) {
    int is_eof = fgetc(src);
    if (is_eof == EOF) {
      break;
    } else {
      ungetc(is_eof, src);
    }
    Bill allocated;
    status = bill_from_file(&allocated, src);
    fail(status);

    int bill_month = gmtime(&(allocated.date))->tm_mon; // bill month
    if (bill_month == now_month) {
      total += allocated.amount;
    }

    free(allocated.reason);
  }

  printf("Total expenses this month: %f€, %d more days to go.\n", total,
         days_in_month[now_month] - now_day);
  return ok();
}
