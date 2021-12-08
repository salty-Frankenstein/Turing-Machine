runtest:test
	./test

test:ir.o parser.o parsec.o
	g++ test.cpp ir.o parser.o parsec.o -Wall -Werror -o test

parser:ir.o parsec.o
	g++ parser.cpp ir.o parsec.o -Wall -Werror -o parser.o

ir:
	g++ ir.cpp -Wall -Werror -o ir.o

parsec:
	g++ parsec.cpp -Wall -Werror -o parsec.o

clean:
	rm -f *.o
	rm -f test