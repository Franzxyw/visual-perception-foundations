# M1 — Frames, SO(3), and SE(3)

## Purpose

Learn to reason about rigid transforms by frame semantics rather than by matrix
dimensions alone. The milestone moves from one isolated transform to chains of
camera, body, IMU, and world frames while keeping every direction explicit and
tested.

## Convention used in this repository

We write:

```text
p_B = T_BA * p_A
```

where:

- `p_A` is the coordinate representation of a physical point in frame A;
- `T_BA` maps coordinates from frame A into frame B;
- `p_B` is the same physical point represented in frame B.

The left subscript is the output/destination frame; the right subscript is the
input/source frame.

Composition follows cancellation of adjacent frame labels:

```text
p_C = T_CB * p_B
    = T_CB * T_BA * p_A
    = T_CA * p_A

T_CA = T_CB * T_BA
```

The inverse reverses the direction:

```text
T_AB = inverse(T_BA)
```

This is a repository convention, not a claim that every textbook or codebase
uses identical subscript ordering. Always inspect the local convention.

## M1.0 — Frame notation before code

Learn to identify:

- the physical point versus its coordinates;
- source and destination frames;
- valid transform-point multiplication;
- valid transform composition;
- why the rightmost transform acts first.

Exercise:

```text
p_A = (1, 0, 0)

T_BA:
  rotate +90 degrees around z
  then translate by (1, 0, 0), expressed in B

T_CB:
  rotate +90 degrees around z
  then translate by (0, 2, 0), expressed in C
```

Predict `p_B`, `p_C`, and the correct expression for `T_CA` before writing C++.

Success criterion: derive `p_B = (1, 1, 0)`, `p_C = (-1, 3, 0)`, and explain why
`T_CA = T_CB * T_BA`, not the reverse order.

## M1.1 — Composition demo

Add a new C++ executable that constructs `T_BA` and `T_CB`, applies them
sequentially, composes `T_CA`, and verifies that both paths produce the same
`p_C`.

Success criterion: sequential and composed results agree within a numerical
tolerance, and variable names expose every frame direction.

## M1.2 — Composition and inverse tests

Add automated known-answer and invariant checks:

- sequential application equals composed application;
- `T_AB * (T_BA * p_A)` recovers `p_A`;
- identity composition behaves as expected;
- a deliberately reversed multiplication order fails a known-answer check.

Success criterion: observe the intended failure for the wrong order, restore
the correct expression, rebuild, and return CTest to 100% passing.

## M1.3 — Refactor repeated test cases

Replace duplicated point checks with a small table of inputs and expected
outputs. Learn the minimum C++ needed for a test-case struct, `std::array`, and
a loop without introducing a full test framework yet.

Success criterion: coverage is unchanged, failure messages identify the case,
and the tests remain readable.

## M1.4 — Rotation representations

Construct the same rotation using:

- a rotation matrix;
- `Eigen::AngleAxisd`;
- `Eigen::Quaterniond`.

Compare their resulting matrices and transformed vectors. Learn normalization,
the quaternion double-cover sign ambiguity, and when Euler angles are useful
for display but risky for computation.

Success criterion: all three representations produce equivalent rotations
within tolerance, with an explanation of what each representation stores.

## M1.5 — Checkpoint

Explain in `LEARNING_LOG.md`:

- the repository's `T_BA` convention;
- composition order and why the rightmost transform acts first;
- how inverse changes frame direction;
- active motion versus passive coordinate change;
- why matrix dimensions cannot protect against frame mistakes.

M1 is complete only after a fresh build, all tests, a wrong-order mutation
check, and the learner-authored explanation are recorded in `PROGRESS.md`.

