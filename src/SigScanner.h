#ifndef SIGSCANNER_HHHHH
#define SIGSCANNER_HHHHH

#include <string>
#include <vector>

#include "SigScannerEntry.h"

namespace MPSig {
    class SigScanner {
        std::vector<char> m_data;
        std::intptr_t m_offset;
        SigScannerEntry m_functionStartSig;
    public:
        SigScanner(const std::string& fName);
        SigScanner(const std::vector<char>& data);

        // Getter
        std::intptr_t GetOffset() const;
        SigScannerEntry GetFunctionStartSig() const;
        
        // Setter
        void SetOffset(std::intptr_t val);
        void SetFunctionStartSig(const SigScannerEntry& val);
    
        // Functions

        // Return value #1, pattern found at address
        // Return value #2, found function with the function start signature
        std::pair<std::intptr_t, std::intptr_t> Scan(const SigScannerEntry& entry);
    };
}



#endif