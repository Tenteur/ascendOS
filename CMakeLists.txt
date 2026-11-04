cmake_minimum_required(VERSION 3.20)
project(ascendOS LANGUAGES C)

# The code uses `constexpr` (C23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Default to Release when no build type is given
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug or Release)" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
endif()

# Sources: everything in src/ except the obsolete jsoncParser.c (it has its own main())
file(GLOB APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM APP_SOURCES "${CMAKE_SOURCE_DIR}/src/jsoncParser.c")

add_executable(ascendOS ${APP_SOURCES})
target_include_directories(ascendOS PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Executable goes directly into the build folder
set_target_properties(ascendOS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
)

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3        REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SDL3_IMAGE  REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_TTF    REQUIRED IMPORTED_TARGET sdl3-ttf)
pkg_check_modules(SDL3_MIXER  REQUIRED IMPORTED_TARGET sdl3-mixer)
pkg_check_modules(YAML        REQUIRED IMPORTED_TARGET yaml-0.1)

target_link_libraries(ascendOS PRIVATE
    PkgConfig::SDL3
    PkgConfig::SDL3_IMAGE
    PkgConfig::SDL3_TTF
    PkgConfig::SDL3_MIXER
    PkgConfig::YAML
    m
)

target_compile_options(ascendOS PRIVATE -Wall -Wextra)

# Release: maximum optimization, no debug asserts
target_compile_options(ascendOS PRIVATE
    "$<$<CONFIG:Release>:-O3;-flto;-fomit-frame-pointer>")
target_compile_definitions(ascendOS PRIVATE "$<$<CONFIG:Release>:NDEBUG>")
target_link_options(ascendOS PRIVATE "$<$<CONFIG:Release>:-flto;-s>")

# Debug: no optimization, full debug info, nothing that hinders debugging
target_compile_options(ascendOS PRIVATE
    "$<$<CONFIG:Debug>:-O0;-g3;-ggdb;-fno-omit-frame-pointer;-fno-inline;-fno-optimize-sibling-calls>")
target_compile_definitions(ascendOS PRIVATE "$<$<CONFIG:Debug>:DEBUG>")
