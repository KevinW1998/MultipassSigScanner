#ifndef SIGSCANNERMEMORYDATA_HHHHH
#define SIGSCANNERMEMORYDATA_HHHHH

#include <vector>
#include <gsl/span>
#include <string>

namespace MPSig {
    class SigScannerMemoryData {
        gsl::span<const char> m_refData;
    public:
        explicit SigScannerMemoryData(gsl::span<const char> data);

        bool IsInRange(std::intptr_t addr) const;
        char* Deref(std::intptr_t addrWithOffset) const;
        std::pair<char*, bool> DerefTry(std::intptr_t addrWithOffset) const;

        gsl::span<const char> Get() const;
        std::intptr_t GetOffset() const;
    };
}



#endif
