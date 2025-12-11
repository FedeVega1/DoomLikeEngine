cmake_minimum_required(VERSION 3.20)

project(DoomLikeEngine)

include(FetchContent)

FetchContent_Declare(
    COLogger
    GIT_REPOSITORY https://github.com/FedeVega1/COLogger.git
    GIT_TAG        main 
)
FetchContent_MakeAvailable(COLogger)

include(FetchContent)

FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(SDL) 

add_subdirectory(DoomCopy)
add_subdirectory(LevelEditor)
