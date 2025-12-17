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

add_subdirectory(DoomCopy)
add_subdirectory(LevelEditor)
