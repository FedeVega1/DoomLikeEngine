cmake_minimum_required(VERSION 3.20)

project(DoomCopy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    CameraComponent.cpp
    D2Renderer.cpp
    DoomCopy.cpp
    FileManager.cpp
    Game.cpp
    GameObjects.cpp
    GDIRenderer.cpp
    Inputs.cpp
    Renderer.cpp
    TransformComponent.cpp
    World.cpp
)

set(HEADERS
    CameraComponent.h
    Component.h
    D2Renderer.h
    DoomCopy.h
    FileManager.h
    Inputs.h
    pch.h
    Game.h
    GDIRenderer.h
    GameObjects.h
    Renderer.h
    RenderingDataTypes.h
    targetver.h
    GameTime.h
    TransformComponent.h
    VectorMath.h
    World.h
    WorldDataTypes.h
)

add_executable(DoomCopy WIN32 ${SOURCES} ${HEADERS} pch.cpp)

target_precompile_headers(DoomCopy PRIVATE pch.h)

target_compile_definitions(DoomCopy PRIVATE _WINDOWS UNICODE _UNICODE)
target_compile_definitions(DoomCopy PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
target_compile_definitions(DoomCopy PRIVATE $<$<CONFIG:Release>:NDEBUG>)

target_link_libraries(DoomCopy PRIVATE COLogger_static)

if(WIN32)
    target_link_libraries(DoomCopy PRIVATE d2d1.lib)
endif()

set_target_properties(DoomCopy PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Debug - x64"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Release - x64"
    PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Debug - x64"
    PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Release - x64"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DoomCopy)
