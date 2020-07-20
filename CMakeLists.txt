

cmake_minimum_required(VERSION 3.4)

project(StringArt)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Add module path in case this is project root
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
endif()

find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED
    GL
    MeshTools
    Primitives
    SceneGraph
    Shaders
    GlfwApplication
    Trade
    )

include_directories (${StringArt_SOURCE_DIR}/src)
file(GLOB StringArt_SOURCES
    "src/*.h"
    "src/*.cpp"
)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)
corrade_add_resource(CubicBezierResources src/resources.conf)
add_executable(main  ${StringArt_SOURCES}
${CubicBezierResources})
target_link_libraries(main PRIVATE
    Corrade::Main
    Magnum::GlfwApplication
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Shaders
 )

