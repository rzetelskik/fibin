CXX = clang
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -lstdc++

.PHONY: all clean

all: install run

install: build link

build: build_fibin build_tests

link: link_tests

run: ./tests

build_fibin: 
	$(CXX) $(CXXFLAGS) -c fibin.cc -o fibin.o

build_tests:
	$(CXX) $(CXXFLAGS) -c tests.cc -o tests.o

link_tests:
	$(CXX) tests.o fibin.o -o tests

clean:
	rm -rf fibin.o tests.o
