#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <mutex>
#include <vector>
#include <algorithm>
template <typename T>
class CircularBuffer
{
public:
    CircularBuffer<T>(int size):v(size, 0), head(size-1){}
    void add(T value){
        std::lock_guard<std::mutex> lg(m);
        head=(head+1)%v.size();
        v[head]=value;
    }
    T get()
    {
        std::lock_guard<std::mutex> lg(m);
        v.at(head);
    }
    T getMax(){
        std::lock_guard<std::mutex> lg(m);
        return *std::max_element(v.begin(), v.end());
    }
private:
    std::mutex m;
    std::vector<T> v;
    int head;
};

#endif // CIRCULARBUFFER_H
