
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

flt dat::read_int(char *in)
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