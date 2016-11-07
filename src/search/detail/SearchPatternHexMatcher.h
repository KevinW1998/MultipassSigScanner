#ifndef SEARCHPATTERNHEXMATCHER_HHHHH
#define SEARCHPATTERNHEXMATCHER_HHHHH

#include <gsl/string_span>
#include <vector>
#include <string>
#include <algorithm>

#include "SearchPatternBase.h"

namespace MPSig {
    namespace detail {
        class SearchPatternHexMatcher final : public SearchPatternBase {
        private:
            std::vector<std::pair<char, bool>> m_compiledPattern;
        public:
            SearchPatternHexMatcher(gsl::cstring_span<-1> hexPattern);
            
            virtual SearchPatternBase::ExecFirstResult<gsl_span_cit_t> ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const override;
            virtual SearchPatternBase::ExecFirstResult<gsl_span_crit_t> ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const override;
            
            virtual SearchPatternBase::ExecFirstResult<gsl_span_cit_t> ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const override;
            virtual SearchPatternBase::ExecFirstResult<gsl_span_crit_t> ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const override;

            virtual std::unique_ptr<SearchPatternBase> Clone() const override;
        };

    }
}

#endif
