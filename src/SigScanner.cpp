#include "SigScanner.h"
#include "SigFileUtils.h"

#include <algorithm>
#include <stdexcept>
#include <type_traits>

MPSig::SigScanner::SigScanner(const SigScannerMemoryData& data) :
    m_data(data)
{}


void MPSig::SigScanner::SetFunctionStartPattern(SearchPattern&& val)
{
    Expects(val.hasPatterns());

    m_functionStartPattern = std::move(val);
}


std::pair<std::intptr_t, std::intptr_t> MPSig::SigScanner::Scan(const SearchPattern& pattern) const
{
    auto dataView = m_data.Get();
    auto result = pattern.Exec(m_data);

    std::ptrdiff_t offset = 0;
    std::ptrdiff_t functionOffset = 0;
    if (result.second) {
        offset = result.first;
        if (m_functionStartPattern.hasPatterns()) {
            auto dataViewToEnd = dataView.subspan(0, offset);
            result = m_functionStartPattern.ReverseExec(MPSig::SigScannerMemoryData(dataViewToEnd));
            if (result.second) {
                functionOffset = result.first + m_data.GetOffset(); // plus add offset to th address
            }
        }

        // Now add the offset to the address
        offset += m_data.GetOffset();
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


