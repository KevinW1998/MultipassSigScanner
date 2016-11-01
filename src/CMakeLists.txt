
# General Settings
set(MULTIPASS_SIGSCANNER_FILES 
    exceptions/SigScannerFileOpenException.h
    exceptions/SigScannerFileOpenException.cpp
    search/SearchPatternBuilder.h
    search/SearchPatternBuilder.cpp
    search/SearchPattern.h
    search/SearchPattern.cpp
    search/detail/SearchPatternBase.h
    search/detail/SearchPatternHexMatcher.h
    search/detail/SearchPatternHexMatcher.cpp
    search/detail/SearchPatternRefBSTR.h
    search/detail/SearchPatternRefBSTR.cpp
    SigFileUtils.h 
    SigFileUtils.cpp 
    SigScannerMemoryData.h 
    SigScannerMemoryData.cpp
    SigScanner.h 
    SigScanner.cpp

)

# TODO: Add clang-tidy if available
# set(CMAKE_CXX_CLANG_TIDY "F:/clang-dynamic/bin/clang-tidy.exe")

# The Library
add_library (MultipassSigscannerLib ${MULTIPASS_SIGSCANNER_FILES})
# Library dependencies
include(../cmake/load_gsl.cmake)
target_include_directories(MultipassSigscannerLib PUBLIC ${GSL_INCLUDE_DIR})
add_dependencies(MultipassSigscannerLib gsl)

# The Exe
add_executable (MultipassSigscannerMain main.cpp)
# Exe Dependencies
include(../cmake/load_json.cmake)
include(../cmake/load_tclap.cmake)
set(MULTIPASS_ADDITIONAL_INCLUDES ${TCLAP_INCLUDE_DIR} ${JSON_INCLUDE_DIR})
target_include_directories(MultipassSigscannerMain PUBLIC ${MULTIPASS_ADDITIONAL_INCLUDES})
add_dependencies(MultipassSigscannerMain tclap json)
# Link the library
target_link_libraries (MultipassSigscannerMain MultipassSigscannerLib)

if(CPPCHECK_FOUND)
    message(STATUS "Added MultipassSigscannerLib to cppcheck!")
    include (../cmake/3rdparty/CppcheckTargets.cmake)
    add_cppcheck(MultipassSigscannerLib STYLE FORCE)
endif()

