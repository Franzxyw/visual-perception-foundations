# Progress

Last updated: 2026-09-18

## Status legend

- `[ ]` not started
- `[~]` in progress or written but not verified
- `[x]` verified with evidence

## Current milestone: M0

Status: **complete on 2026-09-17**

- `[x]` Local Git repository created on branch `main`.
- `[x]` Learning roadmap and progress-check structure created.
- `[x]` Initial concept answers reviewed and supplemented in `LEARNING_LOG.md`.
- `[x]` Minimal CMake/Eigen demo and executable test written and verified.
- `[x]` Ubuntu 24.04 WSL2 toolchain selected and installed.
- `[x]` CMake configure succeeds with a Linux-native build directory.
- `[x]` Project builds without warnings/errors.
- `[x]` `transform_demo` output checked against a hand calculation.
- `[x]` CTest passes 1/1 registered tests.
- `[x]` Learner added a second transform example and known-answer tests.
- `[x]` M0 reflection completed in `LEARNING_LOG.md`.

## Current milestone: M1

Status: **complete on 2026-09-18**

- `[x]` M1 step-by-step plan and repository frame convention documented.
- `[x]` M1.0 three-frame composition derived by hand and reviewed.
- `[x]` M1.1 composition demo implemented and verified.
- `[x]` M1.2 composition/inverse/identity tests and wrong-order mutation completed.
- `[x]` M1.3 repeated checks refactored into table-driven cases.
- `[x]` M1.4 matrix/angle-axis/quaternion equivalence verified.
- `[x]` M1.5 reflection completed and learner answers reviewed.

## Current milestone: M2

Status: **in progress from 2026-09-18**

- `[x]` M2 step-by-step plan and initial camera convention documented.
- `[x]` M2.0 projection reasoning completed and reviewed.
- `[x]` M2.1 minimal pinhole projection API built and verified on the hand-calculated example.
- `[x]` M2.2 projection and invalid-input tests plus mutation check completed.
- `[x]` M2.3 back-projection, z-depth reasoning, and round-trip tests completed.
- `[x]` M2.4 intrinsic/extrinsic matrix equivalence, frame composition, and
  direction mutation completed.
- `[x]` M2.5 radial-distortion model, demo, tests, and sign mutation verified.
- `[x]` M2.6 reprojection-error utilities, OpenCV projection comparison,
  six-view synthetic correspondence generation, parameter/pose recovery, and
  a controlled strong-versus-weak pose calibration comparison verified.

## Evidence log

### 2026-09-23 — M2.5 and M2.6 evidence

- Changing the radial test's `k_1` from `-0.2` to `0.2` caused the intended
  `known radial-distortion result was incorrect` failure. Restoring `-0.2`
  returned the targeted test and the full 7/7 CTest run to passing.
- `reprojection_error_test` covers known residual `(-3, 4)`, error `5 px`,
  two-point RMSE `sqrt(15) px`, empty input, and mismatched point counts.
  The world-to-camera demo produced pixel `(360, 282)` and residual `(-3, 4)`
  against observed pixel `(363, 278)`, with error `5 px`.
- Ubuntu WSL OpenCV 4.6.0 was found. `opencv_projection_check` built and ran:
  the hand-calculated, Eigen, and OpenCV paths all projected
  `p_C = (1, 0, 2)` with `k_1 = -0.2` to pixel `(510, 240)`.
  Its two errors against the known pixel were `0 px`.
- A fresh full build and CTest run passed 8/8 tests after adding the OpenCV
  comparison target. Multi-view calibration had not yet been implemented at
  that stage.
- A 7-by-6 planar inner-corner grid with 0.04 m spacing produced 42 points per
  view. Six distinct board poses generated 252 matching 3D/2D pairs; every
  corner had positive camera-frame depth and projected inside the 640-by-480
  image. The first pixel in view 0 matched the independently hand-calculated
  `(260.4575, 187.9003125)` within 0.001 px. The synthetic demo built and ran,
  and the full CTest suite passed 8/8. Parameter estimation had not yet been run
  at that stage.
