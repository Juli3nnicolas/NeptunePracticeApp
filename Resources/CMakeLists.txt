cmake_minimum_required(VERSION 3.12)

# Local source
add_subdirectory(Shaders)

target_sources(NeptunePracticeApp
PRIVATE
	"Resources/CMakeLists.txt")