#ifndef SEARCHPATTERNBUILDER_HHHHH
#define SEARCHPATTERNBUILDER_HHHHH

#include "detail/SearchPatternBase.h"
#include "SearchPattern.h"

#include <vector>
#include <string>
#include <memory>

namespace MPSig {

    class SearchPatternBuilder {
        std::vector<std::unique_ptr<detail::SearchPatternBase>> m_patternSteps;
    public:
        SearchPatternBuilder();

        SearchPatternBuilder& hasHexPattern(gsl::cstring_span<-1> pattern);

        SearchPattern compile();
    };
}

#endif
