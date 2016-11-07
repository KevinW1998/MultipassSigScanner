#ifndef SEARCHPATTERNREFBSTR_HHHHH
#define SEARCHPATTERNREFBSTR_HHHHH


#include <gsl/string_span>
#include <vector>
#include <string>

#include "SearchPatternBase.h"

namespace MPSig {
    namespace detail {
        class SearchPatternRefBSTR final : public SearchPatternBase {
        private:
            std::wstring m_textToCompare;
        
            template<typename It> 
            bool ExecRefBSTRByIt(const MPSig::SigScannerMemoryData& data, It begin, It end) const;
        public:
            SearchPatternRefBSTR(gsl::cwstring_span<-1> textToCompare);

            virtual SearchPatternBase::ExecFirstResult<gsl_span_cit_t> ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const override;
            virtual SearchPatternBase::ExecFirstResult<gsl_span_crit_t> ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const override;
            
            virtual SearchPatternBase::ExecFirstResult<gsl_span_cit_t> ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const override;
            virtual SearchPatternBase::ExecFirstResult<gsl_span_crit_t> ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const override;

            
            // Geerbt über SearchPatternBase
            virtual std::unique_ptr<SearchPatternBase> Clone() const override;
        };
    }
}

#endif
