# This scripts attempts to build NeptunePracticeApp in Debug on the ia64 architecture
cmake_minimum_required(VERSION 3.12)

project(NeptunePracticeApp)

# Create executable
add_executable(NeptunePracticeApp)

# Define preprocesor constants
target_compile_definitions(NeptunePracticeApp PUBLIC $<$<CONFIG:DEBUG>:NEP_DEBUG>)

# Project config
include("CMake/BuildProjectTree.cmake")
include("CMake/IncludeSources.cmake")

# Add include directories
#target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/../Neptune/Dependencies/All/Include)
#target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/../Neptune/Dependencies/Win32/Include)
#target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/../Neptune/Engine/Win32/HighLevel)
#target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/../Neptune/Engine/Win32/Core)
#target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/../Neptune/Engine/Multiplatform/HighLevel)
#target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/../Neptune/Engine/Multiplatform/Core)

# Add libraries
#link_directories(${CMAKE_SOURCE_DIR}/../Neptune/Dependencies/Win32/Bin/X64)
#link_directories(${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
#target_link_libraries(NeptunePracticeApp general opengl32)
#target_link_libraries(NeptunePracticeApp general glew32)
#target_link_libraries(NeptunePracticeApp general SDL2)
#target_link_libraries(NeptunePracticeApp debug Neptune)

# Add cmake list to source list
target_sources(NeptunePracticeApp
PRIVATE
	"CMakeLists.txt")

# Local source
add_subdirectory(CMake)
