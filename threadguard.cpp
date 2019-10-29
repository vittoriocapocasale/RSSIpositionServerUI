#include "threadguard.h"

ThreadGuard::ThreadGuard(std::thread& t):t(std::move(t))
{
}

ThreadGuard::~ThreadGuard (){
    if(t.joinable())
    {
        t.join();
    }
}

std::thread& ThreadGuard::get(){
    return t;
}
