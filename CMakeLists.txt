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
find_package(OpenCV 4 REQUIRED COMPONENTS core calib3d)

add_library(visual_perception_core INTERFACE)
target_include_directories(
    visual_perception_core
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(visual_perception_core INTERFACE Eigen3::Eigen)

add_executable(transform_demo app/transform_demo.cpp)
target_link_libraries(transform_demo PRIVATE Eigen3::Eigen)

add_executable(frame_composition_demo app/frame_composition_demo.cpp)
target_link_libraries(frame_composition_demo PRIVATE visual_perception_core)

add_executable(
    rotation_representations_demo
    app/rotation_representations_demo.cpp
)
target_link_libraries(rotation_representations_demo PRIVATE Eigen3::Eigen)

add_executable(pinhole_projection_demo app/pinhole_projection_demo.cpp)
target_link_libraries(pinhole_projection_demo PRIVATE visual_perception_core)

add_executable(
    world_to_camera_projection_demo
    app/world_to_camera_projection_demo.cpp
)
target_link_libraries(
    world_to_camera_projection_demo
    PRIVATE visual_perception_core
)

add_executable(radial_distortion_demo app/radial_distortion_demo.cpp)
target_link_libraries(radial_distortion_demo PRIVATE visual_perception_core)

add_executable(opencv_projection_check app/opencv_projection_check.cpp)
target_include_directories(opencv_projection_check PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(
    opencv_projection_check
    PRIVATE visual_perception_core ${OpenCV_LIBS}
)

add_executable(synthetic_calibration_demo app/synthetic_calibration_demo.cpp)
target_include_directories(synthetic_calibration_demo PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(synthetic_calibration_demo PRIVATE ${OpenCV_LIBS})

enable_testing()

add_executable(transform_test tests/transform_test.cpp)
target_link_libraries(transform_test PRIVATE Eigen3::Eigen)
add_test(NAME transform_test COMMAND transform_test)

add_executable(frame_composition_test tests/frame_composition_test.cpp)
target_link_libraries(frame_composition_test PRIVATE visual_perception_core)
add_test(NAME frame_composition_test COMMAND frame_composition_test)

add_executable(
    rotation_representations_test
    tests/rotation_representations_test.cpp
)
target_link_libraries(rotation_representations_test PRIVATE Eigen3::Eigen)
add_test(
    NAME rotation_representations_test
    COMMAND rotation_representations_test
)

add_executable(pinhole_projection_test tests/pinhole_projection_test.cpp)
target_link_libraries(pinhole_projection_test PRIVATE visual_perception_core)
add_test(NAME pinhole_projection_test COMMAND pinhole_projection_test)

add_executable(
    pinhole_back_projection_test
    tests/pinhole_back_projection_test.cpp
)
target_link_libraries(
    pinhole_back_projection_test
    PRIVATE visual_perception_core
)
add_test(
    NAME pinhole_back_projection_test
    COMMAND pinhole_back_projection_test
)

add_executable(
    intrinsic_extrinsic_projection_test
    tests/intrinsic_extrinsic_projection_test.cpp
)
target_link_libraries(
    intrinsic_extrinsic_projection_test
    PRIVATE visual_perception_core
)
add_test(
    NAME intrinsic_extrinsic_projection_test
    COMMAND intrinsic_extrinsic_projection_test
)

add_executable(radial_distortion_test tests/radial_distortion_test.cpp)
target_link_libraries(radial_distortion_test PRIVATE visual_perception_core)
add_test(NAME radial_distortion_test COMMAND radial_distortion_test)

add_executable(reprojection_error_test tests/reprojection_error_test.cpp)
target_link_libraries(reprojection_error_test PRIVATE visual_perception_core)
add_test(NAME reprojection_error_test COMMAND reprojection_error_test)
