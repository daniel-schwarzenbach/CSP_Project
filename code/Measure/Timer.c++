#include <Measure/Timer.h++>
namespace measure
{

flt Timer::get_difference(Time const &e, Time const s)
{
    std::chrono::duration<double> duration =
        static_cast<std::chrono::duration<double>>(
            (e - s).count());
    return static_cast<double>(duration.count());
}
Timer::Time Timer::now() { 
    return std::chrono::high_resolution_clock::now(); 
}

Timer::Timer()
{
    end_ = now();
    start_ = now();
}
// start or restart timer
void Timer::start()
{
    start_ = now();
}
void Timer::stop()
{
    end_ = now();
}

// return time on the timer in seconds
flt Timer::time()
{   // get current time
    Time nowTime;
    if (get_difference(end_, start_) > 0)
        nowTime = end_; // if timer has already been stoped
    else
        // if timer is still runnign
        nowTime = std::chrono::high_resolution_clock::now();
    // return time difference
    return get_difference(nowTime, start_) / 1e+09;
}

}