message(STATUS "Fetching SFML")

include(FetchContent)

FetchContent_Declare(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 0fa201c969e48ecc253581c5841ce73f44d42f49 #3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

set(SFML_BUILD_AUDIO false)
set(SFML_BUILD_NETWORK false)

FetchContent_MakeAvailable(sfml)