cmake_minimum_required(VERSION 3.22)

project(
    visual_perception_foundations
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

add_executable(transform_demo app/transform_demo.cpp)
target_link_libraries(transform_demo PRIVATE Eigen3::Eigen)

enable_testing()

add_executable(transform_test tests/transform_test.cpp)
target_link_libraries(transform_test PRIVATE Eigen3::Eigen)
add_test(NAME transform_test COMMAND transform_test)

