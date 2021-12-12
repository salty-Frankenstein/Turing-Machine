runtest:test
	./test

test:ir.o parser.o parsec.o interpreter.o config.o test.cpp
	g++ test.cpp ir.o parser.o parsec.o interpreter.o config.o -Wall -Werror -o test

interpreter:interpreter.cpp 
	g++ interpreter.cpp -Wall -Werror -o interpreter.o

parser:parser.cpp 
	g++ parser.cpp -Wall -Werror -o parser.o

ir:ir.cpp 
	g++ ir.cpp -Wall -Werror -o ir.o

parsec:parsec.cpp
	g++ parsec.cpp -Wall -Werror -o parsec.o

config:config.cpp
	g++ config.cpp -Wall -Werror -o config.o

clean:
	rm -f *.o
	rm -f test