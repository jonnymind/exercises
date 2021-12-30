#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

struct Context {
    Context(int multiplier): multiplier_{multiplier} {}
    ~Context(){ std::cout << "Context cleanly destroyed\n";}
    int opCount_{0};
    int multiplier_{0};
};

using Callback = std::function<int(int, int)>;

int processor(const std::vector<int>& v, Callback cb)
{
    int result = 0;
    for(const auto& element: v) {
        result += cb(element, static_cast<int>(v.size()));
    }
    return result;
}

int main()
{

    using namespace std::literals;
    std::vector<int> data{1,2,3,4,5};
    std::future<int> expected_result;
    {
        auto ctx = std::make_shared<Context>(2);
        auto lambda = [ctx](int value, int count){
            if(++ctx->opCount_>= count) {
                std::cout << "100%\n";
            }
            else {
                std::cout << static_cast<int>(ctx->opCount_ * 100.0 / count ) 
                            << "%\r" << std::flush;
                std::this_thread::sleep_for(500ms);
            }
            return ctx->multiplier_ * value;
        };
        expected_result = std::move(
            std::async(std::launch::async, &processor, data, std::move(lambda)));
        // Here ctx exits our context.
    }

    auto result = expected_result.get();
    std::cout << "Result: " << result << '\n';
    return 0;
}
