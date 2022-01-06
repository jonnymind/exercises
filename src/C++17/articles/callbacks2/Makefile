.PHONY: prepare clean

all: prepare 

clean:
	rm -rf build

prepare:
	mkdir -p build

build/%: %.cpp
	clang++-11 -g -lpthread --std=c++20 -o $@ $<

