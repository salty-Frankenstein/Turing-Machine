runtest:test
	./test

test:ir.o parser.o parsec.o test.cpp
	g++ test.cpp ir.o parser.o parsec.o -Wall -Werror -o test

parser:ir.o parsec.o parser.cpp 
	g++ parser.cpp ir.o parsec.o -Wall -Werror -o parser.o

ir:ir.cpp 
	g++ ir.cpp -Wall -Werror -o ir.o

parsec:parsec.cpp
	g++ parsec.cpp -Wall -Werror -o parsec.o

clean:
	rm -f *.o
	rm -f test