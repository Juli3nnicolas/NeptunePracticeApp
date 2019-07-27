cmake_minimum_required(VERSION 3.10)

message("########## Executing Resources/Shaders/SelfPractice/CMakeLists.txt")

# List header files, they must be #includable by users
list(APPEND SHADER_FILES 
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
	"Resources/Shaders/SelfPractice/CMakeLists.txt")
