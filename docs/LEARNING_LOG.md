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

- Change made: Added a second input point (0, 1, 0).
- Prediction: After rotation and translation, it should become (0, 2, 0).
- Observed result: The demo printed `second point in frame B: 0 2 0`, matching
  the prediction.
- Explanation: A positive 90-degree rotation around z maps `(0, 1, 0)` to
  `(-1, 0, 0)`. Adding translation `(1, 2, 0)` then produces `(0, 2, 0)`.
  Only `transform_demo.cpp` changed, so the existing build graph was still
  valid. The incremental build recompiled and relinked `transform_demo` but did
  not need to rebuild `transform_test`.
- Test coverage: Added known-answer checks for both transformed points. A
  temporary mutation from expected `(0, 2, 0)` to `(0, 3, 0)` produced the
  intended CTest failure and diagnostic. Restoring `(0, 2, 0)`, rebuilding, and
  rerunning CTest returned to 1/1 tests passed.

### M0 checkpoint answers

- Why did changing a C++ output string require build but not configure?
  - The project structure, source list, dependencies, libraries, and
    `CMakeLists.txt` did not change. The existing build graph remained valid, so
    only the modified source needed to be recompiled and relinked.
- Can `point_b = transform * point_a` alone tell us whether the operation is an
  active transformation or a passive coordinate change?
  - No. The expression does not say whether the physical point moves in a fixed
    frame or the same point is being represented in a different coordinate
    frame. We need an explicit frame convention, transform direction, variable
    naming, and physical interpretation.

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

## M1 reflection

### M1.0 frame notation

- What are the input and output frames of `T_BA`?
  - Learner answer: Input frame A; output frame B.
  - Review: correct. Under this repository's convention,
    `p_B = T_BA * p_A`.
- Why must `t_BA` be expressed in frame B?
  - Learner answer: Because the translation happens after the rotation into
    frame B.
  - Review: the operational intuition is correct for `R_BA * p_A + t_BA`. The
    more fundamental reason is that `R_BA * p_A` is already expressed using
    frame B's basis, and vector addition is meaningful only when `t_BA` uses
    that same basis.
- Why is `T_CA = T_CB * T_BA`?
  - Learner answer: The two transformations share the intermediate frame B.
  - Review: correct. `T_BA` first maps A to B, then `T_CB` maps B to C; the
    adjacent B labels connect and the resulting mapping is A to C.
- What is semantically broken in `T_BA * T_CB * p_A`?
  - Learner answer: There is no shared intermediate frame between the two
    transformations.
  - Review: correct. With right-to-left application, `T_CB` would first receive
    `p_A`, although it expects B-frame coordinates. Equivalently, the C output
    of `T_CB` does not match the A input expected by `T_BA`.

### M1.5 checkpoint questions

Answer these in your own words before M1 is marked complete. Concise answers
are enough; Chinese or English is acceptable.

1. Under this repository's convention, what does `T_BA` consume and produce?
   - Learner answer: Coodinate transformation from frame A to frame B
   - Review: essentially correct. More explicitly, `T_BA` consumes coordinates
     `p_A` expressed in frame A and produces coordinates `p_B` expressed in
     frame B: `p_B = T_BA * p_A`.
2. Why is `T_CA = T_CB * T_BA`, and which transform acts first?
   - Learner answer: The right one(T_BA) acts first.
   - Review: the application order is correct. The reason the composition is
     valid is that `T_BA` produces B-frame coordinates and `T_CB` consumes
     B-frame coordinates. The adjacent B frame connects, leaving an A-to-C
     mapping.
3. If `T_BA` maps A to B, what direction does its inverse map, and why does it
   recover the original point?
   - Learner answer: The inverse map transforms A to B. It recovers the recover point, because T inverse by T is the identity matrix.
   - Revised learner answer: `T_AB * T_BA = T_AA = I`; therefore the inverse
     maps B back to A and recovers the original A-frame point.
   - Review: the algebraic reason is correct, but the direction is reversed in
     the learner answer. `T_AB = inverse(T_BA)` maps B to A. Therefore
     `T_AB * (T_BA * p_A) = (T_AB * T_BA) * p_A = T_AA * p_A = p_A`.
4. Why can the same expression `T * p` describe either active motion or a
   passive coordinate change? What additional information removes the
   ambiguity?
   - Learner answer: We still don't know whether the world or the body coordinate moves.
   - Review: this identifies the missing physical interpretation but needs more
     precision. The numbers alone do not say whether the physical vector moves
     in one fixed frame (active) or one fixed physical vector is re-expressed
     in another frame (passive). Frame-labelled variable names, the declared
     transform direction, and a statement of what is physically fixed or
     moving remove the ambiguity.
5. Why can matrix dimensions and even numerical tests fail to detect a frame
   mismatch?
   - Learner answer: IDK
   - Revised learner idea: identity transforms associated with different
     frames can all have the same numerical matrix `I`, so an identity-based
     numerical check can hide a frame mismatch.
   - Review: A-frame and B-frame vectors are both represented by the same C++
     type and dimensions, such as `Eigen::Vector3d`. A semantically invalid
     multiplication can therefore compile. Tests can also miss the mistake if
     they encode the same wrong convention, cover too few cases, or use an
     identity transform whose numeric matrix hides incorrect frame labels.
6. Why must a rotation quaternion have unit norm, and why does its axis-angle
   conversion use `theta / 2`?
   - Learner answer: So that the conjugate equals the inverse; The maximal angle should not beyond 90 degrees
   - Revised learner answer: in the quaternion sandwich operation, a
     quaternion parameter angle `alpha` produces a physical rotation of
     `2 * alpha`, so a requested physical angle `theta` is encoded using
     `theta / 2`.
   - Review: unit norm does make the conjugate equal the inverse, but more
     fundamentally it restricts the quaternion to a pure rotation without an
     arbitrary scale. The half-angle is not imposed because the maximum angle
     must be 90 degrees. A vector is rotated by the sandwich operation
     `q * p * inverse(q)`; the quaternion parameterization that produces a
     physical rotation `theta` is `[cos(theta/2), axis*sin(theta/2)]`. This also
     leads to the double-cover property of unit quaternions over SO(3).
7. Why do `q` and `-q` represent the same rotation, and why is direct
   coefficient equality therefore unsafe?
   - Learner answer: (-q)p(-q) == qpq^(-1), the minus sign cancles; IDK
   - Revised learner answer: `q` and `-q` represent the same rotation, but a
     direct comparison of their coefficients reports them as different.
   - Review: the cancellation idea is correct, but the complete expression is
     `(-q) * p * inverse(-q) = q * p * inverse(q)`. Thus `q` and `-q` can have
     opposite coefficients while producing the same rotation. Direct
     coefficient equality would report them as different, so compare rotation
     matrices, angular distance, or the absolute dot product instead.
8. What is the difference between scaling all quaternion coefficients by two
   and composing the quaternion with itself?
   - Learner answer: The former one is just about the scale, the angle doesn't change, while the latter one represents two consequent rotations.
   - Review: correct. `2q` changes the quaternion norm and is no longer a unit
     rotation quaternion until normalized; `q * q` composes the rotation with
     itself and therefore accumulates the rotation angle according to the same
     axis and convention.
