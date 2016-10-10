
include(ExternalProject)
find_package(Git REQUIRED)

set(GSL_CUSTOM_INSTALL_DIR "${CMAKE_BINARY_DIR}/external/gsl-install")

ExternalProject_Add(
    gsl
    PREFIX ${CMAKE_BINARY_DIR}/external/gsl
    GIT_REPOSITORY https://github.com/Microsoft/GSL.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    # CONFIGURE_COMMAND ""
    # BUILD_COMMAND ""
    # INSTALL_COMMAND ""
    # INSTALL_DIR "${CMAKE_BINARY_DIR}/external/gsl-install"
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${GSL_CUSTOM_INSTALL_DIR}"
    LOG_DOWNLOAD ON
)

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
# ExternalProject_Get_Property(gsl source_dir)

message(STATUS "gsl source dir is: ${GSL_CUSTOM_INSTALL_DIR}")

set(GSL_INCLUDE_DIR ${GSL_CUSTOM_INSTALL_DIR}/include CACHE INTERNAL "Path to include folder for gsl")
