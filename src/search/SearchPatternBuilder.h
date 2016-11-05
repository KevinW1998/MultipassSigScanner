#ifndef SEARCHPATTERNBUILDER_HHHHH
#define SEARCHPATTERNBUILDER_HHHHH

#include "detail/SearchPatternBase.h"
#include "SearchPattern.h"

#include <vector>
#include <string>
#include <memory>

#include <gsl/string_span>

namespace MPSig {

    class SearchPatternBuilder {
        std::vector<std::unique_ptr<detail::SearchPatternBase>> m_patternSteps;
    public:
        SearchPatternBuilder();

        SearchPatternBuilder& HasHexPattern(gsl::cstring_span<-1> pattern);
        SearchPatternBuilder& ReferencesBSTR(gsl::cwstring_span<-1> bstrText);

        SearchPattern Compile();
    };
}

#endif
