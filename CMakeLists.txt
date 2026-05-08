cmake_minimum_required(VERSION 3.20)

project(DoomLikeEngine)

include(FetchContent)

# Prevent automatic updates of dependencies
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Disable automatic updates of dependencies")

FetchContent_Declare(
    COLogger
    GIT_REPOSITORY https://github.com/FedeVega1/COLogger.git
    GIT_TAG        main
)

FetchContent_GetProperties(COLogger)
if(NOT COLogger_POPULATED)
    FetchContent_MakeAvailable(COLogger)
endif()

FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main
)

FetchContent_GetProperties(SDL)
if(NOT SDL_POPULATED)
    FetchContent_MakeAvailable(SDL)
endif()

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
)

FetchContent_GetProperties(ImGui)
if(NOT ImGui_POPULATED)
    FetchContent_MakeAvailable(ImGui)
endif()

add_library(ImGui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(ImGui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(ImGui PUBLIC SDL3::SDL3)

add_subdirectory(Core)
add_subdirectory(Editor)
add_subdirectory(DoomCopy)
add_subdirectory(LevelEditor)
