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
    // Search for the pattern..
    return std::search(begin, end, patternBegin, patternEnd,
        [&](unsigned char curr, std::pair<unsigned char, bool> currPattern)
    {
        return (!currPattern.second) || curr == currPattern.first;
    });
}

typename MPSig::detail::SearchPatternBase::gsl_span_cit_t MPSig::detail::SearchPatternHexMatcher::Exec(gsl_span_cit_t begin, gsl_span_cit_t end) const
{
    return ExecByIt(begin, end, m_compiledPattern.cbegin(), m_compiledPattern.cend());
}

typename MPSig::detail::SearchPatternBase::gsl_span_crit_t MPSig::detail::SearchPatternHexMatcher::Exec(gsl_span_crit_t begin, gsl_span_crit_t end) const
{
    auto ret = ExecByIt(begin, end, m_compiledPattern.crbegin(), m_compiledPattern.crend()); // Add pattern size
    if (ret != end)
        ret += m_compiledPattern.size();
    
    return ret;
}

