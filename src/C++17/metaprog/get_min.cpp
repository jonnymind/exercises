
#include <iostream>
#include <string>
#include <algorithm>

template<int... >
struct get_min;

template<>
struct get_min<>
{
};

template<int A>
struct get_min<A>
{
    static const int result{A};
};


template<int A, int B>
struct get_min<A, B>
{
    static const int result{std::min(A, B)};
};

template<int A, int B, int... Cs>
struct get_min<A, B, Cs...>
{
    static const int result{std::min(A, get_min<B, Cs...>::result)};
};


int main(int argc, char* argv[])
{
    return get_min<40,20,19,10,4,5>::result;
}
