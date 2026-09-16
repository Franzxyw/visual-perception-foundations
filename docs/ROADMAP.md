# Roadmap

The roadmap is ordered by dependency, not by prestige. A milestone is complete
only when its exit criteria have evidence in `PROGRESS.md`.

## M0 — Toolchain, CMake, Eigen, tests

Learn the compile/link/test loop and use Eigen to apply and invert a rigid
transform. Keep the program deliberately small.

Exit criteria:

- configure and build from a fresh terminal;
- run `transform_demo` and predict its output;
- run `ctest` successfully;
- explain active versus passive transformation in the learning log;
- make one learner-authored modification with a corresponding test.

## M1 — Frames, SO(3), and SE(3)

Implement and test frame composition, inverse transforms, rotation
representations, and convention-safe APIs. Connect the concepts to the IMU
frame ambiguity observed during the Ingenieurpraxis, without depending on that
project's code.

## M2 — Pinhole camera and calibration

Implement projection/back-projection, intrinsics, distortion, and reprojection
error. Validate numerically and visualize controlled examples.

## M3 — Features and two-view geometry

Use OpenCV for image loading, ORB features, matching, RANSAC, the Essential
matrix, pose recovery, and triangulation. Learn where library calls end and
geometric assumptions begin.

## M4 — Minimal two-frame visual odometry

Build an evaluated pipeline from image pair to relative pose. Record failure
cases instead of hiding them. Add a small public dataset only when M3 is sound.

## M5 — Directional branch

Choose based on the real thesis/initial-project opportunity:

- VIO/SLAM: nonlinear least squares, bundle adjustment, Ceres or GTSAM, then
  IMU preintegration;
- neural rendering/synthetic data: PyTorch refresh, 3D representation,
  rendering and GPU profiling;
- learned perception: modern features/depth/segmentation integrated into an
  evaluated geometry pipeline.

ROS 2, CUDA, TensorRT, and full SLAM frameworks are intentionally deferred
until a milestone creates a concrete need for them.

