// Implementation of the Bill type

#include "bill.h"

Error bill_sizeof(const Bill *bill, size_t *size) {
  /*
  This one is fairly straightforward. We just add one check to make sure that the announced length is indeed the one of the string within the Bill
  */
  size_t true_size_str = strlen(bill->reason);
  if (true_size_str != bill->len) {
    *size = 0;  // Setting size to 0 just in case
    return error(-1, "Corrupted bill %d", bill->number);
  }
  else {
    /*
    Setting size to the correct value.

    WARNING : Because the Bill structure is not packed, we cannot use bill_sizeof_fixed for this purpose
    */
    *size = sizeof(Bill) - sizeof(void *) + true_size_str;
    return ok();
  }
}

Error bill_len_ptr(const void *src, size_t *size) {
  /*
  This one is not so obvious. The simplest thing to do is to try and read the beginning of the struct from src to an allocated Bill and then read the value in len.
  */
  Bill *allocated = (Bill *) src;
  *size = allocated->len;
  return ok();
}

Error bill_len_file(FILE *src, size_t *size) {
  /*
  Even harder, this time we have to read the file once then rewind it.
  */
  long current_index = ftell(src);

  Bill allocated; // We allocate some memory to read the file to.
  size_t beginning = bill_sizeof_fixed();
  size_t bytes_read = fread((void *) &allocated, 1, beginning, src);

  // We have to get back to the former index in the file.
  int status_rewind = fseek(src, current_index, SEEK_SET);
  if (status_rewind == -1) {
    *size = 0;
    return error(-2, "Could not rewind source file after reading size of bill");
  }
  // else...

  if (bytes_read != beginning) {
    *size = 0;
    return error(-1, "Could not get size of bill from source file");
  }
  else {
    *size = allocated.len;
    return ok();
  }
}

size_t bill_sizeof_fixed(void) {
  return sizeof(time_t) + sizeof(float) + 2 * sizeof(unsigned int);
}
