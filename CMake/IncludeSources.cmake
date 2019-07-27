cmake_minimum_required (VERSION 3.12)

message("########## Running Cmake/IncludeSources.cmake")

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