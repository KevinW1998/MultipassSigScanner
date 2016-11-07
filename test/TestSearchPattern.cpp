#include <catch.hpp>
#include <search/SearchPatternBuilder.h>

TEST_CASE("Tests the SearchPattern", "[searchpattern]") {
    MPSig::SearchPattern a = MPSig::SearchPatternBuilder().HasHexPattern("AA BB CC DD EE FF").Compile();
    MPSig::SearchPattern b = a; // Invoke copy assignment
    MPSig::SearchPattern c(a); // Invoke copy constructor
}