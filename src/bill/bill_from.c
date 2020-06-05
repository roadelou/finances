#include "bill.h"

Error bill_from_ptr(Bill *bill, const void *src) {
  Bill *src_as_bill = (Bill *)src;
  // First we get the beginning of the bill
  memcpy(bill, src_as_bill, sizeof(Bill));

  // The size of the string is directly accessible from the struct

  // We now allocate some size for the string
  bill->reason = malloc(bill->len + 1); // +1 for null termination

  // Finally we copy the string to its allocated buffer (once again, kind of a
  // hack)
  strncpy(bill->reason, (char *)&(src_as_bill->reason), bill->len + 1);

  // We also have to update the index (if needed)
  if (bill->number >= serial_index) {
    serial_index = bill->number + 1;
  }

  return ok();
}

Error bill_from_file(Bill *bill, FILE *src) {
  // First, we read the beginning of the bill
  size_t beginning = bill_sizeof_fixed();
  size_t read_beginning = fread(bill, 1, beginning, src);
  if (read_beginning != beginning) {
    return error(-1,
                 "Expected to read %ld bytes from file for a bill, but only "
                 "read %ld instead",
                 beginning, read_beginning);
  }
  // else...

  // We allocate some space for the reason string
  bill->reason = malloc(bill->len + 1); // +1 for null termination

  // Finally, we read the reason too
  size_t read_reason = fread(bill->reason, 1, bill->len + 1, src);
  if (read_reason != bill->len + 1) {
    return error(-2,
                 "Expected to read %d bytes for reason of bill %d, but only "
                 "read %ld instead",
                 bill->len + 1, bill->number, read_reason);
  }
  // else...

  // We also have to update the index (if needed)
  if (bill->number >= serial_index) {
    serial_index = bill->number + 1;
  }

  return ok();
}
