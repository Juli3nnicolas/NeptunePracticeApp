# Scripts that creates the whole project's folder structure.
# It is representative to where (the folders) code files are stored 
# and keeps the same structure.

cmake_minimum_required (VERSION 3.12)

source_group("CMake" REGULAR_EXPRESSION "CMake/+\\w*")
source_group("src" REGULAR_EXPRESSION "src/+\\w*")
source_group("Resources" REGULAR_EXPRESSION "Resources/+\\w*")
	source_group("Resources\\Shaders" REGULAR_EXPRESSION "Resources/+Shaders/+\\w*")
		source_group("Resources\\Shaders\\SelfPractice" REGULAR_EXPRESSION "Resources/+Shaders/+SelfPractice/+\\w*")
