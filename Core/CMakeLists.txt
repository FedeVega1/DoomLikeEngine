cmake_minimum_required(VERSION 3.20)

project(Core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/VectorMath.cpp
    src/WorldTypes.cpp
    src/BSPFileIO.cpp
    src/pch.cpp
)

set(HEADERS
    include/Core/VectorMath.h
    include/Core/WorldTypes.h
    include/Core/BSPFileIO.h
    include/pch.h
)

add_library(Core STATIC ${SOURCES} ${HEADERS})

target_include_directories(Core PUBLIC include/)
target_precompile_headers(Core PRIVATE include/pch.h)

target_compile_definitions(Core PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
target_compile_definitions(Core PRIVATE $<$<CONFIG:Release>:NDEBUG>)

set_target_properties(Core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/bin/Core/Debug - x64"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/bin/Core/Release - x64"
    COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG      "${CMAKE_SOURCE_DIR}/bin/Core/Debug - x64"
    COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE    "${CMAKE_SOURCE_DIR}/bin/Core/Release - x64"
)
