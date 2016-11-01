#include "SearchPatternBuilder.h"

#include "detail/SearchPatternHexMatcher.h"
#include "detail/SearchPatternRefBSTR.h"

MPSig::SearchPatternBuilder::SearchPatternBuilder()
{}

MPSig::SearchPatternBuilder& MPSig::SearchPatternBuilder::HasHexPattern(gsl::cstring_span<-1> pattern)
{
    m_patternSteps.push_back(std::unique_ptr<MPSig::detail::SearchPatternBase>(new MPSig::detail::SearchPatternHexMatcher(pattern)));
    return *this;
}

MPSig::SearchPatternBuilder& MPSig::SearchPatternBuilder::ReferencesBSTR(gsl::cwstring_span<-1> bstrText)
{
    m_patternSteps.push_back(std::unique_ptr<MPSig::detail::SearchPatternBase>(new MPSig::detail::SearchPatternRefBSTR(bstrText)));
    return *this;
}

MPSig::SearchPattern MPSig::SearchPatternBuilder::Compile()
{
    return SearchPattern(std::move(m_patternSteps));
}

