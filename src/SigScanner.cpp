#include "SigScanner.h"
#include "SigFileUtils.h"

#include <algorithm>

MPSig::SigScanner::SigScanner(const std::string& fName) : SigScanner(Utils::ReadFileBinary(fName))
{}

MPSig::SigScanner::SigScanner(const std::vector<char>& data) : 
    m_data(data),
    m_offset(0u)
{}


std::intptr_t MPSig::SigScanner::GetOffset() const
{
    return m_offset;
}

std::vector<char> MPSig::SigScanner::GetFunctionStartSig() const
{
    return m_functionStartSig;
}


void MPSig::SigScanner::SetOffset(std::intptr_t val)
{
    m_offset = val;
}

void MPSig::SigScanner::SetFunctionStartSig(const std::vector<char>& val)
{
    m_functionStartSig = val;
}

std::pair<std::intptr_t, std::intptr_t> MPSig::SigScanner::Scan(const SigScannerEntry& entry)
{
    const std::vector<std::vector<std::pair<char, bool>>>& compiledPatterns = entry.GetCompiledPatterns();

    for (const auto& nextPattern : compiledPatterns) {
        while(true) {
            auto ret = std::search(m_data.cbegin(), m_data.cend(), nextPattern.cbegin(), nextPattern.cend(),
                [&](char curr, std::pair<char, bool> currPattern)
            {
                return (!currPattern.second) || curr == currPattern.first;
            });
            
            if (ret != m_data.cend()) {
                auto addr = std::distance(m_data.cbegin(), ret) + m_offset;

                // Now we try to find the function start signature
                // We can use reverse iterator to find it, because it is likely above the found address
                auto reverseRetIt = std::make_reverse_iterator(ret);
                auto funcRet = std::search(reverseRetIt, m_data.crend(), m_functionStartSig.crbegin(), m_functionStartSig.crend());
                
                std::intptr_t funcAddr = 0;
                if (funcRet != m_data.crend())
                    funcAddr = std::distance(m_data.cbegin(), funcRet.base() - m_functionStartSig.size()) + m_offset;

                return std::make_pair(addr, funcAddr);
            } else {
                break;
            }
        }
    }

    return std::make_pair(0, 0);
}
