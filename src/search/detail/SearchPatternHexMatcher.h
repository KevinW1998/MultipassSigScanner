#ifndef SEARCHPATTERNHEXMATCHER_HHHHH
#define SEARCHPATTERNHEXMATCHER_HHHHH

#include <gsl/gsl>
#include <vector>
#include <string>

#include "SearchPatternBase.h"

namespace MPSig {
    namespace detail {
        class SearchPatternHexMatcher final : public SearchPatternBase {
        private:
            std::vector<std::pair<char, bool>> m_compiledPattern;
        public:
            SearchPatternHexMatcher(gsl::cstring_span<-1> hexPattern);
            
            virtual gsl_span_cit_t Exec(gsl_span_cit_t begin, gsl_span_cit_t end) const override;
            virtual gsl_span_crit_t Exec(gsl_span_crit_t begin, gsl_span_crit_t end) const override;
        };

    }
}

#endif
