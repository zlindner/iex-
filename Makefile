CC := clang++ # macOS compiler
#CC := g++ alternative compiler
CFLAGS := -g -Wall -std=c++17 -I include -L /usr/local/lib
LIBS := -l curl

iexpp:
	$(CC) $(CFLAGS) $(LIBS) $(shell find . -name '*.cpp') -o bin/iexpp

start:
	./bin/iexpp

clean:
	rm -rf bin/*
	rm -rf build/*