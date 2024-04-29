#include <LoadingBar.h++>

// init loading bar with 0%
LoadingBar::LoadingBar(uint width): barWidth(width){
    this->update(0.0);
}
// set loadin bar in percent
void LoadingBar::update(F64 percent){
    if(percent >= 0 && percent <= 100.0){
        progress = percent;
    }
    uint pos = static_cast<uint>(barWidth * progress/100.0);
    std::cout << "[";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }
    std::cout   << "] " << setw(9) << std::left
                << progress << " %               \r";
    std::cout.flush();
}

LoadingBar::~LoadingBar(){
    update(100.0);
    cout << endl;
    ~barWidth;
}