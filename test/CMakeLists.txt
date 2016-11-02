include(../cmake/load_catch.cmake)

include_directories (${CMAKE_SOURCE_DIR}/src
                     ${CATCH_INCLUDE_DIR}
                     )

add_executable (MultipassSigscannerTest 
                TestMain.cpp
                TestSigFileUtils.cpp
                TestSigScanner.cpp
                TestSearchPatternRefBSTR.cpp
                )
target_link_libraries (MultipassSigscannerTest
                       MultipassSigscannerLib
                       )

set_target_properties(MultipassSigscannerTest PROPERTIES COMPILE_DEFINITIONS "GSL_THROW_ON_CONTRACT_VIOLATION")                       

add_custom_command(TARGET MultipassSigscannerTest POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/test/test_data ${CMAKE_CURRENT_BINARY_DIR}/test_data)

add_test (NAME MultipassSigscannerTestProj COMMAND MultipassSigscannerTest)
