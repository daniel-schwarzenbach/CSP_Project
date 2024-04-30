#include <Base.h++>

class LoadingBar
{
private:
    // 0 - 100
    uint barWidth = 100;
    flt progress = 0.0;

public:
    // init loading bar with 0%
    LoadingBar(uint barWith);
    // set loadin bar in percent
    void update(flt percent);
    // delete the loading bar
    ~LoadingBar();
};