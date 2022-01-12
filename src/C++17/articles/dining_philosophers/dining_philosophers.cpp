#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>

using namespace std::literals;

class Display {
public:
    Display(std::uint32_t numberOfPhilosophers) {
        std::uint32_t numOfChars = (numberOfPhilosophers-1) * 2 + 1;
        line_.resize(numOfChars, ' ');
    }

    void update(int phid, char symbol) {
        std::unique_lock guard{mtx_};
        line_[phid * 2] = symbol;
        //std::cout << std::setw(10) << turn_++ << std::setw(0) << ": " << line_ << "\n";
        std::cout << line_ << "\n";
    }

private:
    std::string line_;
    int turn_{0};
    std::mutex mtx_;
};

class Philosopher;

class DiningSurface
{
public:
    virtual bool waitForSticks(Philosopher&) = 0;
    virtual void layDownSticks(Philosopher&) = 0;
    virtual bool waitFor(std::chrono::milliseconds howLong) = 0;
};


class Stick {
public:
    int id_{-1};

    Stick() {}
    explicit Stick(int id): id_{id} {}
    Stick(const Stick&) = default;
    Stick(Stick&&) = default;
    Stick& operator=(const Stick& other) = default;
    Stick& operator=(Stick&& other) = default;

    bool isFree() { return ! isUsed_; }
 
    void pickUp() {
        isUsed_ = true;
    }

    void layDown() {
        isUsed_ = false;
    }
private:
    bool isUsed_{false};
};



class Philosopher{
public:
    int id_;
    std::shared_ptr<Stick> leftStick_;
    std::shared_ptr<Stick> rightStick_;

    Philosopher(int id, DiningSurface& table, Display& disp, int maxWait): 
        id_{id}, table_{table}, display_{disp}, random_wait_time_{100, maxWait}
    {}

    void think() {
        display_.update(id_, 'T');
        table_.waitFor(random_wait_time_(random_engine_) * 1ms);
    }

    void eat() {
        display_.update(id_, 'E');
        table_.waitFor(random_wait_time_(random_engine_) * 1ms);
    }

    void run() {
        while (true) {
            think();
            if(!table_.waitForSticks(*this)) {
                return;
            }
            eat();
            table_.layDownSticks(*this);
        }
    }

    void displayWait() {
        display_.update(id_, 'W');
    }

    void displayTerminate() {
        display_.update(id_, 'x');
    }

private:
    DiningSurface& table_;
    Display& display_;
    
    std::random_device random_device_{};
    std::mt19937 random_engine_{random_device_()};
    std::uniform_int_distribution<int> random_wait_time_;
};


class Table: public DiningSurface {
public:   
    Table(int philosopherCount, Display& display, int maxWait) {
        philosophers_.resize(philosopherCount);
        sticks_.resize(philosopherCount);
        for(int i = 0; i < philosopherCount; ++i) {
            sticks_[i] = std::make_unique<Stick>(i);
            philosophers_[i] = std::make_unique<Philosopher>(i, *this, display, maxWait);
        }

        for(int i = 0; i < philosopherCount; ++i) {
            seatPhilosophers(*philosophers_[i]);
        }
    }

    bool waitFor(std::chrono::milliseconds interval) override {
        std::unique_lock guard{mutex_};
        hasTerminated_.wait_for(guard, interval, [this](){return terminated_;});
        return ! terminated_;
    }

    bool waitForSticks(Philosopher& phil) override {
        std::unique_lock guard{mutex_};
        waitingList_.push_back(phil.id_);
        while(!canProgress(phil)) {
            phil.displayWait();
            sticksChanged_.wait(guard);
        }
        std::erase(waitingList_, phil.id_);
        
        if(terminated_) {
            return false;
        }
        phil.leftStick_->pickUp();
        phil.rightStick_->pickUp();
        return true;
    }

    void layDownSticks(Philosopher& phil) override {
        std::unique_lock guard{mutex_};
        phil.leftStick_->layDown();
        phil.rightStick_->layDown();
        sticksChanged_.notify_all();
    }

    void seatPhilosophers(Philosopher& phil) {
        int id = phil.id_;
        phil.leftStick_ = sticks_[id];
        id++;
        if(id >= sticks_.size()) {
            id = 0;
        }
        phil.rightStick_ = sticks_[id];
    }

    bool canProgress(Philosopher& phil) {
        return terminated_ || phil.leftStick_->isFree() && phil.rightStick_->isFree()
            && (phil.id_ == waitingList_.front() 
                || 2 == std::count_if(waitingList_.begin(), waitingList_.end(), 
                    [this,id = phil.id_](int waitingId){return adjacent(waitingId, id);}
                    ));
    }

    bool adjacent(int leftId, int rightId) {
        return leftId == rightId + 1 || leftId == rightId -1 
            || (leftId == 0 && rightId == philosophers_.size() - 1)
            || (leftId == philosophers_.size() - 1 && rightId == 0);
    }

    void start() {
        for(std::size_t i = 0; i < philosophers_.size(); ++i) {
            philThreads_.emplace_back([this, i](){philosophers_[i]->run();});
        }
    }

    void terminate(){
        {
            std::unique_lock guard{mutex_};
            terminated_ = true;
        }
        
        sticksChanged_.notify_all();
        hasTerminated_.notify_all();
        for(std::size_t i = 0; i < philosophers_.size(); ++i) {
            philosophers_[i]->displayTerminate();
            philThreads_[i].join();
        }
    }

private:
    std::vector<std::unique_ptr<Philosopher>> philosophers_;
    std::vector<std::shared_ptr<Stick>> sticks_;

    std::mutex mutex_;
    std::condition_variable sticksChanged_;
    std::condition_variable hasTerminated_;
    std::deque<int> waitingList_;
    bool terminated_{false};

    std::vector<std::thread> philThreads_;
};


int main() {
    constexpr int size = 5;
    constexpr int maxPhilosopherWait = 3000;
    constexpr int duration = 60000;

    Display disp{size};
    Table table{size, disp, maxPhilosopherWait};
    std::cout << "Starting...\n";
    table.start();

    std::this_thread::sleep_for(duration * 1ms);

    std::cout << "Terminating...\n";
    table.terminate();
    
    return 0;
}