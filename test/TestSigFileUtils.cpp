#include <catch.hpp>

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

