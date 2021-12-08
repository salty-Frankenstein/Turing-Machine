runtest:test
	./test

test:ir.o
	g++ test.cpp ir.o -Wall -Werror -o test

ir:
	g++ ir.cpp -Wall -Werror -o ir.o

clean:
	rm -f *.o
	rm -f test