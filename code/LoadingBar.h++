#include <Base.h++>

class LoadingBar{
private:
    // 0 - 100
    uint barWidth = 100;
    F64 progress = 0.0;
public:
    // init loading bar with 0%
    LoadingBar(uint barWith);
    // set loadin bar in percent
    void update(F64 percent);
    // delete the loading bar
    ~LoadingBar();
};