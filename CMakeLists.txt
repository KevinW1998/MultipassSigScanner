cmake_minimum_required (VERSION 2.8)

project (MultipassSigscanner)

# Enable Testing
enable_testing ()

# Now check for Cppcheck
include (cmake/3rdparty/Findcppcheck.cmake)
if(CPPCHECK_FOUND)
    message(STATUS "Cppcheck found, adding target")
    include (cmake/3rdparty/CppcheckTargets.cmake)
else()
    message(STATUS "Cppcheck not found, skipping target")
endif()




# Add source (Executable + Library)
add_subdirectory (src) 
# Add test (Depending on Library)
add_subdirectory (test)





