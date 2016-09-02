#ifndef SIGSCANNERFILEOPENEXCEPTION_HHHHH
#define SIGSCANNERFILEOPENEXCEPTION_HHHHH

#include <stdexcept>

namespace MPSig {
    class SigScannerFileOpenException : public std::runtime_error {
    public:
        SigScannerFileOpenException(const std::string& fileName);
    };
}

#endif