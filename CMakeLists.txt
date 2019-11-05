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
include("CMake/PostBuildOperations.cmake")

# Add include directories
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Dependencies/All)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Dependencies/Win32)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Win32/HighLevel)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Win32/Core)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Multiplatform/HighLevel)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Multiplatform/Core)

# Add libraries
target_link_libraries(NeptunePracticeApp general opengl32)

find_library(GLEW "glew32" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp general ${GLEW})

find_library(SDL2 "SDL2"  ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp general ${SDL2})

find_library(Neptune "Neptune" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp debug ${Neptune})

find_library(LIB_KTX "libktx.gld" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp debug ${LIB_KTX})

find_library(ASSIMP "assimp" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp debug ${ASSIMP})

# Add cmake list to source list
target_sources(NeptunePracticeApp
PRIVATE
	"CMakeLists.txt")

# Local source
add_subdirectory(CMake)
