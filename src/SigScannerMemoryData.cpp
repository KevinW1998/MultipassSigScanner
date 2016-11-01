#include "SigScannerMemoryData.h"

MPSig::SigScannerMemoryData::SigScannerMemoryData(gsl::span<const char> data, std::ptrdiff_t offset /*= 0*/) :
    m_refData(data),
    m_offset(offset)
{}

bool MPSig::SigScannerMemoryData::IsInRange(std::intptr_t addrWithOffset) const
{
    std::intptr_t realAddr = addrWithOffset - m_offset;
    return realAddr >= 0 && realAddr < m_refData.length();
}

char* MPSig::SigScannerMemoryData::Deref(std::intptr_t addrWithOffset) const
{
    auto result = DerefTry(addrWithOffset);
    
    if (!result.second)
        throw std::runtime_error(std::string("Deref-Addr is not in valid range, expected ") +
            std::to_string(m_offset) + "-" + std::to_string(m_offset + m_refData.length()) + ", got " + std::to_string(reinterpret_cast<std::intptr_t>(result.first)));

    return reinterpret_cast<char*>(result.first);
}

std::pair<char*, bool> MPSig::SigScannerMemoryData::DerefTry(std::intptr_t addrWithOffset) const
{
    if (!IsInRange(addrWithOffset))
        throw std::runtime_error(std::string("Addr is not in valid range, expected ") +
            std::to_string(m_offset) + "-" + std::to_string(m_offset + m_refData.length()) + ", got " + std::to_string(addrWithOffset));

    const std::intptr_t addrToDerefIntPtr = reinterpret_cast<const std::intptr_t>(&m_refData[addrWithOffset - m_offset]);
    if (!IsInRange(addrToDerefIntPtr))
        return{ nullptr, false };
    return { *reinterpret_cast<char**>(addrToDerefIntPtr), true };
}

gsl::span<const char> MPSig::SigScannerMemoryData::Get() const
{
    return m_refData;
}

std::ptrdiff_t MPSig::SigScannerMemoryData::GetOffset() const
{
    return m_offset;
}

void MPSig::SigScannerMemoryData::SetOffset(std::ptrdiff_t offset)
{
    m_offset = offset;
}


