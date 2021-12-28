#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class Application {
public:
    // Usual machinery omitted for brevity.
    
    void log_error(const std::string& what) {
        std::cerr << "LOG ERROR: " << what << '\n';
    }

    class thread
    {
    public:
        thread(const thread&) = delete;

        thread(thread&& other) noexcept: 
            name_{std::move(other.name_)}, runner_{std::move(other.runner_)}
            {}

        void join() {
            runner_->join();
        }

        // Similarly detach()
    private:
        template<class Function, class... Args>
        explicit thread(Application* owner, const std::string& name, Function&& f, Args&&... args):
            name_{name} {
            runner_ = std::make_unique<std::thread>([&f, &args..., owner, this] {
                try {
                    f(args...);
                }
                catch(const std::exception& error){
                    owner->log_error(
                        std::string{"Thread `"} + name_ + "` terminated with error: " + error.what());
                }});
            }
               
        std::string name_;
        std::unique_ptr<std::thread> runner_;
        friend class Application;
    };

    template<class Function, class... Args> 
    thread make_thread(const std::string& name, Function&& f, Args&&... args) {
        return thread{this, name, std::forward<Function>(f), std::forward<Args>(args)...};
    }
};

void count_to(int limit, int fail_at) {
    using namespace std::chrono_literals;
    for(int i = 1; i <= limit; ++i) {
        std::cout << "Counting " << i << '\n';
        if(i == fail_at) {
            throw std::runtime_error("I was doomed to fail");
        }
        std::this_thread::sleep_for(250ms);
    }
}

int main() {
    Application the_app;
    auto good_thread = the_app.make_thread("Good Thread", count_to, 10, 0);
    auto bad_thread = the_app.make_thread("Bad Thread", count_to, 10, 5);
    good_thread.join();
    bad_thread.join();
    // Note: we should catch exceptions here too.
    return 0;
}
