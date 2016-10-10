include(../cmake/load_catch.cmake)

include_directories (${CMAKE_SOURCE_DIR}/src
                     ${CATCH_INCLUDE_DIR}
                     )

add_executable (MultipassSigscannerTest 
                TestMain.cpp
                TestSigFileUtils.cpp
                TestSigScanner.cpp
                TestSigScannerEntry.cpp
                )
target_link_libraries (MultipassSigscannerTest
                       MultipassSigscannerLib
                       )

set_target_properties(MultipassSigscannerTest PROPERTIES COMPILE_DEFINITIONS "GSL_THROW_ON_CONTRACT_VIOLATION")                       

file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_test (NAME MultipassSigscannerTestProj COMMAND MultipassSigscannerTest)
