bill_src = bill.c bill_from.c bill_to.c bill_size.c
bill_head = bill.h
test = test.c

error_path = $(CURDIR)/../error
error_src = $(error_path)/error.c
error_head = $(error_path)/error.h

src = $(bill_src) $(error_src)
head = $(bill_head) $(error_head)
out = test.elf

.PHONY: clean

$(out): $(src) $(test) $(head)
	clang -pedantic -Wall -I$(error_path) $(src) $(test) -o $(out)

clean:
	rm -rf $(out)
