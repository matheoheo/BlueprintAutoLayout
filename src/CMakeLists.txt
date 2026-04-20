add_library(BlueprintAutoLayoutLib 
    BlueprintData.cpp
    BlueprintGraph.cpp
    GraphNode.cpp
    GraphLayout.cpp
    GraphRenderer.cpp
    Application.cpp
    GraphViewHandler.cpp)

target_link_libraries(BlueprintAutoLayoutLib PUBLIC 
        SFML::Graphics
        SFML::Window
        SFML::System)

target_compile_features(BlueprintAutoLayoutLib PUBLIC cxx_std_20)
target_include_directories(BlueprintAutoLayoutLib PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/dependencies)