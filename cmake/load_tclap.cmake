
include(ExternalProject)
# find_package(Git REQUIRED)

ExternalProject_Add(
    tclap
    PREFIX ${CMAKE_BINARY_DIR}/external/tclap
    URL http://downloads.sourceforge.net/project/tclap/tclap-1.2.1.tar.gz
    TIMEOUT 10
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(tclap source_dir)
message(STATUS "tclap source dir is: ${source_dir}")

set(TCLAP_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for tclap")
