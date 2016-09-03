#ifndef SIGSCANNERJSONCONFIG_HHHHH
#define SIGSCANNERJSONCONFIG_HHHHH

#include <string>
#include "SigScanner.h"

namespace MPSig {
    class SigScannerJsonConfig {
        int m_offset;

    public:
        SigScannerJsonConfig(const std::string& fileName);
    };
}



#endif