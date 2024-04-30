#include <chrono>
#include <Base.h++>

#ifndef TIMEKEEPER_HPP
#define TIMEKEEPER_HPP

namespace measure
{

    class Timer
    {
    public:
        using Time = std::chrono::time_point<
            std::chrono::high_resolution_clock>;

    private:
        Time start_;
        Time end_;
        double get_difference(Time const &e, Time const s);
        Time now();

    public:
        Timer();
        // start or restart timer
        void start();
        // stop the timer
        void stop();
        // return time on the timer in seconds
        flt time();
    };

}

#endif // TIMEKEPPER_HPP