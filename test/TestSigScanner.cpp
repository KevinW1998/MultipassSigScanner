#include <catch.hpp>

#include <SigScanner.h>
TEST_CASE("Tests the scanner", "[sigscanner]") {
    MPSig::SigScanner scanner("test_data/example.bin");

    std::intptr_t offset = 10u;

    scanner.SetOffset(offset);
    scanner.SetFunctionStartSig({ '\x55', '\x8B', '\xEC' });

    MPSig::SigScannerEntry movEntry({ "68 ?? ?? ?? ??" });
    std::pair<std::intptr_t, std::intptr_t> scanResult = scanner.Scan(movEntry);
    REQUIRE(scanResult.first == 8u + offset);
    REQUIRE(scanResult.second == 2u + offset);
}


