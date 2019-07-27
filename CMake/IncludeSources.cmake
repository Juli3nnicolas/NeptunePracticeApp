cmake_minimum_required (VERSION 3.12)

message("########## Running Cmake/IncludeSources.cmake")
message("Loading cpp source and header files")

# List header files, they must be #includable by users
list(APPEND HEADERS 
	"src/FactoryExamples.h"
	"src/InputSystemExamples.h"
	"src/Mandelbrot.h"
	"src/SimpleAppExamples.h"
	"src/ViewSpawnerExamples.h"
)

# List cpp files, useful for compilation purpose only
list(APPEND CPP_FILES
    "src/main.cpp"
    "src/FactoryExamples.cpp"
	"src/InputSystemExamples.cpp"
	"src/Mandelbrot.cpp"
	"src/SimpleAppExamples.cpp"
	"src/ViewSpawnerExamples.cpp"
)

# Add source files
target_sources(NeptunePracticeApp
PUBLIC
	${HEADERS}
PRIVATE
	${CPP_FILES}
)

## Include shader files
message("Loading shader files")
list(APPEND SHADER_FILES 
	"Resources/Shaders/basic_uniformblock.vert"
	"Resources/Shaders/basic_uniformblock.frag"
	"Resources/Shaders/UniformBlock.vert"
	"Resources/Shaders/UniformBlock.frag"
	"Resources/Shaders/UniformVariable.vert"
	"Resources/Shaders/VertexBuffer.vert"
	"Resources/Shaders/VertexBuffer.frag"
	"Resources/Shaders/SelfPractice/diffuseLighting.vert"
	"Resources/Shaders/SelfPractice/diffuseLightingUniform.vert"
	"Resources/Shaders/SelfPractice/DisplayWithDividedColors.vert"
	"Resources/Shaders/SelfPractice/ModelToClipSpace.vert"
	"Resources/Shaders/SelfPractice/test.vert"
	"Resources/Shaders/SelfPractice/test.frag"
	"Resources/Shaders/SelfPractice/testView.vert"
	"Resources/Shaders/SelfPractice/testView.frag"
	"Resources/Shaders/SelfPractice/xwingDiffuseLighting.vert"
	"Resources/Shaders/SelfPractice/xwingDiffuseLightingUniform.vert"
)

# Add source files
target_sources(NeptunePracticeApp
PRIVATE
	${SHADER_FILES}
)