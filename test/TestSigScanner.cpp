#include <catch.hpp>

#include <SigScanner.h>
#include <SigFileUtils.h>
#include <search/SearchPatternBuilder.h>

TEST_CASE("Tests the scanner with Pattern (new version)", "[sigscanner]") {
    auto readFile = MPSig::Utils::ReadFileBinary("test_data/example.bin");
    char* ptr = &readFile[0];
    gsl::span<const char> data(readFile);
    MPSig::SigScannerMemoryData emulatedMemData(data);

    MPSig::SigScanner scanner(emulatedMemData);
    std::intptr_t offset = reinterpret_cast<std::intptr_t>(ptr);

    scanner.SetFunctionStartPattern(MPSig::SearchPatternBuilder().HasHexPattern("55 8B EC").Compile());

    // Single start function signature, single normal signature
    MPSig::SearchPattern movEntry = 
        MPSig::SearchPatternBuilder()
            .HasHexPattern(gsl::ensure_z("68 ?? ?? ?? ??")).Compile();
    std::pair<std::intptr_t, std::intptr_t> scanResult = scanner.Scan(movEntry);
    REQUIRE(scanResult.first == 8u + offset);
    REQUIRE(scanResult.second == 2u + offset);

    // Single start function signature, multiple normal signature
    MPSig::SearchPattern nopEntry1 =
        MPSig::SearchPatternBuilder()
            .HasHexPattern(gsl::ensure_z("90 68")).Compile();
    MPSig::SearchPattern nopEntry2 =
        MPSig::SearchPatternBuilder()
            .HasHexPattern(gsl::ensure_z("90 C9")).Compile();
    std::pair<std::intptr_t, std::intptr_t> nopScanResult = scanner.Scan({ nopEntry1, nopEntry2 });
    REQUIRE(nopScanResult.first == 7u + offset);
    REQUIRE(nopScanResult.second == 2u + offset);

    // Function signature which does not exist in the example
    MPSig::SearchPattern notExistingEntry =
        MPSig::SearchPatternBuilder()
            .HasHexPattern(gsl::ensure_z("01 02 03 04")).Compile();
    std::pair<std::intptr_t, std::intptr_t> invalidResult = scanner.Scan(notExistingEntry);
    REQUIRE(invalidResult.first == 0);
    REQUIRE(invalidResult.second == 0);


}