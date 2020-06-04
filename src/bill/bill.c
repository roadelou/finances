#include "bill.h"

unsigned int serial_index = 0;

Bill *bill_new(const float amount, const char *reason) {
  Bill *allocated = malloc(sizeof(Bill));
  allocated->number = serial_index++; // Set bill number
  allocated->date = time(NULL);       // Set (approximative) bill date
  allocated->amount = amount;
  allocated->len = strlen(reason);
  allocated->reason = malloc(allocated->len + 1); // +1 for null termination
  strncpy(allocated->reason, reason, allocated->len + 1);
  return allocated;
}

void bill_free(Bill *bill) {
  free(bill->reason);
  free(bill);
}

Error bill_print(const Bill *bill) {
  printf("Bill number: %d\n", bill->number);
  printf("Bill date: %s", ctime(&(bill->date)));
  printf("Bill amount: %f\n", bill->amount);
  printf("Bill len: %d\n", bill->len);
  printf("Bill reason: %s\n", bill->reason);
  return ok();
}
