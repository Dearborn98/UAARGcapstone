#include "timer.h"

Timer::Timer(std::function<void()> callback, int timeout)
    : callback(callback), timeout(timeout), running(false) {}

void Timer::start()
{
    if (running || timerThread.joinable()) {
        return;
    }

    running = true;
    timerThread = std::thread([this]()
                              {
        for (;;) {
            if (running) {
                callback();
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
            } else {
                break;
            }
        } });
}

void Timer::stop()
{
    if (!running || !timerThread.joinable()) {
        return;
    }
    running = false;
    timerThread.join();
}
