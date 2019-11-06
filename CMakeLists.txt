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
include("CMake/AddLibraries.cmake")

# Add cmake list to source list
target_sources(NeptunePracticeApp
PRIVATE
	"CMakeLists.txt")

# Local source
add_subdirectory(CMake)
