#include <catch.hpp>

#include <stdexcept>
#include <SigScannerEntry.h>

TEST_CASE("A invalid signature is compiled", "[sigscannerentry]") {
    REQUIRE_THROWS_AS(MPSig::SigScannerEntry({ "A0 FC XY CD" }), std::invalid_argument);
}

TEST_CASE("A incomplete signature (at the end) is compiled", "[sigscannerentry]") {
    REQUIRE_THROWS_AS(MPSig::SigScannerEntry({ "A0 FC C" }), std::invalid_argument);
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
