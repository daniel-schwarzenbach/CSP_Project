#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include <Data/DataHandler.h++>

flt data::read_flt(char *in)
{
    flt valueF64 = 1;
    string valueStr = in;
    try
    {
        valueF64 = std::stod(valueStr);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid input: " << valueStr << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Input out of range: " << valueStr << std::endl;
    }
    return valueF64;
}

bool data::store_alo_data(const std::string& filename, 
                        const Array2D<flt>& data, 
                        flt const& T){
    std::ofstream outfile(filename);
    assert(data.size);

    if (!outfile)
    {
        cerr << ERROR << " couldnt open file:" << filename << endl;
        return false;
    }
    outfile << "header1"; outfile << '\n';
    outfile << "header2"; outfile << '\n';
    outfile << T << " M " << " Mz " << " E "; outfile << '\n';
    // Write data to the header
    uint size = data[0].size();
    for(uint i = 0; i< size; ++i){
        for (const auto& col : data) {
            outfile << ' ' << col[i];
            
        }
        outfile << '\n';
    }
    return true;
}

    bool data::append_alo_data(const std::string& filename, 
                        const Array2D<flt>& newColumns, 
                        flt const& T) {
    static constexpr uint headerSize = 2;
    assert(newColumns.size() == 4);
    std::ifstream fileIn(filename);
    std::ofstream fileOut(filename + "tmp");

    if (!fileIn.is_open() || !fileOut.is_open()) {
        std::cerr << "Failed to open the files." << std::endl;
        return false;
    }

    std::string line;
    size_t rowIndex = 0;

    while (getline(fileIn, line)) {
        fileOut << line;
        if (rowIndex == headerSize){
            fileOut << T << " M " << " Mz " << " E ";
        }
        if(rowIndex > headerSize) // skip header
            for (const auto& col : newColumns) {
                if (rowIndex-headerSize-1 < col.size()) {
                    fileOut << ' ' << col[rowIndex-headerSize-1];
                } else {
                    // Handle the case where the new column is shorter than the existing data
                    fileOut << ' ';
                }
            }
        fileOut << '\n';
        ++rowIndex;
    }

    fileIn.close();
    fileOut.close();

    // Replace the original file with the temporary file
    std::remove(filename.c_str());
    std::rename((filename + "tmp").c_str(), filename.c_str());
    return true;
}

int data::read_int(char *in)
{
    int valueInt = 1;
    string valueStr = in;
    try
    {
        valueInt = static_cast<int>(std::stod(valueStr));
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid input: " << valueStr << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Input out of range: " << valueStr << std::endl;
    }
    return valueInt;
}

bool data::make_folder(string foldername)
{
    string command = "mkdir -p " + foldername;
    int i = std::system(command.c_str());
    return i;
}

bool data::store_data(const Array<Array<flt>> &data,
                      const std::string &filename)
{
    std::ofstream outfile(filename);

    if (!outfile)
    {
        cerr << ERROR << " couldnt open file:" << filename << endl;
        return false;
    }

    // Write data to the file
    for (const auto &row : data)
    {
        for (const flt value : row)
        {
            outfile << value << ' ';
        }
        outfile << '\n';
    }

    outfile.close();
    cout << "Data successfully stored in " << filename << endl;
    return true;
}

template <uint I>
bool data::store_data(const StaticArray<Array<flt>, I> &data,
                      const string &filename)
{
    std::ofstream outfile(filename);

    if (!outfile)
    {
        cerr << ERROR << " couldnt open file:" << filename << endl;
        return false;
    }
    // Write data to the file
    for (uint i = 0; i < I; ++i)
    {
        for (const flt value : data[i])
        {
            outfile << value << ' ';
        }
        outfile << '\n';
    }
    outfile.close();
    cout << "Data successfully stored in " << filename << endl;
    return true;
}

template bool data::store_data<2>(
    const StaticArray<Array<flt>, 2> &,
    const string &);
template bool data::store_data<3>(
    const StaticArray<Array<flt>, 3> &,
    const string &);
template bool data::store_data<4>(
    const StaticArray<Array<flt>, 4> &,
    const string &);
template bool data::store_data<5>(
    const StaticArray<Array<flt>, 5> &,
    const string &);

Array<Array<flt>> data::load_data(string const &filename)
{
    Array<Array<flt>> data;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        // Return an empty vector if the file can't be opened
        return data;
    }
    //
    std::string line;
    while (getline(file, line))
    {
        Array<flt> row;
        std::stringstream lineStream(line);
        string cell;

        while (getline(lineStream, cell, ','))
        {
            try
            {
                // Convert string to double and add to row
                row.push_back(stod(cell));
            }
            catch (const std::invalid_argument &e)
            {
                cerr << "Could not convert the string to double: "
                     << cell << std::endl;
                // Handle the conversion error or add a default value
            }
        }
        data.push_back(row);
    }

    file.close();
    return data;
}