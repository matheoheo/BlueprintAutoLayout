cmake_minimum_required(VERSION 3.22)
project(BlueprintAutoLayoutProject
    DESCRIPTION "Blueprint Auto Layout"
    LANGUAGES CXX
    VERSION 1.0)

add_subdirectory(dependencies)
add_subdirectory(src)

add_executable(BlueprintAutoLayout src/main.cpp)
target_link_libraries(BlueprintAutoLayout BlueprintAutoLayoutLib)

#Copying 'assets' folder
add_custom_command(TARGET BlueprintAutoLayout POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:BlueprintAutoLayout>/assets"
    COMMENT "Copying assets folder..")