#include "SearchPatternHexMatcher.h"

#include <string>
#include <cctype>

MPSig::detail::SearchPatternHexMatcher::SearchPatternHexMatcher(gsl::cstring_span<-1> hexPattern)
{
    for (int i = 0; i < hexPattern.length(); ++i) {
        if (i == hexPattern.length() - 1) {
            if (!std::isspace(hexPattern[i]))
                throw std::invalid_argument("Expected full 2-byte hex at end, got single hex!");
        }

        // "AC 42"
        // ~~~^~~~
        if (std::isspace(hexPattern[i]))
            continue;

        // "AC 4 29"
        // ~~~~^^~~~
        if (!std::isspace(hexPattern[i]) && std::isspace(hexPattern[i + 1]))
            throw std::invalid_argument("Invalid hex, require two-byte hex!");

        if (std::memcmp(&hexPattern[i], "??", 2u) == 0) {
            m_compiledPattern.push_back(std::make_pair(00, false));
        }
        else {
            char hexBuf[] = { hexPattern[i], hexPattern[i + 1] };
            m_compiledPattern.push_back(std::make_pair(static_cast<char>(std::stoul(hexBuf, nullptr, 16)), true));
        }

        // Here we step one further
        ++i;
    }
}

template<typename ItData, typename ItPattern>
ItData ExecByIt(ItData begin, ItData end, ItPattern patternBegin, ItPattern patternEnd)
{
    bool is0x90 = *begin == '\x90';
    
    // Search for the pattern..
    return std::search(begin, end, patternBegin, patternEnd,
        [&](const unsigned char& curr, const std::pair<unsigned char, bool>& currPattern)
    {
        return (!currPattern.second) || curr == currPattern.first;
    });
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_cit_t> 
    MPSig::detail::SearchPatternHexMatcher::ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const
{
    auto result = ExecByIt(begin, end, m_compiledPattern.cbegin(), m_compiledPattern.cend());
    if (result == end)
        return{ false, end, end };
    return{ true, result, result + m_compiledPattern.size() };
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_crit_t> 
    MPSig::detail::SearchPatternHexMatcher::ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const
{
    auto result = ExecByIt(begin, end, m_compiledPattern.crbegin(), m_compiledPattern.crend()); // Add pattern size
    if (result == end)
        return{ false, end, end };

    return{ true, result, result + m_compiledPattern.size()};
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_cit_t> 
    MPSig::detail::SearchPatternHexMatcher::ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const
{
    if (std::distance(begin, end) < m_compiledPattern.size())
        return{ false, end, end };

    auto result = ExecByIt(begin, begin + m_compiledPattern.size(), m_compiledPattern.cbegin(), m_compiledPattern.cend());
    if (result != end)
        return{ true, result, result + m_compiledPattern.size() };
    
    return{ false, end, end };
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_crit_t>
    MPSig::detail::SearchPatternHexMatcher::ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const
{
    if (std::distance(begin, end) < m_compiledPattern.size())
        return{ false, end, end };

    auto result = ExecByIt(begin, begin + m_compiledPattern.size(), m_compiledPattern.cbegin(), m_compiledPattern.cend());
    if (result != end)
        return{ true, result, result + m_compiledPattern.size() };

    return{ false, end, end };
}

std::unique_ptr<MPSig::detail::SearchPatternBase> MPSig::detail::SearchPatternHexMatcher::Clone() const
{
    return std::unique_ptr<SearchPatternBase>(new SearchPatternHexMatcher(*this));
}

