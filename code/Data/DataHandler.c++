
#include <fstream>
#include <Data/DataHandler.h++>

flt dat::read_f64(char *in)
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

int dat::read_int(char *in)
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

bool dat::make_folder(string foldername)
{
    string command = "mkdir -p " + foldername;
    int i = std::system(command.c_str());
    return i;
}

bool dat::store_data(const Array<Array<flt>> &data,
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

Array<Array<flt>> dat::load_data(string const& filename){
    Array<Array<flt>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        cerr <<"Could not open the file - '"<< filename << "'"<< endl;
        // Return an empty vector if the file can't be opened
        return data;
    }
    //
    std::string line;
    while (getline(file, line)) {
        Array<flt> row;
        std::stringstream lineStream(line);
        string cell;

        while (getline(lineStream, cell, ',')) {
            try {
                // Convert string to double and add to row
                row.push_back(stod(cell)); 
            } catch (const std::invalid_argument& e) {
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