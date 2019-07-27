cmake_minimum_required(VERSION 3.10)

# Local source
add_subdirectory(SelfPractice)

message("########## Executing Resources/Shaders/CMakeLists.txt")

# List header files, they must be #includable by users
list(APPEND SHADER_FILES 
	"Resources/Shaders/basic_uniformblock.vert"
	"Resources/Shaders/basic_uniformblock.frag"
	"Resources/Shaders/UniformBlock.vert"
	"Resources/Shaders/UniformBlock.frag"
	"Resources/Shaders/UniformVariable.vert"
	"Resources/Shaders/VertexBuffer.vert"
	"Resources/Shaders/VertexBuffer.frag"
)

# Add source files
target_sources(NeptunePracticeApp
PRIVATE
	${SHADER_FILES}
	"Resources/Shaders/CMakeLists.txt")
