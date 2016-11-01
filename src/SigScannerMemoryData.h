#ifndef SIGSCANNERMEMORYDATA_HHHHH
#define SIGSCANNERMEMORYDATA_HHHHH

#include <vector>
#include <gsl/gsl>

namespace MPSig {
    class SigScannerMemoryData {
        gsl::span<const char> m_refData;
        std::ptrdiff_t m_offset;
    public:
        explicit SigScannerMemoryData(gsl::span<const char> data, std::ptrdiff_t offset = 0);

        bool IsInRange(std::intptr_t addr) const;
        char* Deref(std::intptr_t addrWithOffset) const;
        std::pair<char*, bool> DerefTry(std::intptr_t addrWithOffset) const;

        gsl::span<const char> Get() const;
        
        std::ptrdiff_t GetOffset() const;
        void SetOffset(std::ptrdiff_t offset);
    };
}



#endif
