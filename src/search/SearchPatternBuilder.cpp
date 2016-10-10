#include "SearchPatternBuilder.h"

#include "detail/SearchPatternHexMatcher.h"

MPSig::SearchPatternBuilder::SearchPatternBuilder()
{}

MPSig::SearchPatternBuilder& MPSig::SearchPatternBuilder::hasHexPattern(gsl::cstring_span<-1> pattern)
{
    m_patternSteps.push_back(std::unique_ptr<MPSig::detail::SearchPatternBase>(new MPSig::detail::SearchPatternHexMatcher(pattern)));
    return *this;
}

MPSig::SearchPattern MPSig::SearchPatternBuilder::compile()
{
    return SearchPattern(std::move(m_patternSteps));
}

