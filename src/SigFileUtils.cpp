#include "SigFileUtils.h"
#include "exceptions/SigScannerFileOpenException.h"

#include <fstream>

std::vector<char> MPSig::Utils::ReadFileBinary(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::in | std::ios::binary);
    if (!file.is_open())
        throw SigScannerFileOpenException(fileName);
    
    std::vector<char> data;
    if (!file.eof() && !file.fail())
    {
        file.seekg(0, std::ios_base::end);
        std::size_t fileSize = static_cast<std::size_t>(file.tellg().seekpos());
        data.resize(fileSize);

        file.seekg(0, std::ios_base::beg);
        file.read(&data[0], fileSize);
    }
    return data;

}
