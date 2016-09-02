#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <stdexcept>


#include <SigScannerEntry.h>

TEST_CASE("A invalid signature is compiled", "[sigscannerentry]") {
    REQUIRE_THROWS_AS( MPSig::SigScannerEntry({ "A0 FC XY CD" }), std::invalid_argument);
}

TEST_CASE("A incomplete signature (at the end) is compiled", "[sigscannerentry]") {
    REQUIRE_THROWS_AS(MPSig::SigScannerEntry({"A0 FC C"}), std::invalid_argument);
}

TEST_CASE("A incomplete signature (in the middle) is compiled", "[sigscannerentry]") {
    REQUIRE_THROWS_AS(MPSig::SigScannerEntry({ "A0 C FC" }), std::invalid_argument);
}

class SigScannerEntryTest : protected MPSig::SigScannerEntry {
public:
    SigScannerEntryTest(const std::initializer_list<std::string>& patterns) : MPSig::SigScannerEntry(patterns) {};
    const auto& GetCompiledPatternsForTest() { return GetCompiledPatterns(); }
};


TEST_CASE("A valid signature is compiled", "[sigscannerentry]") {
    std::unique_ptr<SigScannerEntryTest> entryTest;
    REQUIRE_NOTHROW(entryTest.reset(new SigScannerEntryTest({ "A0 ?? CD" })));
    
    const auto& patterns = entryTest->GetCompiledPatternsForTest();
    REQUIRE(patterns.size() == 1u);
    
    const auto& firstPattern = patterns[0];
    REQUIRE(firstPattern.size() == 3u);

    REQUIRE(firstPattern[0].first == '\xA0');
    REQUIRE(firstPattern[1].first == '\x00');
    REQUIRE(firstPattern[2].first == '\xCD');

    REQUIRE(firstPattern[0].second == true);
    REQUIRE(firstPattern[1].second == false);
    REQUIRE(firstPattern[2].second == true);
}


// =============================================================

#include <SigFileUtils.h>
#include <exceptions/SigScannerFileOpenException.h>

TEST_CASE("Opens a invalid file", "[sigfileutils]") {
    REQUIRE_THROWS_AS(MPSig::Utils::ReadFileBinary("abc.bin"), MPSig::SigScannerFileOpenException);
}

TEST_CASE("Reads a file in binary", "[sigfileutils]") {
    std::vector<char> data = MPSig::Utils::ReadFileBinary("test_data/example.bin");
    // The the first bytes and length
    REQUIRE(data.size() == 19u);
    REQUIRE(data[0] == '\x00');
    REQUIRE(data[1] == '\x00');
    REQUIRE(data[2] == '\x55');
    REQUIRE(data[3] == '\x8B');
    REQUIRE(data[4] == '\xEC');
}



// =============================================================


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


