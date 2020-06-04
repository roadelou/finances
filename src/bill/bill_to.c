#include "bill.h"

Error bill_to_ptr(const Bill *bill, void *dest, const int len) {
  // First thing to do is to check the allocated size
  size_t size;
  Error bill_sizeof_error = bill_sizeof(bill, &size);
  fail(bill_sizeof_error);
  // else...

  if (size + 1 > len) {
    return error(-1, "The size allocated to serialize bill %d was %d bytes, but it needs %ld", bill->number, len, size);
  }
  // else...

  Bill *dest_as_bill = (Bill *) dest;

  memcpy(dest_as_bill, bill, bill_sizeof_fixed());

  /*
  What follows is kind of a hack. Because the only difference between the serialized Bill and the one in memory is that reason contains the whole string and not just a pointer to it, we copy the content of the string to the location where its pointer would normally be.
  */
  strncpy((char *) &(dest_as_bill->reason), bill->reason, bill->len + 1);  // +1 for null termination
  return ok();
}

Error bill_to_file(const Bill *bill, FILE *dest) {
  // First thing to do is to check the allocated size
  size_t size;
  Error bill_sizeof_error = bill_sizeof(bill, &size);
  fail(bill_sizeof_error);
  // else...

  // We write the beginning of the bill to the file
  size_t beginning = bill_sizeof_fixed();
  size_t written_beginning = fwrite(bill, 1, beginning, dest);

  // We check if the file was correctly written to
  if (written_beginning != beginning) {
    return error(-1, "Expected to write %ld bytes to file for bill %d, but only wrote %ld instead", beginning, bill->number, written_beginning);
  }
  // else...

  // We write the content of the reason to the file. +1 for termination
  size_t written_reason = fwrite(bill->reason, 1, bill->len + 1, dest);

  // We check if the right amount of bytes has been written
  if (written_reason != bill->len + 1) {
    return error(-2, "Expected to write %d bytes for reason of bill %d, but only wrote %ld instead", bill->len + 1, bill->number, written_reason);
  }
  // else...

  return ok();
}
