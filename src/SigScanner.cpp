#include "SigScanner.h"
#include "SigFileUtils.h"

#include <algorithm>

template<typename DataIt, typename SearchFuncType>
auto FindSignatureByIterator(
    DataIt dataItBegin,
    DataIt dataItEnd,
    const std::vector<std::vector<std::pair<char, bool>>>& compiledPatterns, 
    SearchFuncType searchFunc
) 
{  
    for (const auto& nextPattern : compiledPatterns) {
        while (true) {
            auto resultIt = searchFunc(dataItBegin, dataItEnd, nextPattern);
            if (resultIt != dataItEnd) {
                return resultIt;
            } else {
                break;
            }
        }
    }
    return dataItEnd;
}




MPSig::SigScanner::SigScanner(const std::string& fName) : SigScanner(Utils::ReadFileBinary(fName))
{}

MPSig::SigScanner::SigScanner(const std::vector<char>& data) : 
    m_data(data),
    m_offset(0u),
    m_functionStartSig({})
{}


std::intptr_t MPSig::SigScanner::GetOffset() const
{
    return m_offset;
}

MPSig::SigScannerEntry MPSig::SigScanner::GetFunctionStartSig() const
{
    return m_functionStartSig;
}


void MPSig::SigScanner::SetOffset(std::intptr_t val)
{
    m_offset = val;
}

void MPSig::SigScanner::SetFunctionStartSig(const MPSig::SigScannerEntry& val)
{
    m_functionStartSig = val;
}

std::pair<std::intptr_t, std::intptr_t> MPSig::SigScanner::Scan(const SigScannerEntry& entry)
{
    // TODO: std::pair<char, bool> to own class
    const auto& compiledPatterns = entry.GetCompiledPatterns();
    auto isMatchedPatternFunc =
        [](char curr, std::pair<char, bool> currPattern)
    {
        return (!currPattern.second) || curr == currPattern.first;
    };

    auto mainSigIt = FindSignatureByIterator(m_data.cbegin(), m_data.cend(), compiledPatterns, 
        [&isMatchedPatternFunc](auto itStart, auto itEnd, auto compiledPattern) {
        return std::search(itStart, itEnd, compiledPattern.begin(), compiledPattern.end(), isMatchedPatternFunc);
    });
    std::intptr_t addr = 0;
    std::intptr_t funcAddr = 0;
    if (mainSigIt != m_data.cend()) {
        addr = std::distance(m_data.cbegin(), mainSigIt) + m_offset;

        const auto& compiledPatternsForFunctionStartSig = m_functionStartSig.GetCompiledPatterns();
        auto funcStartSigIt = FindSignatureByIterator(std::make_reverse_iterator(mainSigIt), m_data.crend(), compiledPatternsForFunctionStartSig, 
            [&isMatchedPatternFunc](auto itStart, auto itEnd, auto compiledPattern) {
            auto searchResultIt = std::search(itStart, itEnd, compiledPattern.crbegin(), compiledPattern.crend(), isMatchedPatternFunc);
            if (searchResultIt != itEnd)
                return searchResultIt + compiledPattern.size();
            return searchResultIt;
        });

        if (funcStartSigIt != m_data.crend()) {
            funcAddr = std::distance(m_data.cbegin(), funcStartSigIt.base()) + m_offset;
        }
    }

    return std::make_pair(addr, funcAddr);
}
