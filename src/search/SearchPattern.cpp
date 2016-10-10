#include "SearchPattern.h"

MPSig::SearchPattern::SearchPattern(std::vector<std::unique_ptr<MPSig::detail::SearchPatternBase>>&& patternsToMove) : 
    m_patternSteps(std::move(patternsToMove))
{}

template<typename It>
std::pair<std::ptrdiff_t, bool> MPSig::SearchPattern::ExecImpl(It begin, It end) const
{
    auto current = begin;
    
    for (const auto& m_patternStep : m_patternSteps) {
        current = m_patternStep->Exec(current, end);
        if (current == end)
            return{ std::distance(begin, end), false };
    }

    return{ std::distance(begin, current), true };
}

std::pair<std::ptrdiff_t, bool> MPSig::SearchPattern::Exec(gsl::span<const char> data) const 
{
    return ExecImpl(data.cbegin(), data.cend());
}

std::pair<std::ptrdiff_t, bool> MPSig::SearchPattern::ReverseExec(gsl::span<const char> data) const
{
    auto ret = ExecImpl(data.crbegin(), data.crend());
    if (ret.second)
        ret.first = data.size() - ret.first; // ret.first is the distance between current and end, but we want the distance between begin and end
    return ret;
}
