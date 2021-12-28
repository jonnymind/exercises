.PHONY: prepare clean

all: prepare future thread_future thread_package thread_app

clean:
	rm -rf build

prepare:
	mkdir -p build

%: %.cpp
	clang++-11 -g -lpthread --std=c++20 -o build/$@ $@.cpp  

