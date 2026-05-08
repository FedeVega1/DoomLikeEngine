cmake_minimum_required(VERSION 3.20)

project(DoomCopy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/CameraComponent.cpp
    src/D2Renderer.cpp
    src/Game.cpp
    src/GameObjects.cpp
    src/GDIRenderer.cpp
    src/Inputs.cpp
    src/Renderer.cpp
    src/TransformComponent.cpp
    src/World.cpp
    src/SDLRenderer.cpp
    src/Main.cpp
)

set(HEADERS
    include/CameraComponent.h
    include/Component.h
    include/D2Renderer.h
    include/Inputs.h
    include/pch.h
    include/Game.h
    include/GDIRenderer.h
    include/GameObjects.h
    include/Renderer.h
    include/RenderingDataTypes.h
    include/targetver.h
    include/GameTime.h
    include/TransformComponent.h
    include/World.h
    include/SDLRenderer.h
    include/Main.h)

add_executable(DoomCopy WIN32 ${SOURCES} ${HEADERS} src/pch.cpp)

target_include_directories(DoomCopy PRIVATE include/)
target_precompile_headers(DoomCopy PRIVATE include/pch.h)

target_compile_definitions(DoomCopy PRIVATE _WINDOWS UNICODE _UNICODE)
target_compile_definitions(DoomCopy PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
target_compile_definitions(DoomCopy PRIVATE $<$<CONFIG:Release>:NDEBUG>)

target_link_libraries(DoomCopy PRIVATE Core)
target_link_libraries(DoomCopy PRIVATE COLogger_static)
target_link_libraries(DoomCopy PRIVATE SDL3::SDL3)
target_link_libraries(DoomCopy PRIVATE ImGui)

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
