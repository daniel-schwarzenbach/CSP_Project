#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

void appendMultipleColumns(const std::string& filename, const std::vector<std::vector<double>>& newColumns, double const& T) {
    static constexpr uint headerSize = 2;
    assert(newColumns.size() == 4);
    std::ifstream fileIn(filename);
    std::ofstream fileOut("temp.csv", std::ios::trunc);

    if (!fileIn.is_open() || !fileOut.is_open()) {
        std::cerr << "Failed to open the files." << std::endl;
        return;
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
    std::rename("temp.csv", filename.c_str());
}