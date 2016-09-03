
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    json
    PREFIX ${CMAKE_BINARY_DIR}/external/json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(json source_dir)
message(STATUS "Json source dir is: ${source_dir}")

set(JSON_INCLUDE_DIR ${source_dir}/src CACHE INTERNAL "Path to include folder for Json")