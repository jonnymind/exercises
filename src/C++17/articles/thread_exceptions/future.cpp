#include <iostream>
#include <vector>
#include <future>

int faulty(std::size_t element)
{
    static std::vector<int> shortOne{1,2,3,4,5};
    return shortOne.at(element); // This will throw.
}

int main() {
    try {
        auto future = std::async(&faulty, 100);
        std::cout << "Element at position 100: " << future.get() << '\n';
    }
    catch(std::exception& error) {
        std::cout << "Huston, we have a problem: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

