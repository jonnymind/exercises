#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

class CallbackInterface
{
public:
    virtual void onStart(int count) const = 0;
    virtual int processElement(int element) const = 0;
    virtual void onComplete(int total) const = 0;
};

struct Multiplier
{
    class CallbackAdapter: public CallbackInterface
    {
    public:
        CallbackAdapter(Multiplier* owner): owner_{owner} {}

        virtual void onStart(int count) const override {
            owner_->setSize(count);
        }
        virtual int processElement(int element) const override {
            return owner_->multiply(element);
        }
        virtual void onComplete(int total) const override {
            std::cout << "Final result: " << total << '\n';
        }

    private:
        Multiplier* owner_;
    };

    Multiplier(int multiplier): 
        adapter_{this},
        multiplier_{multiplier} {}
        
    void setSize(int size) {size_ = size;}

    int multiply(int value) {
        using namespace std::chrono_literals;
        std::cout << "Progress: " << (++count_ * 100.0 / size_) << "%\r" << std::flush;
        if(count_ < size_) {
            std::this_thread::sleep_for(500ms);
        } else {
            std::cout << '\n';

        }
        return value * multiplier_;
    }

    const CallbackInterface& getAdapter() const {return adapter_;}

private:
    CallbackAdapter adapter_;
    int multiplier_;
    int size_;
    int count_{0};
};

void processor(const std::vector<int>& v, const CallbackInterface& cb)
{
    cb.onStart(static_cast<int>(v.size()));
    int result = 0;
    for(const auto& element: v) {
        result += cb.processElement(element);
    }
    cb.onComplete(result);
}

int main()
{
    std::vector<int> data{1,2,3,4,5};
    processor(data, Multiplier{2}.getAdapter());    
    return 0;
}
