#include "SigScannerFileOpenException.h"


MPSig::SigScannerFileOpenException::SigScannerFileOpenException(const std::string& fileName) : 
    std::runtime_error(std::string("Failed to open file ") + fileName)
{}
