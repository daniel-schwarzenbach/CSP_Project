#include <Data/DataHandler.h++>


int main(){
    Array2D<flt> d1 =   {{2,1,1,2,3,4,2},
                         {2,1,1,2,3,4,2},
                         {2,1,1,2,3,4,2},
                         {2,1,1,2,3,4,2}};

    data::store_alo_data("testdata.txt", d1, 1);
    
    Array2D<flt> d2 =   {{3,1,1,2,3,4,2},
                         {3,1,1,2,3,4,2},
                         {3,1,1,2,3,4,2},
                         {3,1,1,2,3,4,2}};
    data::append_alo_data("testdata.txt", d2, 5);

}