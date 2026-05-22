cmake_minimum_required(VERSION 3.20)

project(Editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/Main.cpp
    src/EditorApp.cpp
    src/MapView.cpp
    src/MapRenderer.cpp
    src/MapFileIO.cpp
    src/BSPCompiler.cpp
    src/PropertiesPanel.cpp
    src/OptionsPanel.cpp
    src/MapGrid.cpp
    src/EditorCommands.cpp
    src/SelectionManager.cpp
    src/MapData.cpp
)

set(HEADERS
    include/Editor/EditorApp.h
    include/Editor/MapView.h
    include/Editor/MapRenderer.h
    include/Editor/MapFileIO.h
    include/Editor/BSPCompiler.h
    include/Editor/Panels/PropertiesPanel.h
    include/Editor/EditorTypes.h
    include/Editor/Main.h
    include/Editor/MapGrid.h
    include/Editor/Config/EditorTheme.h
    include/Editor/Config/EditorHotKeys.h
    include/Editor/Config/EditorConfiguration.h
    include/Editor/Panels/OptionsPanel.h
    include/Editor/CommandHistory.h
    include/Editor/EditorCommands.h
    include/Editor/SelectionManager.h
    include/pch.h
    include/imconfig.h
)

add_executable(Editor ${SOURCES} ${HEADERS} src/pch.cpp)

target_include_directories(Editor PRIVATE include/)
target_precompile_headers(Editor PRIVATE include/pch.h)

target_link_libraries(Editor PRIVATE Core)
target_link_libraries(Editor PRIVATE COLogger_static)
target_link_libraries(Editor PRIVATE Core SDL3::SDL3 ImGui)

if(WIN32)
    set_target_properties(Editor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

target_compile_definitions(Editor PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
target_compile_definitions(Editor PRIVATE $<$<CONFIG:Release>:NDEBUG>)
target_compile_definitions(Editor PRIVATE IMGUI_USER_CONFIG="imconfig.h")

if(WIN32)
    add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/_deps/sdl-build/$<CONFIG>/SDL3.dll"
        "$<TARGET_FILE_DIR:Editor>"
        COMMENT "Copying SDL3.dll to Editor output directory"
    )
endif()

set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/Editor/Debug - x64"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Editor/Release - x64"
    PDB_OUTPUT_DIRECTORY_DEBUG       "${CMAKE_SOURCE_DIR}/bin/Editor/Debug - x64"
    PDB_OUTPUT_DIRECTORY_RELEASE     "${CMAKE_SOURCE_DIR}/bin/Editor/Release - x64"
    VS_DEBUGGER_WORKING_DIRECTORY    "${CMAKE_CURRENT_SOURCE_DIR}"
)
