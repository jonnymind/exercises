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
    std::vector<int> saved_data_;
    
    Multiplier(int multiplier): 
        multiplier_{multiplier} {}
        
    int callMeBack(int value) {
        saved_data_.push_back(value);
        return value * multiplier_;
    }

private:
    int multiplier_;
};

int main()
{
    std::vector<int> data{1,2,3,4,5};

    auto multiplier = Multiplier{2};
    processor(data, std::bind(&Multiplier::callMeBack, &multiplier, std::placeholders::_1));
    std::cout << "Original data: ";
    for(const auto& element: multiplier.saved_data_) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
    
    return 0;
}
