#include <chrono>
#include <future>
#include <functional>
#include <iostream>
#include <thread>

void countTo(int limit, int failAt) {
    using namespace std::chrono_literals;
    for(int i = 1; i <= limit; ++i) {
        std::cout << "Counting " << i << '\n';
        if(i == failAt) {
            throw std::runtime_error("I was doomed to fail");
        }
        std::this_thread::sleep_for(250ms);
    }
}

int main() {
    try {
        std::packaged_task<void(int, int)> parallel_countTo{
            [](int limit, int failAt){
                countTo(limit, failAt);}
            };
        auto result = parallel_countTo.get_future();
        auto thread = std::thread{std::move(parallel_countTo), 10, 5};
        thread.join();
        result.get();

        std::cout << "countTo was completed\n";
        return 0;
    }
    catch(const std::exception& error) {
        std::cout << "The thread died unexpectedly with error: " << error.what() << '\n';
        return 1;
    }    
}
