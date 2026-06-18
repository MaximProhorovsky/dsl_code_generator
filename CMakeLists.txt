cmake_minimum_required(VERSION 4.2.1)
project(dsl_codegen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml
    GIT_TAG v1.15
)
FetchContent_MakeAvailable(pugixml)

add_subdirectory(common)
add_subdirectory(app)
