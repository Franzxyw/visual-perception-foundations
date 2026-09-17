# M0 — Toolchain and First Eigen Program

## Purpose

Establish a repeatable C++ learning loop and understand one rigid transform.
The goal is not to rush toward SLAM; it is to make every later geometric result
buildable, testable, and explainable.

## Step 0 — Build the mental model first

Do not begin by reading every line alone. Use this four-part model:

1. A `.cpp` file is human-readable C++ source code; the computer cannot run it
   directly.
2. A compiler translates that source into machine code and reports language
   errors.
3. CMake reads `CMakeLists.txt` and generates instructions for the actual build
   tool. CMake is not the compiler.
4. A test is another executable whose result is interpreted as pass or fail.

Now inspect the repository with the following guide.

### `CMakeLists.txt`

- `project(... LANGUAGES CXX)` declares a C++ project.
- `set(CMAKE_CXX_STANDARD 17)` requests the C++17 language standard.
- `find_package(Eigen3 ...)` asks the system to locate Eigen rather than copying
  it into this repository.
- `add_executable(transform_demo ...)` says which source file becomes the demo
  program.
- `target_link_libraries(... Eigen3::Eigen)` gives that program access to Eigen.
- `enable_testing()`, the second executable, and `add_test(...)` register a
  program that CTest can run as a check.

### `app/transform_demo.cpp`

The program creates:

- a 90-degree rotation around the z axis;
- a translation `(1, 2, 0)`;
- a point `(1, 0, 0)`.

Under the convention used by the sample, rotation is applied first and
translation second:

```text
(1, 0, 0) --rotate 90 degrees around z--> (0, 1, 0)
          --add translation (1, 2, 0)--> (1, 3, 0)
```

The useful beginner question is not "do I know Eigen syntax by memory?" It is
"can I state what operation the code represents and predict its result?"

### `tests/transform_test.cpp`

The test applies a transform and then its inverse. If the recovered point is
not sufficiently close to the original point, it returns a failure code.
CTest treats exit code zero as pass and a nonzero exit code as fail.

After this guided pass, answer the three "Before running" questions in
`LEARNING_LOG.md`. Short and uncertain answers are acceptable; they establish a
baseline for comparison after the program runs.

Success criterion: in plain language, you can distinguish source code,
compiler, CMake, executable, and test, and predict the demo result `(1, 3, 0)`.

## Step 1 — Choose the development environment

Ubuntu 24.04 WSL2 is the selected development environment. It is stored on the
D drive and provides GCC/G++, CMake, Git, and Eigen without modifying the Aoki
project environments.

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

The source repository is on the Windows-mounted D drive. CMake cannot reliably
apply Linux permission metadata to a build directory there, so generated build
files belong in the Linux filesystem. From the repository root, use:

```bash
cmake -S . -B ~/build/visual-perception-foundations
```

Meaning:

- `-S .` selects the current directory as the source tree;
- `-B ~/build/visual-perception-foundations` keeps generated build files in a
  separate Linux-native directory.

Success criterion: CMake finishes with "Configuring done" and "Generating
done". Record the compiler CMake selected.

## Step 4 — Build

```bash
cmake --build ~/build/visual-perception-foundations
```

Success criterion: both `transform_demo` and `transform_test` are built. If the
build fails, preserve the first meaningful compiler error before changing code.

## Step 5 — Predict, then run

Before execution, calculate the expected transformed point by hand. Then run:

```bash
~/build/visual-perception-foundations/transform_demo
```

Success criterion: the numerical result agrees with your calculation, and you
can explain whether rotation or translation is applied first by this expression.

## Step 6 — Test

```bash
ctest --test-dir ~/build/visual-perception-foundations --output-on-failure
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

## GitHub publication checkpoint

The local Git history preserves commit dates even if it is pushed later. Create
the GitHub repository after M0 builds and tests successfully, then push the
complete history. This gives the public repository an honest runnable first
state without losing evidence of the earlier work.
