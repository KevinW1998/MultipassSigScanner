#include <catch.hpp>
#include <SigScanner.h>
#include <SigFileUtils.h>
#include <search/SearchPatternBuilder.h>

TEST_CASE("Tests the SearchPatternRefBSTR", "[searchpatternrefbstr]") {
    auto readFile = MPSig::Utils::ReadFileBinary("test_data/example_ref_bstr.bin");
    char* ptr = &readFile[0];
    gsl::span<const char> data(readFile);
    MPSig::SigScannerMemoryData emulatedMemData(data);

    // Set at +22 the address to the string
    std::intptr_t ptrToBstr = reinterpret_cast<std::intptr_t>(ptr) + 4;
    std::intptr_t* ptrToModify = reinterpret_cast<std::intptr_t*>(ptr + 22);
    *ptrToModify = ptrToBstr;

    std::intptr_t offset = reinterpret_cast<std::intptr_t>(ptr);
    
    MPSig::SigScanner scanner(emulatedMemData);
    scanner.SetOffset(offset);
    scanner.SetFunctionStartPattern(MPSig::SearchPatternBuilder().HasHexPattern("55 8B EC").Compile());
    
    
    auto findTestPattern = MPSig::SearchPatternBuilder().HasHexPattern("90 90 90 68").ReferencesBSTR(L"Test").Compile();
    std::pair<std::intptr_t, std::intptr_t> scanResult = scanner.Scan(findTestPattern);

    REQUIRE(scanResult.first == 18u + offset);
    REQUIRE(scanResult.second == 15u + offset);
}