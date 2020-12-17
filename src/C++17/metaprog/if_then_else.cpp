
#include <iostream>
#include <string>
#include <algorithm>

template<bool cond, typename THEN, typename ELSE>
struct IF
{
    template<bool> struct selector;

    template<>
    struct selector<true> {
        using selected = THEN;
    };

    template<>
    struct selector<false> {
        using selected = ELSE; 
    };

    using result = typename selector<cond>::selected;
};

struct onTrue {
    static void doit() {
        std::cout << "It's true\n";
    }
};


struct onFalse {
    static void doit() {
        std::cout << "It's false\n";
    }
};

template<void(p)()> void indirect()
{
    p();
}

void other()
{
    std::cout<< "I am the other\n";
}

int main(int argc, char* argv[])
{
    IF<sizeof(int)==2, onTrue, onFalse>::result::doit();
    indirect<other>();
    return 0;
}
