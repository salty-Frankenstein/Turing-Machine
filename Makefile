DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CXXFLAGS =-D DEBUG -Wall	# DEBUG 
else
    CXXFLAGS =-D NDEBUG -O2		# release
endif

OBJS = config.o parsec.o ir.o parser.o interpreter.o shell.o

main:${OBJS} main.cpp
	g++ main.cpp ${OBJS} -o main 

runtest:test
	./test

test:${OBJS} test.cpp
	g++ test.cpp ${OBJS} -o test

shell:shell.cpp
	g++ shell.cpp -o shell

interpreter:interpreter.cpp 
	g++ interpreter.cpp -o interpreter.o

parser:parser.cpp 
	g++ parser.cpp -o parser.o

ir:ir.cpp 
	g++ ir.cpp -o ir.o

parsec:parsec.cpp
	g++ parsec.cpp -o parsec.o

config:config.cpp
	g++ config.cpp -o config.o

clean:
	rm -f ${OBJS}
	rm -f test
	rm -f main