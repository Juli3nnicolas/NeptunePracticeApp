cmake_minimum_required(VERSION 3.10)

# Local source
add_subdirectory(Shaders)

target_sources(NeptunePracticeApp
PRIVATE
	"Resources/CMakeLists.txt")