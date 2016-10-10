#include "SigScanner.h"
#include "SigFileUtils.h"

#include <algorithm>
#include <stdexcept>
#include <type_traits>

// Checks with a list of patterns for the relevant address
template<typename DataIt, typename SearchFuncType>
auto FindSignatureByIterator(
    DataIt dataItBegin,
    DataIt dataItEnd,
    const std::vector<std::vector<std::pair<char, bool>>>& compiledPatterns, 
    std::size_t skipCount,
    SearchFuncType searchFunc
) 
{  
    for (const auto& nextPattern : compiledPatterns) {
        std::size_t curSkipCount = skipCount;
        DataIt curIt = dataItBegin;
        while (true) {
            DataIt resultIt = searchFunc(curIt, dataItEnd, nextPattern);
            if (resultIt != dataItEnd) {
                if(curSkipCount > 0) {
                    curIt = resultIt + 1;
                    curSkipCount--;
                } else {
                    return resultIt;
                }
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

void MPSig::SigScanner::SetFunctionStartPattern(SearchPattern&& val)
{
    Expects(val.hasPatterns());

    m_functionStartPattern = std::move(val);
}

std::pair<std::intptr_t, std::intptr_t> MPSig::SigScanner::Scan(const SigScannerEntry& entry, std::size_t skipCount, intptr_t start)
{
    // Check if start offset is correct
    if(start < 0 || start >= static_cast<int>(m_data.size()))
        throw std::runtime_error(std::string("Invalid start offset, must be between 0 and ") + std::to_string(m_data.size()) + ", but is " + std::to_string(start));

    // TODO: std::pair<char, bool> to own class
    const auto& compiledPatterns = entry.GetCompiledPatterns();
    auto isMatchedPatternFunc =
        [](char curr, std::pair<char, bool> currPattern)
    {
        return (!currPattern.second) || curr == currPattern.first;
    };

    // Now find by iterator
    auto mainSigIt = FindSignatureByIterator(
                m_data.cbegin() + start, // begin + offset to end
                m_data.cend(),
                compiledPatterns, // All compiled patterns
                skipCount,  // Skip count
    [&isMatchedPatternFunc](auto itStart, auto itEnd, auto compiledPattern) { // Processor 
        return std::search(itStart, itEnd, compiledPattern.begin(), compiledPattern.end(), isMatchedPatternFunc);
    });

    std::intptr_t addr = 0;
    std::intptr_t funcAddr = 0;
    if (mainSigIt != m_data.cend()) {
        addr = std::distance(m_data.cbegin(), mainSigIt) + m_offset;

        const auto& compiledPatternsForFunctionStartSig = m_functionStartSig.GetCompiledPatterns();
        auto funcStartSigIt = FindSignatureByIterator(
                    std::make_reverse_iterator(mainSigIt),
                    m_data.crend(),
                    compiledPatternsForFunctionStartSig,
                    0u,
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

std::pair<std::intptr_t, std::intptr_t> MPSig::SigScanner::Scan(const SearchPattern& pattern) const
{
    auto dataView = gsl::span<const char>(m_data);
    auto result = pattern.Exec(dataView);

    std::ptrdiff_t offset = 0;
    std::ptrdiff_t functionOffset = 0;
    if (result.second) {
        offset = result.first;
        if (m_functionStartPattern.hasPatterns()) {
            auto dataViewToEnd = dataView.subspan(0, offset);
            result = m_functionStartPattern.ReverseExec(dataViewToEnd);
            if (result.second) {
                functionOffset = result.first + m_offset; // plus add offset to th address
            }
        }

        // Now add the offset to the address
        offset += m_offset;
    }

    return{ offset, functionOffset };
}

std::pair<std::intptr_t, std::intptr_t> MPSig::SigScanner::Scan(std::initializer_list<std::reference_wrapper<SearchPattern>> patterns) const
{
    for (const SearchPattern& pattern : patterns) {
        auto ret = Scan(pattern);
        if (ret.first != 0)
            return ret;
    }
    return{ 0, 0 };
}


