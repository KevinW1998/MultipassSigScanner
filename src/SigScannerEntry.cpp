#include "SigScannerEntry.h"

#include <cctype>

MPSig::SigScannerEntry::SigScannerEntry(const std::initializer_list<std::string>& patterns) : SigScannerEntry({patterns.begin(), patterns.end()})
{}

MPSig::SigScannerEntry::SigScannerEntry(const std::vector<std::string>& patterns)
{
    // Compile the pattern
    for (const std::string& pattern : patterns) {

        std::vector<std::pair<char, bool>> compiledPattern;
        // Convert the pattern to a vector of data.. 
        for (int i = 0; i < pattern.length(); ++i) {
            if (i == pattern.length() - 1) {
                if (!std::isspace(pattern[i]))
                    throw std::invalid_argument("Expected full 2-byte hex at end, got single hex!");
            }
                
            // "AC 42"
            // ~~~^~~~
            if (std::isspace(pattern[i])) 
                continue;

            // "AC 4 29"
            // ~~~~^^~~~
            if (!std::isspace(pattern[i]) && std::isspace(pattern[i + 1]))
                throw std::invalid_argument("Invalid hex, require two-byte hex!");

            if (std::memcmp(&pattern[i], "??", 2u) == 0) {
                compiledPattern.push_back(std::make_pair(00, false));
            } else {
                char hexBuf[] = { pattern[i], pattern[i + 1] };
                compiledPattern.push_back(std::make_pair(static_cast<char>(std::stoul(hexBuf, nullptr, 16)), true));
            }

            // Here we step one further
            ++i;
        }

        m_compiledPatterns.push_back(std::move(compiledPattern));
    }
    
}




const std::vector<std::vector<std::pair<char, bool>>>& MPSig::SigScannerEntry::GetCompiledPatterns() const
{
    return m_compiledPatterns;
}

