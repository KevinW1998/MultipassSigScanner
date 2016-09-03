
# The Library
set(MULTIPASS_SIGSCANNER_FILES 
    exceptions/SigScannerFileOpenException.h
    exceptions/SigScannerFileOpenException.cpp
    SigScannerEntry.h 
    SigScannerEntry.cpp 
    SigFileUtils.h 
    SigFileUtils.cpp 
    SigScannerJsonConfig.h 
    SigScannerJsonConfig.cpp
    SigScanner.h 
    SigScanner.cpp
)
add_library (MultipassSigscannerLib ${MULTIPASS_SIGSCANNER_FILES})

# The Exe
include(../cmake/load_tclap.cmake)
include(../cmake/load_json.cmake)
include_directories(
    ${TCLAP_INCLUDE_DIR}
    ${JSON_INCLUDE_DIR}        
)

add_executable (MultipassSigscannerMain main.cpp)
target_link_libraries (MultipassSigscannerMain MultipassSigscannerLib)

