#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include <Data/DataHandler.h++>
#include <thread>

// read a flt from an char*
flt data::read_flt(char *in)
{
    flt valueF64 = 1.0;
    string valueStr = in;
    try
    {   // try reading it
        valueF64 = std::stod(valueStr);
    }
    // catch errors
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
                               string const &data, 
                               u64 const &lineNum)
{
    // open file
    std::ifstream fileIn(filename);
    // init the lines array
    Array<string> lines(0);
    string line;
    int currentLine = 0;
    // check if we can open the file
    if (!fileIn.is_open())
    {
        std::cerr << "Error opening file to read." << std::endl;
        return false;
    }
    // Read all lines into an Array
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
                                Array<string> const &datas, 
                                Array<u64> const &lineNums)
{
    // open file
    std::ifstream fileIn(filename);
    // init Array for all lines
    Array<string> lines(0);
    string line;
    int currentLine = 0;
    // check if the file cold been opened
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
    // close the file
    fileIn.close();
    // check if the data fits
    u64 dataSize = datas.size();
    if (dataSize != lineNums.size())
    {
        cerr << ERROR << "can not append files! size missmatch" 
             << endl;
        cerr << "datas.size() =" << dataSize << endl;
        cerr << "lines.size() =" << lineNums.size() << endl;
        return false;
    }
    // add data to all requested lines
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
    // Write everything back to the file
    std::ofstream fileOut(filename);
    // check if we can open the file
    if (!fileOut.is_open())
    {
        std::cerr << "Error opening file to write" << std::endl;
        return false;
    }
    // rewrite the hole file
    for (const auto &outputLine : lines)
    {
        fileOut << outputLine << "\n";
    }
    // close the file
    fileOut.close();
    return true;
}

// read integer valuew from char*
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
    // get rows and cols
    uint cols = data.size();
    u64 rows = data[0].size();
    // check if the data has the correct size
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
    // chek if we opened the file correctly
    if (!outfile)
    {
        cerr << ERROR << " couldnt open file to write to:" << filename << endl;
        return false;
    }
    // write the data in the file
    for (u64 i = 0; i < rows; ++i)
    {
        for (u64 j = 0; j < cols; ++j)
        {
            outfile << data[j][i] << ' ';
        }
        outfile << endl;
    }
    // close the file
    outfile.close();
    return true;
}

Array2D<flt> data::load_data(string const &filename)
{
    // open file and check if we could
    std::ifstream file(filename);
    if (!file)
    {
        cerr << ERROR << " couldnt open file to loat:"
             << filename << endl;
        // empty data
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
    // close the file
    file.close();
    return columns;
}


// for ploting the lattice we need a 
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
    // check if it can be ofened
    if (!outfile)
    {
        cerr << ERROR << " couldnt open file to write to:"
             << filename << endl;
        return false;
    }
    // get dims
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    // write the entire lattice to the file
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
    // close the folder
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
    // get dims
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();

    std::string line;
    f32 spinX, spinY, spinZ;
    // srings to ignore
    char brace;
    // read entire spin
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
                iss >> brace >> spinX >> brace >> spinY >> brace
                    >> spinZ >> brace;
                lattice(x, y, z) = Spin{spinX, spinY, spinZ};
            }
            std::getline(infile, line); // Skip }
        }
        std::getline(infile, line); // Skip }
        
    }
    // close file
    infile.close();
    return true;
}
