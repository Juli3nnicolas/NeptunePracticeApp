cmake_minimum_required(VERSION 3.12)

message("########## Executing CMake/AddLibraries.cmake")

# Add include directories
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Dependencies/All)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Dependencies/Win32)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Win32/HighLevel)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Win32/Core)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Multiplatform/HighLevel)
target_include_directories(NeptunePracticeApp PUBLIC ${CMAKE_SOURCE_DIR}/Libs/Include/Neptune/Engine/Multiplatform/Core)

# Add libraries
target_link_libraries(NeptunePracticeApp PUBLIC opengl32)

find_library(GLEW "glew32" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp PUBLIC ${GLEW})

find_library(SDL2 "SDL2"  ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp PUBLIC ${SDL2})

find_library(Neptune "Neptune" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp PUBLIC ${Neptune})

find_library(LIB_KTX "libktx.gld" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp PUBLIC ${LIB_KTX})

find_library(ASSIMP "assimp" ${CMAKE_SOURCE_DIR}/Libs/X64/Debug)
target_link_libraries(NeptunePracticeApp PUBLIC ${ASSIMP})
