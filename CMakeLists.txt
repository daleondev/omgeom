cmake_minimum_required(VERSION 4.2.0 FATAL_ERROR)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

# --- Project Setup ---
project(omgeom
    VERSION 0.1.0
    DESCRIPTION "OMG Geometry"
    LANGUAGES CXX
)

# --- Library Target Definition ---
add_executable(omgeom)

# --- Source Files ---
file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE MODULES 
    CONFIGURE_DEPENDS 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cppm")

file(GLOB_RECURSE HEADERS
    CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

target_sources(omgeom
    PRIVATE
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ${MODULES}
    PRIVATE
        ${SOURCES}
        ${HEADERS}
)

# --- Include Directories ---
target_include_directories(omgeom
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# --- Compile Features ---
target_compile_features(omgeom
    PUBLIC
        cxx_std_23
)

# --- Compile Options ---
target_compile_options(omgeom
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
)

# --- Link Options ---
target_link_options(omgeom 
    PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
)

# --- Source Properties/Groups for IDE ---
foreach(h IN LISTS HEADERS)
    set_source_files_properties(${h} PROPERTIES HEADER_FILE_ONLY TRUE)
endforeach()
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PREFIX "Source Files"
    FILES 
        ${SOURCES} 
        ${MODULES}
)
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PREFIX "Header Files"
    FILES 
        ${HEADERS}
)