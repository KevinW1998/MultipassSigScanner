#include <catch.hpp>

#include <SigScanner.h>
#include <search/SearchPatternBuilder.h>

TEST_CASE("Tests the scanner", "[sigscanner]") {
    MPSig::SigScanner scanner("test_data/example.bin");

    std::intptr_t offset = 10u;

    scanner.SetOffset(offset);
    scanner.SetFunctionStartSig({ {"55 8B EC"} });

    // Single start function signature, single normal signature
    MPSig::SigScannerEntry movEntry({ "68 ?? ?? ?? ??" });
    std::pair<std::intptr_t, std::intptr_t> scanResult = scanner.Scan(movEntry);
    REQUIRE(scanResult.first == 8u + offset);
    REQUIRE(scanResult.second == 2u + offset);

    // Single start function signature, multiple normal signature
    MPSig::SigScannerEntry nopEntries({ "90 68", "90 C9" });
    std::pair<std::intptr_t, std::intptr_t> nopScanResult = scanner.Scan(nopEntries);
    REQUIRE(nopScanResult.first == 7u + offset);
    REQUIRE(nopScanResult.second == 2u + offset);

    // Function signature which does not exist in the example
    MPSig::SigScannerEntry notExistingEntry({ "01 02 03 04" });
    std::pair<std::intptr_t, std::intptr_t> invalidResult = scanner.Scan(notExistingEntry);
    REQUIRE(invalidResult.first == 0);
    REQUIRE(invalidResult.second == 0);

    // Multiple start function signatures (+ with one non-existing)
    scanner.SetFunctionStartSig({ "01 02 03 04", "55 8B EC" });

    // Multiple function signatures with non-existing ones
    MPSig::SigScannerEntry mixedEntry({ "01 02 03 04", "C9 C2 ?? ??" });
    std::pair<std::intptr_t, std::intptr_t> mixedResult = scanner.Scan(mixedEntry);
    REQUIRE(mixedResult.first == 15u + offset);
    REQUIRE(mixedResult.second == 2u + offset);

}


TEST_CASE("Tests the scanner with Pattern (new version)", "[sigscanner]") {
    MPSig::SigScanner scanner("test_data/example.bin");

    std::intptr_t offset = 10u;

    scanner.SetOffset(offset);
    scanner.SetFunctionStartPattern(MPSig::SearchPatternBuilder().hasHexPattern("55 8B EC").compile());

    // Single start function signature, single normal signature
    MPSig::SearchPattern movEntry = 
        MPSig::SearchPatternBuilder()
            .hasHexPattern(gsl::ensure_z("68 ?? ?? ?? ??")).compile();
    std::pair<std::intptr_t, std::intptr_t> scanResult = scanner.Scan(movEntry);
    REQUIRE(scanResult.first == 8u + offset);
    REQUIRE(scanResult.second == 2u + offset);

    // Single start function signature, multiple normal signature
    MPSig::SearchPattern nopEntry1 =
        MPSig::SearchPatternBuilder()
            .hasHexPattern(gsl::ensure_z("90 68")).compile();
    MPSig::SearchPattern nopEntry2 =
        MPSig::SearchPatternBuilder()
            .hasHexPattern(gsl::ensure_z("90 C9")).compile();
    std::pair<std::intptr_t, std::intptr_t> nopScanResult = scanner.Scan({ nopEntry1, nopEntry2 });
    REQUIRE(nopScanResult.first == 7u + offset);
    REQUIRE(nopScanResult.second == 2u + offset);

    // Function signature which does not exist in the example
    MPSig::SearchPattern notExistingEntry =
        MPSig::SearchPatternBuilder()
            .hasHexPattern(gsl::ensure_z("01 02 03 04")).compile();
    std::pair<std::intptr_t, std::intptr_t> invalidResult = scanner.Scan(notExistingEntry);
    REQUIRE(invalidResult.first == 0);
    REQUIRE(invalidResult.second == 0);


}