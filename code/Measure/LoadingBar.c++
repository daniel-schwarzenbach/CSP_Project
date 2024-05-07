#include <Measure/LoadingBar.h++>

namespace measure{

// init loading bar
LoadingBar::LoadingBar(uint width) : barWidth(width){}

// set loadin bar in percent
void LoadingBar::update(flt percent)
{
    if (percent >= 0 && percent <= 100.0)
    {
        progress = percent;
    }
    // get the position of the marker
    uint pos = static_cast<uint>(barWidth * progress / 100.0);
    std::cout << "[";
    // rewrite the output
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    // end with a persent and set \r
    std::cout << "] " << setw(9) << std::left
              << progress << "%  \r";
    // flush the output
    std::cout.flush();
}

// delete loading bar
LoadingBar::~LoadingBar(){
    // make a linebrake
    cout << endl;
    // delete barWidth
    ~barWidth;
}

}