# Visual Perception Foundations

A learning repository for building the C++ and geometric-computer-vision
foundation needed for visual odometry, SLAM, VIO, robotics perception, and
later learning-based vision work.

This is deliberately a small, cumulative project. Each milestone must leave a
runnable result, tests, and a short learning note before the next milestone
starts.

## Current status

- Completed milestones: **M0 — toolchain and first Eigen program**
  (2026-09-17) and **M1 — frames, SO(3), and SE(3)** (2026-09-18)
- Repository scaffold: created
- Environment: Ubuntu 24.04 WSL2 with GCC 13.3, CMake 3.28.3, and Eigen 3.4
- Build verification: the M1 and M2 examples were checked against hand
  calculations; the latest recorded full CTest run passed 8/8 tests
- Current main milestone: **M2 — pinhole camera and calibration**
- Parallel track: **ML0 — rebuild PyTorch and learning foundations**

Because the source repository is on a Windows-mounted drive, generated build
files live in the Linux filesystem:

```bash
cmake -S . -B ~/build/visual-perception-foundations
cmake --build ~/build/visual-perception-foundations
~/build/visual-perception-foundations/transform_demo
ctest --test-dir ~/build/visual-perception-foundations --output-on-failure
```

## Repository map

```text
include/visual_perception/  reusable C++ geometry and camera functions
app/                        small runnable examples and experiments
tests/                      executable checks registered with CTest
docs/ROADMAP.md      milestone sequence and scope
docs/PROGRESS.md     source of truth for current progress
docs/LEARNING_LOG.md short reflections written by the learner
docs/milestones/     step-by-step plan for each active milestone
docs/notes/          reusable concept notes and reference material
```

Use this route when looking for something: **header = implementation/API**,
**app = example that prints results**, **test = automatic correctness check**.
The CMake target names and source-file names are listed in `CMakeLists.txt`.

| Topic | Reusable code | Run an example | Check behavior |
| --- | --- | --- | --- |
| Rigid transforms and frame composition | `include/visual_perception/frame_transform.hpp` | `app/transform_demo.cpp`, `app/frame_composition_demo.cpp` | `tests/transform_test.cpp`, `tests/frame_composition_test.cpp` |
| Rotation representations | Eigen types used directly | `app/rotation_representations_demo.cpp` | `tests/rotation_representations_test.cpp` |
| Pinhole projection and back-projection | `include/visual_perception/pinhole_camera.hpp` | `app/pinhole_projection_demo.cpp`, `app/world_to_camera_projection_demo.cpp` | `tests/pinhole_projection_test.cpp`, `tests/pinhole_back_projection_test.cpp`, `tests/intrinsic_extrinsic_projection_test.cpp` |
| Radial distortion | `include/visual_perception/radial_distortion.hpp` | `app/radial_distortion_demo.cpp` | `tests/radial_distortion_test.cpp` |
| Reprojection residual, error, and RMSE | `include/visual_perception/reprojection_error.hpp` | `app/world_to_camera_projection_demo.cpp` | `tests/reprojection_error_test.cpp` |
| OpenCV comparison and synthetic calibration | OpenCV APIs used by the experiments | `app/opencv_projection_check.cpp`, `app/synthetic_calibration_demo.cpp` | Run these executables directly; the calibration experiment is not a CTest target |

For one API, start at its header and read only the matching example and test;
there is no need to keep every file in your head at once. For the exact current
milestone and verification evidence, use `docs/PROGRESS.md` rather than this
navigation page.

## Working agreement

For every step:

1. Read the goal and predict what should happen.
2. Run or change one small thing yourself.
3. Compare the result with the stated success criterion.
4. Explain the result in your own words in `docs/LEARNING_LOG.md`.
5. Update `docs/PROGRESS.md` only with evidence.

The repository should not claim that something works merely because the files
exist. "Written", "built", "tested", and "understood" are separate states.
