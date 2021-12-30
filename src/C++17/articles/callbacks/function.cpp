#include <functional>
#include <iostream>
#include <vector>


using Callback = std::function<int(int)>;

void processor(const std::vector<int>& v, Callback CB)
{
    std::cout << "Process results: ";
    for(const auto& element: v) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
}

// A plain function.
int multiply_by_two(int value) {
    return value * 2;
}

// A functor.
struct Multiplier
{
    Multiplier(int multiplier): multiplier_{multiplier} {}
    // The operator() makes instances of this class callable entities.
    int operator()(int value) {return value * multiplier_;}
private:
    int multiplier_;
};

// A non-functor class.
struct MultiplierClass
{
    MultiplierClass(int multiplier): multiplier_{multiplier} {}
    // Using a standard method as callback.
    int multiply(int value) {return value * multiplier_;}
private:
    int multiplier_;
};

int main()
{
    std::vector<int> data{1,2,3,4,5};

    // Callback using a function pointer.
    processor(data, &multiply_by_two);

    // Callback using a functor.
    processor(data, Multiplier{2});

    // Callback using a method pointer (we need std::bind).
    MultiplierClass doubler{2};
    auto method = std::bind(&MultiplierClass::multiply, &doubler, std::placeholders::_1);
    processor(data, method);

    // Callback using a lambda.
    processor(data, [](int value){return value * 2;});
    return 0;
}
