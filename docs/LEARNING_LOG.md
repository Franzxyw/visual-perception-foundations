# Learning Log

Write short answers in your own words. Imperfect explanations are useful: they
show what we should revisit.

## M0 reflection

### Before running the program

- What do you expect `transform_demo` to print, and why?
  - Vectors respectively in the original frame and the transformed frame.
  - Review: correct direction, but make the prediction numerical. It should
    print `point_a = (1, 0, 0)` and approximately `point_b = (1, 3, 0)` because
    the point is first rotated 90 degrees around z to `(0, 1, 0)` and then
    translated by `(1, 2, 0)`.
- In `T * p`, what frames do you think `T` maps between?
  - The original frame and the transformed frame?
  - Review: that is the intended idea. If we define `p_B = T_BA * p_A`, then
    `T_BA` maps coordinates expressed in frame A to coordinates expressed in
    frame B. The current variable name `transform` does not enforce or clearly
    document this convention, which is why frame-aware naming matters.
- What work is done by CMake, and what work is done by the compiler?
  - CMake tells the general information of this project, while the compiler
    checks the syntax and translates C++ code to machine code.
  - Review: correct. More precisely, CMake reads the project description and
    generates build rules: targets, source files, dependencies, compiler
    settings, and tests. The compiler checks and translates each C++ source
    file. A linker then combines compiled objects and libraries into an
    executable.

### After building and testing

- What changed between configure, build, and test?
  - Supplement: configure checks the project description, compiler, and
    dependencies and generates build files. Build compiles and links the
    targets. Test runs the registered test executables and reports pass/fail.
    This answer is conceptual only; we have not run these stages yet.
- Why should applying `T.inverse()` recover the original point?
  - Because the inverse transformation restores what the forward transformation
    does.
  - Review: correct. Algebraically, `T.inverse() * (T * p) = p`, up to floating
    point error. For `p_B = R * p_A + t`, the inverse is
    `p_A = R.transpose() * (p_B - t)`.
- Which part of the code would become dangerous if frame names were ambiguous?
  - `T_body_to_world * v_world`
  - Review: exactly the right kind of bug. If `T_body_to_world` expects a vector
    expressed in the body frame, its input should be `v_body`, not `v_world`.
    The dimensions still match, so ordinary C++ types cannot detect the semantic
    error.

### Learner-authored change

- Change made:
- Prediction:
- Observed result:
- Explanation:

Not attempted yet. Complete this only after the project can be built and the
baseline test has been run; do not invent an observed result.

## Concept follow-up from review

### How much CMake should I learn?

For now, be able to read and write a small target using
`add_executable`, `target_link_libraries`, `find_package`, and `add_test`. You do
not need advanced CMake. Python often hides this stage because its interpreter
runs source directly and package tools manage many details; compiled C++ must
make the build graph explicit.

### What is `kPi / 2.0`?

`kPi` is a `double` constant defined earlier as pi. Angles passed to Eigen are
in radians, so `kPi / 2.0` is pi/2 radians, or 90 degrees. The `.0` makes the
literal visibly floating point; because `kPi` is already a `double`, `2` would
also produce floating-point division here.

### Where does the z-rotation matrix come from?

It is not typed directly into the program. `Eigen::AngleAxisd` stores the axis
and angle, and `rotation.toRotationMatrix()` computes
`[[cos(theta), -sin(theta), 0], [sin(theta), cos(theta), 0], [0, 0, 1]]`.
For `theta = pi/2`, cosine is 0 and sine is 1, producing the matrix shown in the
lesson (up to tiny floating-point residuals).

### Is a homogeneous point a quaternion?

No. Both happen to contain four numbers, but they represent different things.
A homogeneous point `[x, y, z, 1]` lets a 4x4 transform apply rotation and
translation. A homogeneous direction uses `[x, y, z, 0]`, so translation does
not affect it. A quaternion, commonly `[w, x, y, z]`, represents rotation and
obeys different algebra.

### Why not use `approx()` or `allclose()`?

Those names are familiar from test frameworks or NumPy, but they are not free
functions in standard C++. Eigen does provide `actual.isApprox(expected,
tolerance)`. The small helper in this repository deliberately uses the absolute
Euclidean error `(actual - expected).norm() < tolerance`, making the chosen
criterion visible. Later a C++ test framework can provide clearer assertions.

### Why does this test look unlike pytest?

Pytest is both a test framework and runner: it discovers Python functions,
handles assertions, and formats failures. Here, `transform_test` is a plain C++
executable that manually returns zero for success and nonzero for failure.
CTest only launches registered executables and collects their results; it is
not an assertion framework. We can introduce Catch2 or GoogleTest later, after
the basic compile-link-run model is clear.

### What is the main danger in `T * p`?

The main danger here is semantic frame mismatch, for example applying a
body-to-world transform to a vector already expressed in the world frame. Poor
conditioning is important in numerical estimation and optimization, but an
ideal rotation matrix itself has condition number 1. It is not the primary
problem in this simple rigid-transform example.

### Why avoid direct floating-point equality?

The learner answer "rounding error" is correct. Many real values cannot be
represented exactly in binary floating point, and arithmetic accumulates small
errors, so numerical code compares against an explicit tolerance.
