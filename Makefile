runtest:test
	./test

test:
	g++ test.cpp -Wall -Werror -o test

clean:
	rm -f *.o
	rm -f test