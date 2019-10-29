#ifndef THREADGUARD_H
#define THREADGUARD_H
#include <thread>

class ThreadGuard
{
public:
    ThreadGuard(std::thread& t);
    ThreadGuard(const ThreadGuard& t)=delete;
    ~ThreadGuard();
    ThreadGuard& operator=(const ThreadGuard& t)=delete;
    std::thread& get();
private:
    std::thread t;
};

#endif // THREADGUARD_H
