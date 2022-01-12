.PHONY: prepare clean

all: prepare dining_philosophers

clean:
	rm -rf build

prepare:
	mkdir -p build

%: %.cpp
	clang++-11 -g -lpthread --std=c++20 -o build/$@ $@.cpp  

