# M0 — Toolchain and First Eigen Program

## Purpose

Establish a repeatable C++ learning loop and understand one rigid transform.
The goal is not to rush toward SLAM; it is to make every later geometric result
buildable, testable, and explainable.

## Step 0 — Inspect the repository

Read, but do not edit yet:

1. `CMakeLists.txt`
2. `app/transform_demo.cpp`
3. `tests/transform_test.cpp`

Answer the three "Before running" questions in `LEARNING_LOG.md`.

Success criterion: you can point to the executable target, its dependency, and
the test registration in `CMakeLists.txt`.

## Step 1 — Choose the development environment

Current evidence shows no usable compiler/CMake on Windows and no Ubuntu WSL.
The recommended next action is to install an Ubuntu WSL distribution and keep
the learning toolchain there. Do not install packages until we review the exact
commands together.

Why WSL Ubuntu is the default recommendation:

- close to common robotics/CV Linux environments;
- simpler package installation for Eigen and OpenCV;
- avoids coupling this repository to the Aoki Docker images;
- prepares for later ROS 2 work without requiring ROS 2 now.

Success criterion: a fresh Ubuntu terminal reports versions for `git`, `g++`,
and `cmake`.

## Step 2 — Install only M0 dependencies

Required packages will be limited to:

- a C++ compiler and standard build tools;
- CMake;
- Eigen development headers.

OpenCV is deliberately deferred to M2/M3.

Success criterion: Eigen is discoverable by CMake without manually copying
headers into this repository.

## Step 3 — Configure

From the repository root, the intended command is:

```bash
cmake -S . -B build
```

Meaning:

- `-S .` selects the current directory as the source tree;
- `-B build` keeps generated build files in a separate directory.

Success criterion: CMake finishes with "Configuring done" and "Generating
done". Record the compiler CMake selected.

## Step 4 — Build

```bash
cmake --build build
```

Success criterion: both `transform_demo` and `transform_test` are built. If the
build fails, preserve the first meaningful compiler error before changing code.

## Step 5 — Predict, then run

Before execution, calculate the expected transformed point by hand. Then run
the program from the build directory and compare.

Success criterion: the numerical result agrees with your calculation, and you
can explain whether rotation or translation is applied first by this expression.

## Step 6 — Test

```bash
ctest --test-dir build --output-on-failure
```

Success criterion: the inverse-transform test passes, and you can explain why
this test would still be incomplete for a production geometry library.

## Step 7 — Make it yours

Change one transform and add one assertion that would fail if your predicted
frame convention were wrong. Do this yourself before asking Codex to review it.

Success criterion: you first observe the test fail for the intended reason,
then make it pass and document the reasoning in `LEARNING_LOG.md`.

## M0 checkpoint

We review:

- build/test evidence;
- your explanation of the transform;
- your learner-authored change;
- whether the scope is ready for M1.

Do not start M1 solely because the initial sample compiled.

