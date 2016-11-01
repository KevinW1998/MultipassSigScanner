#ifndef SIGSCANNER_HHHHH
#define SIGSCANNER_HHHHH

#include <string>
#include <vector>
#include <initializer_list>

#include "SigScannerMemoryData.h"
#include "search/SearchPattern.h"

// TODO: Remove offset, offset can be received with pointer

namespace MPSig {
    class SigScanner {
        SigScannerMemoryData m_data;
        SearchPattern m_functionStartPattern;
    public:
        explicit SigScanner(const SigScannerMemoryData& data);

        // Getter
        std::ptrdiff_t GetOffset() const;
        
        // Setter
        void SetOffset(std::ptrdiff_t val);
        void SetFunctionStartPattern(SearchPattern&& val);
        
        // Return value #1, pattern found at address
        // Return value #2, found function with the function start signature
        std::pair<std::intptr_t, std::intptr_t> Scan(const SearchPattern& pattern) const;
        std::pair<std::intptr_t, std::intptr_t> Scan(std::initializer_list<std::reference_wrapper<SearchPattern>> patterns) const;
    };
}



#endif
