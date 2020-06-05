#include "bill.h"
#include <mcheck.h> // I had memory corruption issues, hence this check

int main(int argc, char const *argv[]) {

  printf("%ld\n", sizeof(Bill));
  mcheck_pedantic(NULL);
  Bill *bill = bill_new(10.19, "Test");
  mcheck_check_all();

  bill_print(bill);

  size_t size;
  bill_sizeof(bill, &size);
  printf("Bill sizeof: %ld\n", size);

  Bill *buffer = malloc(size + 1);
  bill_to_ptr(bill, buffer, size + 1);

  printf("Bill number: %d\n", buffer->number);
  printf("Bill date: %s", ctime(&(buffer->date)));
  printf("Bill amount: %f\n", buffer->amount);
  printf("Bill len: %d\n", buffer->len);
  printf("Bill reason: %s\n", (char *)&(buffer->reason));

  mcheck_check_all();

  Bill recover;
  bill_from_ptr(&recover, buffer);
  mcheck_check_all();

  bill_print(&recover);

  // bill_free(bill);
  free(buffer);
  free(recover.reason);
  printf("%s\n", "vert");

  FILE *test_file = fopen("test.bill", "w");
  for (size_t i = 0; i < 5; i++) {
    bill_to_file(bill, test_file);
  }

  fclose(test_file);

  test_file = fopen("test.bill", "r");
  Bill from_file;
  for (size_t i = 0; i < 5; i++) {
    bill_from_file(&from_file, test_file);
  }
  fclose(test_file);

  bill_print(&from_file);

  free(from_file.reason);
  bill_free(bill);

  return 0;
}
