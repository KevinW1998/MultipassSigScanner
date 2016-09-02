
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
include(load_catch.cmake)


include_directories (${CMAKE_SOURCE_DIR}/src
                     ${CATCH_INCLUDE_DIR}
                     )

add_executable (MultipassSigscannerTest test.cpp)
target_link_libraries (MultipassSigscannerTest
                       MultipassSigscannerLib
                       )
                       


file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

