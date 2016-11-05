#ifndef SEARCHPATTERNBASE_HHHHH
#define SEARCHPATTERNBASE_HHHHH

#include "../../SigScannerMemoryData.h"

#include <vector>
#include <string>

#include <gsl/span>

namespace MPSig {
    namespace detail {
        class SearchPatternBase {
        public:
            virtual ~SearchPatternBase() = default;

            using gsl_span_cit_t = typename gsl::span<const char>::const_iterator;
            using gsl_span_crit_t = typename gsl::span<const char>::const_reverse_iterator;

            template<typename It>
            struct ExecFirstResult {
                bool success; // If it was a success
                It rangeBegin; // The first valid iterator 
                It rangeEnd; // The next depended iterator point (from which all other patterns depend on)
            };

            // ExecFirst can look up begin to end fully, without restrictions
            virtual ExecFirstResult<gsl_span_cit_t> ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const = 0;
            virtual ExecFirstResult<gsl_span_crit_t> ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const = 0;
            
            // ExecDepend must only check the current location as it depends on a search success
            virtual ExecFirstResult<gsl_span_cit_t> ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const = 0;
            virtual ExecFirstResult<gsl_span_crit_t> ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const = 0;
        };

    }    
}

#endif
