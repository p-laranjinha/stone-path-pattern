cmake_minimum_required(VERSION 3.11)
# Has to be the same as the 'pname' on 'flake.nix'.
project(stone-path CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib QUIET)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_RAYLIB_CPP_MODULES ON CACHE BOOL "" FORCE)
FetchContent_Declare(raylib_cpp SOURCE_DIR $ENV{RAYLIB_CPP_PATH})
FetchContent_MakeAvailable(raylib_cpp)
target_link_libraries(raylib_cpp INTERFACE raylib)

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
target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp)

install(TARGETS stone-path DESTINATION bin)
