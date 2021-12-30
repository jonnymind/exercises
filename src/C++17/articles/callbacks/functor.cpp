#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using Callback = std::function<int(int)>;

void processor(const std::vector<int>& v, Callback cb)
{
    std::cout << "Process results: ";
    for(const auto& element: v) {
        std::cout << cb(element) << ' ';
    }
    std::cout << '\n';
}

struct Multiplier
{
    std::shared_ptr<std::vector<int>> saved_data_;
    
    Multiplier(int multiplier): 
        saved_data_{std::make_shared<std::vector<int>>()},
        multiplier_{multiplier} {}
        
    int operator()(int value) {
        saved_data_->push_back(value);
        return value * multiplier_;
    }

private:
    int multiplier_;
};

int main()
{
    std::vector<int> data{1,2,3,4,5};

    auto multiplier = Multiplier{2};
    // Actually, passing a std::function with a copy of multiplier.
    processor(data, multiplier);
    std::cout << "Original data: ";
    for(const auto& element: *multiplier.saved_data_) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
    
    return 0;
}
