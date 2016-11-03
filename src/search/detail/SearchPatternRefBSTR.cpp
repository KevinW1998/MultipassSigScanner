#include "SearchPatternRefBSTR.h"
#include "../../SigScannerMemoryData.h"

#include <string>
#include <cctype>

MPSig::detail::SearchPatternRefBSTR::SearchPatternRefBSTR(gsl::cwstring_span<-1> textToCompare) : 
    m_textToCompare(textToCompare.begin(), textToCompare.end())
{}

template<typename It>
bool MPSig::detail::SearchPatternRefBSTR::ExecRefBSTRByIt(const MPSig::SigScannerMemoryData& data, It begin, It end) const {
    constexpr int ptrSize = sizeof(void*);
    if (std::distance(begin, end) < ptrSize)
        return false;

    // 1. Get pointer
    char* ptr = const_cast<char*>(&begin[0]);
    
    // 2. Deref pointer
    auto derefResult = data.DerefTry(reinterpret_cast<std::intptr_t>(ptr));

    // 3. Test if valid
    if (!derefResult.second)
        return false;

    // Deref:
    std::uint32_t lenBytes = *reinterpret_cast<std::uint32_t*>(derefResult.first - 4);
    if(lenBytes > 400) // TODO: Better size check!
        return false;

    gsl::cwstring_span<-1> dataToBSTR(reinterpret_cast<wchar_t*>(derefResult.first), lenBytes / 2);
    while (dataToBSTR.size() >= 1) {
        wchar_t curChar = *(dataToBSTR.cend() - 1);
        if (curChar != L'\x00')
            break;
        else
            dataToBSTR = dataToBSTR.subspan(0, dataToBSTR.size() - 1);
    }

    if (dataToBSTR == m_textToCompare)
        return true;

    return false;
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_cit_t> 
    MPSig::detail::SearchPatternRefBSTR::ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const
{
    constexpr int ptrSize = sizeof(void*);
    
    auto current = begin;
    while (std::distance(current, end) >= 4) {
        bool success = ExecRefBSTRByIt(data, current, end);
        if (success)
            return{ true, current, current + ptrSize };
        current++;
    }

    return { false, end, end };
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_crit_t> 
    MPSig::detail::SearchPatternRefBSTR::ExecFirst(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const
{
    constexpr int ptrSize = sizeof(void*);

    auto current = begin;
    while (std::distance(current, end) >= 4) {
        bool success = ExecRefBSTRByIt(data, current, end);
        if (success)
            return{ true, current, current + ptrSize };
        current++;
    }
    
    return{ false, end, end };
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_cit_t>
    MPSig::detail::SearchPatternRefBSTR::ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_cit_t begin, gsl_span_cit_t end) const
{
    constexpr int ptrSize = sizeof(void*);

    bool success = ExecRefBSTRByIt(data, begin, end);
    if (success) {
        return{ true, begin, begin + ptrSize };
    }

    return{ false, end, end };
}

MPSig::detail::SearchPatternBase::ExecFirstResult<typename MPSig::detail::SearchPatternBase::gsl_span_crit_t>
    MPSig::detail::SearchPatternRefBSTR::ExecDepend(const MPSig::SigScannerMemoryData& data, gsl_span_crit_t begin, gsl_span_crit_t end) const
{
    constexpr int ptrSize = sizeof(void*);
    if (std::distance(begin, end) >= 4 && ExecRefBSTRByIt(data, begin + ptrSize, end))
        return{ true, begin, begin + ptrSize };

    return{ false, end, end };
}
