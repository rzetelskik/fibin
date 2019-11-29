CXX = clang
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -lstdc++

.PHONY: all

all: build_tests

build_tests:
	$(CXX) $(CXXFLAGS) tests.cc -o tests

clean:
	rm -rf tests
