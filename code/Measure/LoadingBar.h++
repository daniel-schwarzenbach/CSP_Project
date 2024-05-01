#ifndef __LOADINGBAR_H__
#define __LOADINGBAR_H__
/*
    create a loading bar in the terminal
    usefull for large wait times
*/

#include <Base.h++>

namespace measure{

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

}
#endif // __LOADINGBAR_H__