cmake_minimum_required(VERSION 3.11)
# Has to be the same as the 'pname' on 'flake.nix'.
project(stone-path CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE) # disable x11 on linux
# Not enabling wayland because I'd have to figure out the correct dependencies.
# set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE) # enable wayland on linux

FetchContent_Declare(raylib SOURCE_DIR $ENV{RAYLIB_PATH})
FetchContent_GetProperties(raylib)
FetchContent_MakeAvailable(raylib)

# Adding our source files
file(
    GLOB_RECURSE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
)
# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Setting ASSETS_PATH
# target_compile_definitions( ${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
# target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable

install(TARGETS stone-path DESTINATION bin)
