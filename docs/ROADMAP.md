# Roadmap

The roadmap is ordered by dependency, not by prestige. A milestone is complete
only when its exit criteria have evidence in `PROGRESS.md`.

The plan is intentionally supervisor-independent. It prepares for computer
vision, robot perception, and learning-based vision without assuming that a
specific thesis opening or supervisor will remain available. Geometry and C++
form the main dependency chain; a bounded PyTorch track starts after M1 and
runs alongside the vision milestones.

## M0 — Toolchain, CMake, Eigen, tests

Status: **completed 2026-09-17**

Learn the compile/link/test loop and use Eigen to apply and invert a rigid
transform. Keep the program deliberately small.

Exit criteria:

- configure and build from a fresh terminal;
- run `transform_demo` and predict its output;
- run `ctest` successfully;
- explain active versus passive transformation in the learning log;
- make one learner-authored modification with a corresponding test.

## M1 — Frames, SO(3), and SE(3)

Status: **completed 2026-09-18**

Implement and test frame composition, inverse transforms, rotation
representations, and convention-safe APIs. Connect the concepts to the IMU
frame ambiguity observed during the Ingenieurpraxis, without depending on that
project's code.

## M2 — Pinhole camera and calibration

Status: **in progress from 2026-09-18**

Implement projection/back-projection, intrinsics, distortion, and reprojection
error. Validate numerically and visualize controlled examples.

## Parallel ML0 — Rebuild PyTorch and learning foundations

Start after the M1 checkpoint and keep this track to roughly 20–30% of the
available study time while M2–M4 remain the main line. Assume no reliable
operational recall from earlier ML work: rebuild tensors, shapes, dtypes/devices,
autograd, `Dataset`/`DataLoader`, model/loss/optimizer, train versus evaluation
mode, data splits, metrics, checkpoints, and reproducibility from first
principles. Prior exposure may make the material return faster, but it is not a
prerequisite for any exercise.

Build one deliberately small image training pipeline. The objective is not a
large portfolio model; it is to regain the ability to train, diagnose, evaluate,
and reproduce a model without hiding the process inside a notebook.

Exit criteria:

- a small model trains and evaluates from scripts or clear entry points;
- validation and test data are separated from training data;
- a saved checkpoint can be loaded for inference;
- metrics and at least one failure mode are documented;
- the learner can explain autograd, train/eval mode, and overfitting.

## M3 — Features and two-view geometry

Use OpenCV for image loading, ORB features, matching, RANSAC, the Essential
matrix, pose recovery, and triangulation. Learn where library calls end and
geometric assumptions begin.

After the classical pipeline is working and evaluated, replace one component
with a pretrained learned alternative, such as a feature/matching or depth
front-end. Keep the same geometric checks and compare behavior rather than
treating the learned model as an opaque demo.

## M4 — Minimal two-frame visual odometry

Build an evaluated pipeline from image pair to relative pose. Record failure
cases instead of hiding them. Add a small public dataset only when M3 is sound.

Keep the classical baseline intact. A learned component may be plugged in only
after the baseline provides a measurable comparison.

## M5 — Robot perception foundations

Extend from image geometry to time-aware, multi-sensor perception. Cover the
parts that transfer across robotics, automotive, and XR systems:

- camera/IMU/LiDAR frames, extrinsics, timestamps, and synchronization;
- basic IMU and state-estimation concepts, including bias and uncertainty;
- point-cloud representation and one small geometric operation;
- data logging, evaluation, latency, and failure analysis;
- minimal ROS 2 dataflow only when a concrete exercise needs it.

The milestone should integrate at least two pieces of information, such as
camera plus IMU or camera plus depth, rather than becoming a survey of many
frameworks.

## M6 — Directional branch

Choose based on the real thesis/initial-project opportunity:

- VIO/SLAM: nonlinear least squares, bundle adjustment, Ceres or GTSAM, then
  IMU preintegration;
- neural rendering/synthetic data: PyTorch refresh, 3D representation,
  rendering and GPU profiling;
- learned 2D/3D perception: modern depth, segmentation, detection, tracking, or
  multimodal fusion integrated into an evaluated perception pipeline;
- perception systems: ROS 2 integration, profiling, embedded deployment, and
  end-to-end latency work.

CUDA, TensorRT, advanced ROS 2 work, and full SLAM frameworks are intentionally
deferred until a milestone or real project creates a concrete need for them.
