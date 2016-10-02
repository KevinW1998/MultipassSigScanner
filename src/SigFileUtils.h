#ifndef SIGFILEUTILS_HHHHH
#define SIGFILEUTILS_HHHHH

#include <vector>
#include <string>

namespace MPSig {
    namespace Utils {
        std::vector<char> ReadFileBinary(const std::string& fileName);
    }
}

#endif
