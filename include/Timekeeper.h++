#include <chrono>

#ifndef TIMEKEEPER_HPP
#define TIMEKEEPER_HPP

class TimeKeeper{
public:
    using Time = std::chrono::time_point<
            std::chrono::high_resolution_clock>;
private:
    Time start_;
    Time end_;
    double get_difference(Time const& e, Time const s){
        std::chrono::duration<double> duration =
                static_cast<std::chrono::duration<double>>(
                (e - s).count());
        return static_cast<double>(duration.count());
    }
    Time now(){return std::chrono::high_resolution_clock::now();}
public:
    TimeKeeper(){
        end_ = now();
        start_ = now();
    }
    // start or restart timer
    void start(){
        start_ = now();
    }
    void stop(){
        end_ = now();
    }
    // return time on the timer in seconds
    double time(){
        Time nowTime;
        if(get_difference(end_,start_) > 0)
            nowTime = end_;
        else
            nowTime = std::chrono::high_resolution_clock::now();
        
        return get_difference(nowTime,start_) / 1e+09;
    }
};

#endif // TIMEKEPPER_HPP