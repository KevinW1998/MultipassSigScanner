cmake_minimum_required (VERSION 3.6)

project (MultipassSigscanner)

# Enable Testing
enable_testing ()

# Now check for Cppcheck
include (cmake/3rdparty/Findcppcheck.cmake)
if(CPPCHECK_FOUND)
    message(STATUS "Cppcheck found")
else()
    message(STATUS "Cppcheck not found")
endif()




# Add source (Executable + Library)
add_subdirectory (src) 
# Add test (Depending on Library)
add_subdirectory (test)





