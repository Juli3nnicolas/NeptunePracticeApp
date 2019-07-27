cmake_minimum_required (VERSION 3.12)

message("########## Running PostBuildOperations.cmake")

# Copy shared libraries
add_custom_command(TARGET NeptunePracticeApp POST_BUILD										# Adds a post-build event to Neptune
    COMMAND ${CMAKE_COMMAND} -E copy_directory												# which executes "cmake - E copy_if_different..."
    ${CMAKE_SOURCE_DIR}/../Neptune/Dependencies/Win32/Bin/X64                           	# where to get the dependencies from
    $<TARGET_FILE_DIR:NeptunePracticeApp>)															# where to copy them

add_custom_command(TARGET NeptunePracticeApp POST_BUILD										# Adds a post-build event to Neptune
    COMMAND ${CMAKE_COMMAND} -E copy_directory												# which executes "cmake - E copy_if_different..."
    ${CMAKE_SOURCE_DIR}/Libs/X64/Debug                           	# where to get the dependencies from
    $<TARGET_FILE_DIR:NeptunePracticeApp>)															# where to copy them
