#include "SearchPattern.h"

MPSig::SearchPattern::SearchPattern(std::vector<std::unique_ptr<MPSig::detail::SearchPatternBase>>&& patternsToMove) : 
    m_patternSteps(std::move(patternsToMove))
{}

template<typename It, typename std::enable_if <
    std::is_same<It, typename MPSig::detail::SearchPatternBase::gsl_span_cit_t>::value ||
    std::is_same<It, typename MPSig::detail::SearchPatternBase::gsl_span_crit_t>::value, int>::type
>
std::pair<std::ptrdiff_t, bool> MPSig::SearchPattern::ExecImpl(const MPSig::SigScannerMemoryData& data, It begin, It end) const
{
    Expects(m_patternSteps.size() > 0);
    
    // Start
    auto possibleFoundLocation = begin;
    auto currentProgress = begin;
    


    // TODO: Check if iterator advances:
    // 1. Iterator advances --> Do next check, or return if no more patterns left
    // 2. Iterator is same --> Failed test, retry with pattern, advance iterator with +1
    // 3. Iterator is end --> Totally Failed!
    
    std::size_t i = 0;
    while (i < m_patternSteps.size()) {
        const auto& patternStep = m_patternSteps[i];

        if (i == 0) {
            // Result:
            // 1. New It
            // 2. End
            auto result = patternStep->ExecFirst(data, possibleFoundLocation, end);
            if (!result.success)
                return{ std::distance(begin, end), false };
            
            possibleFoundLocation = result.rangeBegin;
            currentProgress = result.rangeEnd;

            // Special case for reverse iterator
            if (std::is_same<It, typename MPSig::detail::SearchPatternBase::gsl_span_crit_t>::value) {
                // If it is our last element:
                if (i == m_patternSteps.size() - 1) {
                    possibleFoundLocation = result.rangeEnd;
                    // We will leave our loop anyway
                    break; // Just in case
                } 
            }
        } else {
            // Result:
            // 1. New It
            // 2. End
            auto result = patternStep->ExecDepend(data, currentProgress, end);
            if (!result.success) {
                possibleFoundLocation++;
                i = 0;
                continue;
            }
            currentProgress = result.rangeEnd;

            // Special case for reverse iterator
            if (std::is_same<It, typename MPSig::detail::SearchPatternBase::gsl_span_crit_t>::value) {
                if (i == m_patternSteps.size() - 1) {
                    possibleFoundLocation = result.rangeEnd;
                    // We will leave our loop anyway
                    break; // Just in case
                }
            } // For normal iterators we use the one at the beginning
            
        }

        i++;
    }

    return { std::distance(begin, possibleFoundLocation), true };
}

std::pair<std::ptrdiff_t, bool> MPSig::SearchPattern::Exec(const MPSig::SigScannerMemoryData& data) const
{
    auto dataView = data.Get();
    return ExecImpl(data, dataView.cbegin(), dataView.cend());
}

std::pair<std::ptrdiff_t, bool> MPSig::SearchPattern::ReverseExec(const MPSig::SigScannerMemoryData& data) const
{
    auto dataView = data.Get();
    auto ret = ExecImpl(data, dataView.crbegin(), dataView.crend());
    if (ret.second)
        ret.first = dataView.size() - ret.first; // ret.first is the distance between current and end, but we want the distance between begin and end
    return ret;
}
