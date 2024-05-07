#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include <Data/DataHandler.h++>
#include <thread>

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

bool data::append_line_in_file(string const &filename,
                               string const &data, u64 const &lineNum)
{
    std::ifstream fileIn(filename);
    Array<string> lines(0);
    string line;
    int currentLine = 0;

    if (!fileIn.is_open())
    {
        std::cerr << "Error opening file to read." << std::endl;
        return false;
    }

    // Read all lines into a vector
    while (std::getline(fileIn, line))
    {
        lines.push_back(line);
        currentLine++;
    }
    fileIn.close();
    // Check if the specified line number is within the file size
    if (lineNum >= currentLine)
    {
        std::cerr << ERROR << "line number out of range." << std::endl;
        return false;
    }

    // Append the data to the specified line
    lines[lineNum] += data;

    // Write everything back to the file
    std::this_thread::sleep_for(
        std::chrono::microseconds(1));
    std::ofstream fileOut(filename);
    if (!fileOut.is_open())
    {
        std::cerr << "Error opening file to write." << std::endl;
        return false;
    }
    for (const auto &outputLine : lines)
    {
        fileOut << outputLine << "\n";
    }

    fileOut.close();
    return true;
}

bool data::append_lines_in_file(string const &filename,
                                Array<string> const &datas, Array<u64> const &lineNums)
{
    std::ifstream fileIn(filename);
    Array<string> lines(0);
    string line;
    int currentLine = 0;

    if (!fileIn.is_open())
    {
        std::cerr << "Error opening file to read" << std::endl;
        return false;
    }

    // Read all lines into a vector
    while (std::getline(fileIn, line))
    {
        lines.push_back(line);
        currentLine++;
    }
    fileIn.close();
    u64 dataSize = datas.size();
    if (dataSize != lineNums.size())
    {
        cerr << ERROR << "can not append files! size missmatch" << endl;
        cerr << "datas.size() =" << dataSize << endl;
        cerr << "lines.size() =" << lineNums.size() << endl;
        return false;
    }
    for (u64 i = 0; i < dataSize; ++i)
    {
        // Check if the specified line number is within the file size
        if (lineNums[i] >= currentLine)
        {
            std::cerr << ERROR << "line number out of range." << std::endl;
            return false;
        }
        // Append the data to the specified line
        lines[lineNums[i]] += datas[i];
    }
    std::this_thread::sleep_for(
        std::chrono::microseconds(1));
    // Write everything back to the file
    std::ofstream fileOut(filename);
    if (!fileOut.is_open())
    {
        std::cerr << "Error opening file to write" << std::endl;
        return false;
    }
    for (const auto &outputLine : lines)
    {
        fileOut << outputLine << "\n";
    }

    fileOut.close();
    return true;
}

bool data::store_alo_data(const string &filename,
                          string const &algoname,
                          const Array2D<flt> &data,
                          flt const &T, flt const &J,
                          Spin const &h, Spin const &k,
                          u64 const &Ns, u64 const &Nmax,
                          Index const &L)
{
    // check data size!!!
    if (data.size() != 4)
    {
        cerr << ERROR << " wrong data size:" << filename << endl;
        return false;
    }
    u64 n_data = data[0].size();
    for (Array<flt> d : data)
    {
        if (d.size() != n_data)
        {
            cerr << ERROR << " wrong data size:" << filename << endl;
            return false;
        }
    }
    // openfile
    std::ofstream outfile(filename);
    if (!outfile)
    {
        cerr << ERROR << " couldnt open file:" << filename << endl;
        return "";
    }
    outfile << filename << endl
            << algoname << endl
            << "L = " << L[0] << "," << L[1] << "," << L[2] << endl
            << "h = " << h(0) << "," << h(1) << "," << h(2) << endl
            << "k = " << k(0) << "," << k(1) << "," << k(2) << endl
            << "Ns = " << Ns << endl
            << "Nmax = " << Nmax << endl
            << "No_of_datapoints = " << n_data << endl
            << T << endl
            << "====" << endl
            << T << " M "
            << " Mz "
            << " E " << endl;
    // Write data to the header
    for (uint i = 0; i < n_data; ++i)
    {
        for (const auto &col : data)
        {
            outfile << col[i] << " ";
        }
        outfile << endl;
    }

    outfile.close();
    return true;
}

bool appent_algo_lines(string const &filename,
                       u64 const &dataSize, flt const &T)
{
    std::ifstream fileIn(filename);
    Array<string> lines(0);
    string line;
    int currentLine = 0;

    if (!fileIn.is_open())
    {
        std::cerr << "Error opening file to read" << std::endl;
        return false;
    }

    // Read all lines into a vector
    while (std::getline(fileIn, line))
    {
        lines.push_back(line);
        currentLine++;
    }
    fileIn.close();

    // Check if the specified line number is within the file size
    if (8 >= currentLine)
    {
        std::cerr << ERROR << "line number out of range." << std::endl;
        return false;
    }
    // Append the data to the specified line
    size_t pos = lines[7].find_last_of("= ");
    int old_size = std::stoi(lines[7].substr(pos + 1));
    lines[7] = "No_of_datapoints = " + to_string(old_size + dataSize);
    lines[8] += ", " + to_str(T);

    std::this_thread::sleep_for(
        std::chrono::microseconds(1));
    // Write everything back to the file
    std::ofstream fileOut(filename);
    if (!fileOut.is_open())
    {
        std::cerr << "Error opening file to write" << std::endl;
        return false;
    }
    for (const auto &outputLine : lines)
    {
        fileOut << outputLine << "\n";
    }

    fileOut.close();
    return true;
}

