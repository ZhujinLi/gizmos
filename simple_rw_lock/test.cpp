#include "simple_rw_lock.h"
#include <thread>
#include <chrono>
#include <iostream>

SimpleRWLock rwLock;

void read()
{
    rwLock.lockR();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    rwLock.unlockR();
}

void write()
{
    rwLock.lockW();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    rwLock.unlockW();
}

void tictoc()
{
    static bool flag;
    static std::chrono::time_point<std::chrono::high_resolution_clock> time;

    if (!flag) // tic
    {
        time = std::chrono::high_resolution_clock::now();
    }
    else // toc
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - time;
        std::cout << "Elapsed time: " << elapsed.count() / 1e9 << "s" << std::endl;
    }

    flag = !flag;
}

int main()
{
    std::cout << "read & write:" << std::endl;
    tictoc();
    {
        std::thread t1(read);
        std::thread t2(write);
        t1.join();
        t2.join();
    }
    tictoc();

    std::cout << "write & write:" << std::endl;
    tictoc();
    {
        std::thread t1(write);
        std::thread t2(write);
        t1.join();
        t2.join();
    }
    tictoc();

    std::cout << "read & read:" << std::endl;
    tictoc();
    {
        std::thread t1(read);
        std::thread t2(read);
        t1.join();
        t2.join();
    }
    tictoc();

    std::cout << "read & read & write:" << std::endl;
    tictoc();
    {
        std::thread t1(read);
        std::thread t2(read);
        std::thread t3(write);
        t1.join();
        t2.join();
        t3.join();
    }
    tictoc();

    return 0;
}
