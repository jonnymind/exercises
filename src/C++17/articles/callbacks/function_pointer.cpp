#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

struct Context {
    Context(int multiplier): multiplier_{multiplier} {}
    int opCount_{0};
    int multiplier_{0};
};

using Callback = int(*)(Context&, int, int);

void processor(const std::vector<int>& v, Callback cb, Context ctx)
{
    int result = 0;
    for(const auto& element: v) {
        result += cb(ctx, element, static_cast<int>(v.size()));
    }
    std::cout << "Result: " << result << '\n';
}

int main()
{
    using namespace std::literals;
    std::vector<int> data{1,2,3,4,5};
    auto lambda = [](Context& ctx, int value, int count){

        if(++ctx.opCount_>= count) {
            std::cout << "100%\n";
        }
        else {
            std::cout << static_cast<int>(ctx.opCount_ * 100.0 / count ) 
                      << "%\r" << std::flush;
            std::this_thread::sleep_for(500ms);
        }
        return ctx.multiplier_ * value;
    };

    processor(data, lambda, Context{2});
    
    return 0;
}
