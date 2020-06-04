// This file contains the interface to the Bill type

#ifndef BILL_INCLUDED
#define BILL_INCLUDED

#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For FILE manipulations
#include <time.h>   // For time_t manipulations
#include <string.h> // For string manipulations

#include "error.h"  // For custom error handling

/*
One instance of a this struct represents one bill

IMPORTANT : This structure is NOT PACKED on standard x86_64 architecture !!!
*/
struct Bill {
  time_t date;  // The date of the bill
  unsigned int number;  // A number to identify the bill
  float amount; // The amount that was paid
  unsigned int len; // The allocated size of the string that follows
  char *reason; // What was paid with this bill
};

typedef struct Bill Bill;

/*
Global value used to set the number field of a Bill automatically. Used and updated by bill_new, also updated by bill_from (all versions).
*/
extern unsigned int serial_index;

/*
Allocates memory for a new bill
*/
Bill *bill_new(const float amount, const char *reason);

/*
Frees memory of an old bill
*/
void bill_free(Bill *bill);

/*
Prints the content of the bill (usefull mostly for debug)
*/
Error bill_print(const Bill *bill);

/*
bill is the Bill to deserialize to, src is the pointer to read from.
*/
Error bill_from_ptr(Bill *bill, const void *src);

/*
bill is the bill to serialize, dest is the pointer to write the serialized data to and len is the allocated size of dest
*/
Error bill_to_ptr(const Bill *bill, void *dest, const int len);

/*
Deserializes a Bill directly from a file
*/
Error bill_from_file(Bill *bill, FILE *src);

/*
Serializes a Bill directly to a file
*/
Error bill_to_file(const Bill *bill, FILE *dest);

/*
A helper function to get the size in bytes of a bill once it will be serialized
*/
Error bill_sizeof(const Bill *bill, size_t *size);

/*
Another helper function to get the size of a bill once it will be deserialized from a pointer
*/
Error bill_len_ptr(const void *src, size_t *size);

/*
Another helper function to get the size of a bill once it will be deserialized from a file. It uses the rewind function.
*/
Error bill_len_file(FILE *src, size_t *size);

/*
A small helper routine to get the plateform dependant size of the beginning of the beginning of a bill.
*/
size_t bill_sizeof_fixed(void);
#endif /* end of include guard: BILL_INCLUDED */
