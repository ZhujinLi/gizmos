/*
    A simple implementation of reader-writer lock in C++11.
    Reference: https://eli.thegreenplace.net/2019/implementing-reader-writer-locks/
*/

#include <mutex>
#include <chrono>
#include <thread>

class SimpleRWLock
{
public:
    SimpleRWLock() : m_readerCount(0) {}

    void lockR()
    {
        m_mutex.lock();
        m_readerCount++;
        m_mutex.unlock();
    }

    void unlockR()
    {
        m_mutex.lock();
        m_readerCount--;
        m_mutex.unlock();
    }

    void lockW()
    {
        for (;;)
        {
            m_mutex.lock();
            if (m_readerCount == 0)
                break;

            m_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void unlockW()
    {
        m_mutex.unlock();
    }

private:
    std::mutex m_mutex;
    int m_readerCount;
};
