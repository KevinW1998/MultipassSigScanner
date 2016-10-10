#ifndef SEARCHPATTERNBASE_HHHHH
#define SEARCHPATTERNBASE_HHHHH

#include <vector>
#include <string>

#include <gsl/gsl>

namespace MPSig {
    namespace detail {
        class SearchPatternBase {
        public:
            virtual ~SearchPatternBase() = default;

            using gsl_span_cit_t = typename gsl::span<const char>::const_iterator;
            using gsl_span_crit_t = typename gsl::span<const char>::const_reverse_iterator;

            virtual gsl_span_cit_t Exec(gsl_span_cit_t begin, gsl_span_cit_t end) const = 0;
            virtual gsl_span_crit_t Exec(gsl_span_crit_t begin, gsl_span_crit_t end) const = 0;
        };

    }    
}

#endif
