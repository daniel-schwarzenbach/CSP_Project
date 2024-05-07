#include <gtest/gtest.h>
#include <Heisenberg.h++>
#include <Data/DataHandler.h++>

constexpr flt _eps_ = 1e-4;

// compare to datas
bool operator==(Array2D<flt> const& d1, Array2D<flt> const& d2){
    const int Size = d1.size();
    if (Size != d2.size()){
        cerr << "not the same cols" << endl;
        return false;
    }
    cout << "Size = " << Size << endl;
    for (int i = 0; i < Size; ++i){
        if(i >= 3){
            return true;
        }
        cout << "i = " << i << endl;
        const int subSize = d1[i].size();
        if (subSize != d2[i].size()){
            cerr << "not the same rows" << endl;
            return false;
        }
        for (int j = 0; j < subSize; ++j){

            cout << "("<< i << ","<< j <<")";
            flt diff = abs(d1.at(i).at(j) - d2.at(i).at(j));
            if(diff > _eps_){
                cerr << "--unsimilar values--" << diff << endl;
                return false;
            }
            cout << "--good--" << endl;
        }
    }
    return true;
}



TEST(Data, store_and_load){
    // init data to store
    const Array2D<flt> to_store = {
    {1.2, 1.3, 5.6, 7.5, 8.6},
    {1.2, 1.4, 5.6, 7.5, 8.1},
    {1.2, 1.3, 5.6, 7.9, 8.2}
    };
    // store data
    cout << "store data size: (" << to_store.size() << "," 
         << to_store[0].size() << ")" << endl;
    string filename = "test_data";
    cout << "storing data ..." << endl;
    data::store_data(to_store, filename);
    cout << "storing is successfull" << endl;
    // load data
    Array2D<flt> to_load = data::load_data(filename);
    cout << "load data size: (" << to_load.size() << "," 
         << to_load[0].size() << ")" << endl;
    // compare data
    EXPECT_TRUE(to_load == to_store);

}