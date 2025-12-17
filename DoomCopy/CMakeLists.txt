cmake_minimum_required(VERSION 3.20)

project(DoomCopy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    CameraComponent.cpp
    D2Renderer.cpp
    FileManager.cpp
    Game.cpp
    GameObjects.cpp
    GDIRenderer.cpp
    Inputs.cpp
    Renderer.cpp
    TransformComponent.cpp
    World.cpp
    SDLRenderer.cpp
    Main.cpp
)

set(HEADERS
    CameraComponent.h
    Component.h
    D2Renderer.h
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
    SDLRenderer.h 
    Main.h)

add_executable(DoomCopy WIN32 ${SOURCES} ${HEADERS} pch.cpp)

target_precompile_headers(DoomCopy PRIVATE pch.h)

target_compile_definitions(DoomCopy PRIVATE _WINDOWS UNICODE _UNICODE)
target_compile_definitions(DoomCopy PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
target_compile_definitions(DoomCopy PRIVATE $<$<CONFIG:Release>:NDEBUG>)

target_link_libraries(DoomCopy PRIVATE COLogger_static)
target_link_libraries(DoomCopy PRIVATE SDL3::SDL3)

if(WIN32)
    target_link_libraries(DoomCopy PRIVATE d2d1.lib)
    
    # Copy SDL3.dll to output directory
    # SDL3 DLL is built in the _deps folder by FetchContent
    add_custom_command(TARGET DoomCopy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/_deps/sdl-build/$<CONFIG>/SDL3.dll"
        "$<TARGET_FILE_DIR:DoomCopy>"
        COMMENT "Copying SDL3.dll to output directory"
    )
endif()

set_target_properties(DoomCopy PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Debug - x64"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Release - x64"
    PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Debug - x64"
    PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/DoomCopy/Release - x64"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DoomCopy)
