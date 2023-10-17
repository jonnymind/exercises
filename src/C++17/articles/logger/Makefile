.PHONY: prepare clean

all: prepare logger

clean:
	rm -rf build

prepare:
	mkdir -p build

%: %.cpp prepare
	clang++ -g -lpthread -lgtest -lgtest_main -L/usr/local/lib --std=c++20 -I. -I/usr/local/include -o build/$@ $@.cpp  

