#pragma once

#include <functional>
#include <thread>
#include <chrono>
#include <atomic>

/**
 * Timer class that periodically calls a function.
 */
class Timer
{
public:

    /**
     * Constructor.
     *
     * @param timeout in ms
     */
    Timer(std::function<void()> callback, int timeout);

    /**
     * Start the timer.
     */
    void start();

    /**
     * Stop the timer.
     */
    void stop();

    void setTimeout(int timeout)
    {
        timeout = timeout;
    }

private:
    std::function<void()> callback;
    int timeout;
    std::thread timerThread;
    std::atomic<bool> running;
};