- OpenCV calibration of those 252 correspondences recovered intrinsics within
  `0.0001 px` and `k_1` within `4.2e-7` of the synthetic truth. Recomputed
  reprojection RMS matched OpenCV's `9.81443e-06 px` at displayed precision.
  Comparing each estimated board-to-camera pose with its generating pose gave
  rotation errors from `9.11207e-06` to `2.61297e-05` degrees and translation
  errors from `2.03455e-07` to `3.4921e-07` metres. The demo built and ran in
  Ubuntu WSL; the existing CTest suite passed 8/8. The demo is not yet a CTest
  target, and weak/degenerate data have not been examined.
- The strong-pose baseline now adds deterministic uniform pixel perturbations
  bounded by `+/-0.2 px` independently in `u` and `v`, using seed `20260923`.
  The ideal first-point hand check still runs before perturbation. Calibration
  on the perturbed correspondences returned `0.156565 px` reprojection RMS,
  `f_x` error `-3.27441 px`, and `f_y` error `-3.33981 px`; the recomputed RMS
  agreed within `2.8e-17 px`. The demo built and ran, and CTest passed 8/8.
- In a paired weak-pose case, all six generating rotation vectors were scaled
  by `0.01`; board points, translations, camera truth, image size, and each
  corner's stored pixel perturbation were unchanged. The first observed pixel
  in every view matched the strong-pose case, as expected for the board origin.
  Weak-pose calibration returned RMS `0.154715 px` versus `0.156565 px` for the
  strong-pose case, but absolute `f_x` error rose from `3.27441 px` to
  `47.7667 px`. Weak-pose translation errors were `0.09566`–`0.144554 m`,
  versus `0.00478962`–`0.00829375 m` for the strong-pose case. Thus a slightly
  lower in-sample reprojection RMS did not imply more accurate recovered
  parameters. A full WSL build and the existing CTest suite passed 8/8; this
  demo itself is not yet registered as a CTest test.

### 2026-09-18 — M2 ideal projection evidence

- After configuring the new CMake target, `pinhole_projection_demo` built and
  ran successfully.
- For `p_C = (0.5, -0.2, 2.0)` and intrinsics
  `(f_x, f_y, c_x, c_y) = (400, 420, 320, 240)`, the demo printed pixel
  `(420, 198)`, matching the corrected hand calculation.
- Scaling the point to `(1.5, -0.6, 6.0)` left the projected pixel unchanged at
  `(420, 198)`, verifying ray-scale invariance in the demo.
- Invalid-depth behavior was then covered by the automated test.
- A full CTest run passed 4/4 tests, including `pinhole_projection_test`.
  The new test verifies three known projections, ray-scale invariance, and
  rejection of both zero and negative depth.
- Mutation check: changing the known expected pixel from `(420, 198)` to
  `(420, 199)` caused `pinhole_projection_test` to fail with
  `known projection projected to the wrong pixel`, while the three earlier
  tests still passed. The expected value was then restored to `(420, 198)`.
- After restoration, two observed full CTest runs passed 4/4 tests.
- `pinhole_projection_demo` then printed normalized ray
  `(0.25, -0.1, 1)` and recovered the original point
  `(0.5, -0.2, 2.0)` using its z-depth.
- `pinhole_back_projection_test` verifies the known ray, known 3D point,
  project/back-project round trip, and rejection of zero or negative z-depth.
- The configure/build completed successfully and the full CTest run passed
  5/5 tests.
- The world-to-camera demo produced `p_C = (0.5, 0.5, 5)` and pixel
  `(360, 282)` through both the scalar `project()` path and the homogeneous
  `K * p_C` path.
- `intrinsic_extrinsic_projection_test` passed, including the known extrinsic,
  known pixel, matrix/scalar agreement, and inverse recovery checks. The full
  CTest run passed 6/6 tests.
