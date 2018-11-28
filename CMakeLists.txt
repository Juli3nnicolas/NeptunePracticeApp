# This scripts attempts to build NeptunePracticeApp in Debug on the ia64 architecture
cmake_minimum_required(VERSION 3.10)

project(NeptunePracticeApp)

# Local source
add_subdirectory(Resources)
add_subdirectory(Src)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/../Neptune/Dependencies/All/Include)
include_directories(${CMAKE_SOURCE_DIR}/../Neptune/Dependencies/Win32/Include)
include_directories(${CMAKE_SOURCE_DIR}/../Neptune/Engine/Win32/HighLevel)
include_directories(${CMAKE_SOURCE_DIR}/../Neptune/Engine/Win32/Core)
include_directories(${CMAKE_SOURCE_DIR}/../Neptune/Engine/Multiplatform/HighLevel)
include_directories(${CMAKE_SOURCE_DIR}/../Neptune/Engine/Multiplatform/Core)

# Define preprocesor constants
add_definitions(NEP_DEBUG)

# Create executable
add_executable(NeptunePracticeApp ${CMAKE_SOURCE_DIR}/Src/*)

# Add libraries
link_directories(${CMAKE_SOURCE_DIR}/../Neptune/Dependencies/Win32/Bin/X64)
link_directories(${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp general opengl32)
target_link_libraries(NeptunePracticeApp general glew32)
target_link_libraries(NeptunePracticeApp general SDL2)
target_link_libraries(NeptunePracticeApp debug Neptune)

