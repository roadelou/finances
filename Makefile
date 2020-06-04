main_src = src/main.c
main_head = src/finances.h

bill_path = src/bill
bill_src = $(bill_path)/bill.c $(bill_path)/bill_to.c $(bill_path)/bill_from.c $(bill_path)/bill_size.c
bill_head = $(bill_path)/bill.h

error_path = src/error
error_src = $(error_path)/error.c
error_head = $(error_path)/error.h

src = $(main_src) $(bill_src) $(error_src)
head = $(main_head) $(bill_head) $(error_head)
include_flags = -I$(error_path) -I$(bill_path)

out = dist/finances
debug = dist/finances.elf

.phony: clean install

$(out): $(src) $(head)
	clang $(include_flags) $(src) -o $(out)

$(debug): $(src) $(head)
	clang $(include_flags) -O0 -g $(src) -o $(debug)

clean:
	rm -rf $(out) $(debug)

install: $(out)
	cp $(out) $(HOME)/localApp/
