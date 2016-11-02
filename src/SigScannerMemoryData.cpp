#include "SigScannerMemoryData.h"

MPSig::SigScannerMemoryData::SigScannerMemoryData(gsl::span<const char> data) :
    m_refData(data)
{}

bool MPSig::SigScannerMemoryData::IsInRange(std::intptr_t addrWithOffset) const
{
    const char* addrToTest = reinterpret_cast<const char*>(addrWithOffset);
    return m_refData.data() <= addrToTest && addrToTest <= m_refData.data() + m_refData.length();
}

char* MPSig::SigScannerMemoryData::Deref(std::intptr_t addrWithOffset) const
{
    auto result = DerefTry(addrWithOffset);
    
    if (!result.second)
        throw std::runtime_error(std::string("Address to BSTR is not in valid range, expected ") +
            std::to_string(reinterpret_cast<std::intptr_t>(m_refData.data())) + "-" + 
            std::to_string(reinterpret_cast<std::intptr_t>(m_refData.data() + m_refData.length())) +
            ", got " + std::to_string(reinterpret_cast<std::intptr_t>(result.first)));

    return reinterpret_cast<char*>(result.first);
}

std::pair<char*, bool> MPSig::SigScannerMemoryData::DerefTry(std::intptr_t addrWithOffset) const
{
    if (!IsInRange(addrWithOffset))
        throw std::runtime_error(std::string("Input-Address is not in valid range, expected ") +
            std::to_string(reinterpret_cast<std::intptr_t>(m_refData.data())) + "-" +
            std::to_string(reinterpret_cast<std::intptr_t>(m_refData.data() + m_refData.length())) + 
            ", got " + std::to_string(addrWithOffset));

    char* result = *reinterpret_cast<char**>(addrWithOffset);
    return { result , IsInRange(reinterpret_cast<std::intptr_t>(result)) };
}

gsl::span<const char> MPSig::SigScannerMemoryData::Get() const
{
    return m_refData;
}

std::intptr_t MPSig::SigScannerMemoryData::GetOffset() const
{
    return reinterpret_cast<std::intptr_t>(m_refData.data());
}