bool data::append_algo_data(const std::string &filename,
                            const Array2D<flt> &data,
                            flt const &T)
{

    // check data size!!!
    if (data.size() != 4)
    {
        cerr << ERROR << " wrong data size:" << filename << endl;
        return false;
    }
    u64 n_data = data[0].size();
    for (Array<flt> d : data)
    {
        if (d.size() != n_data)
        {
            cerr << ERROR << " wrong data size:" << filename << endl;
            return false;
        }
    }
    appent_algo_lines(filename, n_data, T);
    // Open the file in append mode
    std::ofstream file(filename, std::ios_base::app);

    // Check if the file was opened successfully
    if (!file.is_open())
    {
        cerr << ERROR << " opening file for appending." << std::endl;
        return false;
    }
    file << "====" << endl;
    file << to_str(T) << " M "
         << " Mz "
         << " E " << endl;
    // Append data
    for (uint i = 0; i < n_data; i++)
    {
        for (uint j = 0; j < 4; j++)
        {
            file << data[j][i] << " ";
        }
        file << endl;
    }
    // Close the file
    file.close();
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
    uint cols = data.size();
    u64 rows = data[0].size();
    for (Array<flt> d : data)
    {
        if (d.size() != rows)
        {
            cerr << ERROR << " wrong data size:" << filename << endl;
            return false;
        }
    }
    // openfile
    std::ofstream outfile(filename);
    if (!outfile)
    {
        cerr << ERROR << " couldnt open file to write to:" << filename << endl;
        return false;
    }
    for (u64 i = 0; i < rows; ++i)
    {
        for (u64 j = 0; j < cols; ++j)
        {
            outfile << data[j][i] << ' ';
        }
        outfile << endl;
    }
    outfile.close();
    return true;
}

template <uint I>
bool data::store_data(const StaticArray<Array<flt>, I> &data,
                      const string &filename)
{
    // openfile
    std::ofstream outfile(filename);
    if (!outfile)
    {
        cerr << ERROR << " couldnt open file to write to:" << filename << endl;
        return "";
    }
    for (u64 i = 0; i < I; ++i)
    {
        for (const flt value : data[i])
        {
            outfile << value << ' ';
        }
        outfile << '\n';
    }
    outfile.close();
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

Array2D<flt> data::load_data(string const &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        cerr << ERROR << " couldnt open file to loat:"
             << filename << endl;
        return {{}};
    }
    Array2D<flt> columns;
    string line;

    // Read the first line to determine the number of columns
    if (std::getline(file, line))
    {
        std::istringstream iss(line);
        double value;
        while (iss >> value)
        {
            columns.push_back(std::vector<double>{value});
        }
    }

    // Read the rest of the lines
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        double value;
        int colIndex = 0;
        while (iss >> value)
        {
            if (colIndex < columns.size())
            {
                columns[colIndex].push_back(value);
                colIndex++;
            }
        }
    }

    file.close();
    return columns;
}
std::ofstream &operator<<(std::ofstream &of, Spin const &v)
{
    of << "{" << v[0] << ", " << v[1] << ", " << v[2] << "}";
    return of;
}

bool data::store_lattice(Lattice3D<Spin> const &lattice,
                         string const &filename)
{
    // openfile
    std::ofstream outfile(filename);
    if (!outfile)
    {
        cerr << ERROR << " couldnt open file to write to:"
             << filename << endl;
        return false;
    }
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();

    for (uint x = 0; x < Lx; ++x)
    {
        outfile << "{" << endl;
        for (uint y = 0; y < Ly; ++y)
        {
            outfile << "{" << endl;
            for (uint z = 0; z < Lz; ++z)
            {
                outfile << lattice(x, y, z) << endl;
            }
            outfile << "} " << endl;
        }
        outfile << "}" << endl;
    }
    outfile.close();
    return true;
}

bool data::load_lattice(Lattice3D<Spin> &lattice, string const &filename)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        std::cerr << "ERROR: Couldn't open file to read from: " << filename << std::endl;
        return false;
    }
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();

    std::string line;
    f32 spinX, spinY, spinZ;
    char brace;

    for (uint x = 0; x < Lx; ++x)
    {
        std::getline(infile, line); // Skip {
        for (uint y = 0; y < Ly; ++y)
        {
            std::getline(infile, line); // Skip {
            for (uint z = 0; z < Lz; ++z)
            {
                std::getline(infile, line);
                std::istringstream iss(line);
                iss >> brace >> spinX >> brace >> spinY >> brace >> spinZ >> brace;
                lattice(x, y, z) = Spin{spinX, spinY, spinZ};
            }
            std::getline(infile, line); // Skip }
        }
        std::getline(infile, line); // Skip }
    }

    infile.close();
    return true;
}
