
# General Settings
set(MULTIPASS_SIGSCANNER_FILES 
    exceptions/SigScannerFileOpenException.h
    exceptions/SigScannerFileOpenException.cpp
    SigScannerEntry.h 
    SigScannerEntry.cpp 
    SigFileUtils.h 
    SigFileUtils.cpp 
    SigScanner.h 
    SigScanner.cpp
)

include(../cmake/load_tclap.cmake)
include(../cmake/load_json.cmake)
set(MULTIPASS_ADDITIONAL_INCLUDES ${TCLAP_INCLUDE_DIR} ${JSON_INCLUDE_DIR})
add_library (MultipassSigscannerLib ${MULTIPASS_SIGSCANNER_FILES})
target_include_directories(MultipassSigscannerLib PUBLIC ${MULTIPASS_ADDITIONAL_INCLUDES})

# The Exe
add_executable (MultipassSigscannerMain main.cpp)
target_link_libraries (MultipassSigscannerMain MultipassSigscannerLib)

if(CPPCHECK_FOUND)
    message(STATUS "Added MultipassSigscannerLib to cppcheck!")
    include (../cmake/3rdparty/CppcheckTargets.cmake)
    add_cppcheck(MultipassSigscannerLib STYLE FORCE)
endif()
