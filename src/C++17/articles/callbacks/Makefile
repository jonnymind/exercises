.PHONY: prepare clean

all: prepare function function_pointer survival functor method interface

clean:
	rm -rf build

prepare:
	mkdir -p build

%: %.cpp
	clang++-11 -g -lpthread --std=c++20 -o build/$@ $@.cpp  

