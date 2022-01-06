set(CMAKE_CXX_STANDARD 17)

find_package(Threads)

add_executable(qsort
	C++17/qsort.cpp)

add_executable(qsort_mt
	C++17/qsort_mt.cpp)
target_link_libraries (qsort_mt ${CMAKE_THREAD_LIBS_INIT})
