#ifndef SIGSCANNERENTRY_HHHHH
#define SIGSCANNERENTRY_HHHHH

#include <string>
#include <vector>



namespace MPSig {
    class SigScannerEntry {
    friend class SigScanner;
        std::vector<std::vector<std::pair<char, bool>>> m_compiledPatterns;
    public:
        SigScannerEntry(const std::initializer_list<std::string>& patterns);
        explicit SigScannerEntry(const std::vector<std::string>& patterns);
    protected:
        const std::vector<std::vector<std::pair<char, bool>>>& GetCompiledPatterns() const;
    };
}



#endif