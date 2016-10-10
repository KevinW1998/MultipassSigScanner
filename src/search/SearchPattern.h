#ifndef SEARCHPATTERN_HHHHH
#define SEARCHPATTERN_HHHHH

#include "detail/SearchPatternBase.h"

#include <vector>
#include <string>
#include <memory>
#include <utility>

namespace MPSig {

    class SearchPattern {
        friend class SearchPatternBuilder;
        friend class SigScanner;
        std::vector<std::unique_ptr<detail::SearchPatternBase>> m_patternSteps;

        template<typename It>
        std::pair<std::ptrdiff_t, bool> ExecImpl(It begin, It end) const;

    protected:
        SearchPattern() = default;
        SearchPattern(std::vector<std::unique_ptr<detail::SearchPatternBase>>&& patternsToMove);

        std::pair<std::ptrdiff_t, bool> Exec(gsl::span<const char> data) const;
        std::pair<std::ptrdiff_t, bool> ReverseExec(gsl::span<const char> data) const;

        bool hasPatterns() const { return m_patternSteps.size() > 0; }
    public:
        SearchPattern(const SearchPattern& other) = delete;
        SearchPattern(SearchPattern&& other) = default;

        SearchPattern& operator=(const SearchPattern& other) = delete;
        SearchPattern& operator=(SearchPattern&& other) = default;
    };

}

#endif
