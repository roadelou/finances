src = error.c
head = error.h
test = test.c
out = test.elf

.PHONY: clean

$(out): $(src) $(head) $(test)
	clang $(src) $(test) -o $(out)

clean:
	rm -rf $(out)
