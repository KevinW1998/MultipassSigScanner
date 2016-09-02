
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

add_library (MultipassSigscannerLib ${MULTIPASS_SIGSCANNER_FILES})
add_executable (MultipassSigscannerMain main.cpp)
target_link_libraries (MultipassSigscannerMain MultipassSigscannerLib)

