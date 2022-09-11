.PHONY: prepare clean

all: prepare pollution

clean:
	rm -rf build

prepare:
	mkdir -p build

%: %.cpp
	clang++-11 -g -lpthread --std=c++20 -o build/$@ $@.cpp  

