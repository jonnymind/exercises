
#include <algorithm>
#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

template<typename T, typename Comp=std::less<>>
std::size_t qsort_pivot(std::vector<T>& vect, std::size_t left, std::size_t right, const Comp& comp=Comp())
{
    const T& pivot = vect[right];
    for(std::size_t j = left; j <= right; ++j)
    {
        if(comp(vect[j], pivot)) {
            std::swap(vect[j], vect[left]);
            ++left;
        }
    }

    if(left != right) {
        std::swap(vect[left], vect[right]);
    }
    return left;
}


template<typename T, typename Comp=std::less<>>
void qsort_sub(std::vector<T>& vect, std::size_t left, std::size_t right, const Comp& comp=Comp())
{
    if(left >= right) {
        return;
    }

    std::size_t pivot = qsort_pivot(vect, left, right, comp);
    
    auto f1 = std::async([&](){
        if(left+1 < pivot)
            qsort_sub(vect, left, pivot-1, comp);        
    });
    auto f2 = std::async([&](){
        if(pivot+1 < right)
            qsort_sub(vect, pivot + 1, right, comp);
    });

    f1.get();
    f2.get();
}


template<typename T, typename Comp=std::less<>>
void qsort(std::vector<T>& vect, const Comp& comp=Comp())
{
    if(vect.size() != 0) {
        qsort_sub(vect, 0, vect.size()-1, comp);
    }
}


int main() {
    std::cout << "Starting\n";

    std::vector<int> vect = {2,4,9,2,3,1,7,3,5,1,2,8};
    qsort(vect);
    for(const auto& elem: vect) {
        std::cout << elem << ", ";
    }
    std::cout<< "\n";
    return 0;
}
