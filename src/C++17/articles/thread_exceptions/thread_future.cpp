#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <variant>

template<class Result>
struct OpQueue {
    OpQueue(): thread_{&OpQueue::run, this} {}
    ~OpQueue() {
        push(Termination{});
        thread_.join(); 
    }

    std::future<Result> queueOp(std::function<Result()> op) {
        std::promise<Result> promise;
        auto future = promise.get_future();
        push(std::make_pair(std::move(promise), std::move(op)));
        return future;
    }

private:
    using Operation = std::pair<std::promise<Result>, std::function<Result()>>;
    using Termination = bool;
    using OptionalOp = std::variant<Operation, Termination>;
    std::queue<OptionalOp> thingsToDo_;
    std::mutex mtxThingsToDo_;
    std::condition_variable cvNewThing_;
    std::thread thread_;

    void push(OptionalOp&& op) {
        std::lock_guard<std::mutex> guard{mtxThingsToDo_};
        thingsToDo_.push(std::move(op));
        cvNewThing_.notify_one();
    }

    // We can grant we're not raising an exception.
    void run() noexcept {
        while(true) {
            OptionalOp toDo;

            try {
                // Lock scope will not include function execution.
                std::unique_lock<std::mutex> guard{mtxThingsToDo_};
                cvNewThing_.wait(guard, [this]{return !thingsToDo_.empty();});
                toDo = std::move(thingsToDo_.front());
                // Is this a termination request?
                if(std::holds_alternative<Termination>(toDo)) {
                    return;
                }
                thingsToDo_.pop();
            }
            catch(const std::exception& error) {
                // This would be a system error on conditions, mutex or memory.
                std::cout << "We got a fatal error in OpQueue internals: " <<
                    error.what() << '\n';
                // A real world application may prefer to abort().
                continue;
            }

            try{                
                auto& [promise, op] = std::get<Operation>(toDo);
                promise.set_value(op());
            }
            catch(...) {
                // Any bad outcome will be notified cleanly.
                std::get<Operation>(toDo).first.set_exception(std::current_exception());
            }
        }
    }

};

int main()
{
    try {
        OpQueue<int> ops;
        std::vector<std::function<int()>> thingsToDo = {
            []{return 1 + 1;},
            []{return 2 + 2;},
            []{return std::vector<int>{1, 2, 3}.at(5);}, // This will throw.
            []{return 4 + 4;}
            };
        std::queue<std::future<int>> results;

        for(auto& toDo: thingsToDo) {
            results.emplace(std::move(ops.queueOp(toDo)));
        }

        int count = 0;
        while(!results.empty()) {
            auto value = results.front().get();
            results.pop();
            std::cout << "Result n. " << ++count << " is " << value << '\n';
        }
    }
    catch(const std::exception& error) {
        std::cout << "Huston, we have a problem: " << error.what() << '\n';
    }

    return 0;
}
