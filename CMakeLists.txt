cmake_minimum_required(VERSION 3.20)

project(DoomLikeEngine)


# Dependencies
include(FetchContent)

FetchContent_Declare(
    COLogger
    GIT_REPOSITORY https://github.com/FedeVega1/COLogger.git
    GIT_TAG        main 
)
FetchContent_MakeAvailable(COLogger)

# Add subdirectories
add_subdirectory(DoomCopy)
add_subdirectory(LevelEditor)
