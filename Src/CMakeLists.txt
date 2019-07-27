cmake_minimum_required (VERSION 3.12)

message("########## Executing Src/CMakeLists.txt")

# List header files, they must be #includable by users
list(APPEND HEADERS 
	"Src/FactoryExamples.h"
	"Src/InputSystemExamples.h"
	"Src/Mandelbrot.h"
	"Src/SimpleAppExamples.h"
	"Src/ViewSpawnerExamples.h"
)

# List cpp files, useful for compilation purpose only
list(APPEND CPP_FILES
    "Src/main.cpp"
    "Src/FactoryExamples.cpp"
	"Src/InputSystemExamples.cpp"
	"Src/Mandelbrot.cpp"
	"Src/SimpleAppExamples.cpp"
	"Src/ViewSpawnerExamples.cpp"
)

# Add source files
target_sources(NeptunePracticeApp
PUBLIC
	${HEADERS}
PRIVATE
	${CPP_FILES}
	"Src/CMakeLists.txt")