- Frame-direction mutation: replacing `T_CW * p_W` with `T_WC * p_W` produced
  the intended `T_CW produced the wrong camera-frame point` failure. A
  subsequent transient edit state produced
  `inverse extrinsic did not recover the world point`; its exact source diff
  was not captured. Restoring
  `p_C = T_CW * p_W` and `p_W = inverse(T_CW) * p_C` returned the targeted
  test to passing.
- The radial-distortion demo produced ideal normalized coordinate `(0.5, 0)`,
  distorted normalized coordinate `(0.475, 0)`, ideal pixel `(520, 240)`, and
  distorted pixel `(510, 240)` for `k_1 = -0.2`.
- The new radial-distortion test and all six earlier tests passed; the observed
  full CTest result was 7/7 passing.

### 2026-09-16 — Initial audit

- Windows PATH: Git available; CMake and C++ compilers not found.
- WSL: only the internal `docker-desktop` distribution was listed; no Ubuntu
  development distribution was available.
- Docker CLI: not found on Windows PATH or at the standard Docker Desktop path.
- Consequence: source files exist, but no build/test success is claimed yet.

### 2026-09-17 — M0 build and test evidence

- Installed Ubuntu 24.04.5 LTS as WSL2 at `D:\WSL\Ubuntu-24.04`.
- Verified GCC/G++ 13.3.0, CMake 3.28.3, Git 2.43.0, and Eigen 3.4.0.
- Configuring into `/mnt/d/.../build` failed with `Operation not permitted`
  because the Windows-mounted source tree uses DrvFS/9p ownership and permission
  semantics.
- Kept source at `/mnt/d/workspace/visual-perception-foundations` and configured
  successfully into `/home/franzxyw/build/visual-perception-foundations`.
- `cmake --build` built both `transform_demo` and `transform_test`.
- `transform_demo` printed `(1, 0, 0)` and `(1, 3, 0)`, matching the hand
  calculation.
- CTest result: `100% tests passed, 0 tests failed out of 1`.
- Mutation check: changing the second expected point from `(0, 2, 0)` to
  `(0, 3, 0)` caused the intended failure; restoring it returned CTest to 1/1
  passing.
- Final checkpoint: changing only a C++ output string triggered an incremental
  rebuild of `transform_test`; the direct executable printed
  `all transform checks passed`, and CTest remained 1/1 passing.

### 2026-09-18 — M1 frames and rotation evidence

- `frame_composition_demo` produced `p_B = (1, 1, 0)` and matching sequential
  and composed `p_C = (-1, 3, 0)` results.
- Composition tests cover the known answer, sequential/composed equivalence,
  inverse recovery, and correctly labelled left/right identity composition.
- Reversing the order to `T_BA * T_CB` caused the intended composition-test
  failure; restoring `T_CB * T_BA` returned CTest to 2/2 passing.
- Repeated point cases in `transform_test` were replaced by a named
  `std::array` of test-case structs and a range-based loop. Mutating the
  y-axis expected result produced the intended named failure; after restoring
  it, the later full CTest run passed.
- Shared rigid-transform construction was extracted to the header-only
  `visual_perception_core` interface target and consumed by both the demo and
  composition test.
- The rotation-representation demo showed equivalent matrix, angle-axis, and
  quaternion results for a +90-degree z rotation. The quaternion printed as
  approximately `(w, x, y, z) = (0.707107, 0, 0, 0.707107)`.
- `rotation_representations_test` verifies equivalent matrices and transformed
  vectors, quaternion double cover (`q` and `-q`), normalization, and the
  half-angle coefficients.
- Final observed CTest result before the reflection: `100% tests passed,
  0 tests failed out of 3`.
- The learner completed the M1 checkpoint explanations covering transform
  direction and inverse recovery, composition order, active/passive ambiguity,
  frame mismatches hidden by shared numeric types or identity matrices,
  quaternion unit norm and half angles, double cover, and composition versus
  coefficient scaling.

## Checkpoint rule

At each checkpoint, record:

1. exact command run;
2. important output or error;
3. what the result proves;
4. the next smallest step.
