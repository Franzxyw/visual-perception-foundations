# Quaternions for 3D Vision and Robot Perception

This note records the quaternion knowledge required by M1. It uses Eigen's
conventions and the repository frame convention:

```text
p_B = R_BA * p_A
q_BA maps vectors from frame A to frame B
```

## Representation

A quaternion has one scalar and three vector components:

```text
q = w + x i + y j + z k = [w, (x, y, z)]
```

Quaternion multiplication is associative but generally not commutative. A
quaternion used as a rotation must be a unit quaternion:

```text
||q|| = sqrt(w^2 + x^2 + y^2 + z^2) = 1
```

## Axis-angle conversion

For a rotation of angle `theta` about a unit axis `u = (ux, uy, uz)`:

```text
q = [cos(theta / 2), u * sin(theta / 2)]
```

For +90 degrees about z:

```text
q = [sqrt(2)/2, 0, 0, sqrt(2)/2]
  ~= [0.707107, 0, 0, 0.707107]
```

The axis supplied to `Eigen::AngleAxisd` must be normalized.

## Rotating a vector

A 3D vector `v` can be treated mathematically as a pure quaternion `[0, v]`.
The rotated vector is the vector part of:

```text
q * [0, v] * inverse(q)
```

Eigen exposes the operation directly:

```cpp
const Eigen::Vector3d rotated = quaternion * vector;
```

A quaternion represents rotation only. Translation still requires an
`Eigen::Isometry3d` or another pose representation.

## Composition and inverse

Under this repository's frame convention:

```text
q_CA = q_CB * q_BA
```

The rightmost rotation acts first, just as it does for rotation matrices and
rigid transforms.

The conjugate is:

```text
conjugate(q) = [w, -x, -y, -z]
```

For a general nonzero quaternion:

```text
inverse(q) = conjugate(q) / ||q||^2
```

For a unit quaternion, `inverse(q) == conjugate(q)`. If `q_BA` maps A to B,
then `inverse(q_BA)` maps B to A.

## Double cover: q and -q

`q` and `-q` represent the same 3D rotation. Coefficient equality is therefore
not a valid general test for rotation equality. Compare rotation matrices,
angular distance, or the absolute dot product of unit quaternions.

This matters for interpolation, trajectories, and learning-based rotation
losses: a sign flip in quaternion coefficients is not a physical jump in
orientation.

## Normalization

Floating-point composition, integration, optimization, and learned predictions
can move a quaternion away from unit norm. Normalize when appropriate:

```cpp
quaternion.normalize();
const Eigen::Quaterniond normalized = quaternion.normalized();
```

Do not normalize the all-zero quaternion. In estimation problems, orientation
is usually updated on the SO(3) manifold rather than treating four quaternion
coefficients as unconstrained independent values.

## Eigen quick reference

```cpp
const Eigen::AngleAxisd angle_axis(theta, axis.normalized());
const Eigen::Quaterniond q(angle_axis);

const Eigen::Vector3d rotated = q * vector;
const Eigen::Matrix3d rotation_matrix = q.toRotationMatrix();
const Eigen::Quaterniond inverse = q.inverse();
const Eigen::Quaterniond interpolated = q0.slerp(alpha, q1);
```

Eigen's scalar constructor uses `(w, x, y, z)`:

```cpp
Eigen::Quaterniond q(w, x, y, z);
```

However, `q.coeffs()` is ordered `(x, y, z, w)`. Other libraries and data
formats may also use `xyzw` rather than `wxyz`. Always check coefficient order,
frame direction, active/passive meaning, handedness, and multiplication order.

## Choosing a rotation representation

- Rotation matrix: direct and linear-algebra friendly, but stores nine values
  and must remain orthonormal with determinant +1.
- Axis-angle: geometrically intuitive for one rotation, but has ambiguity at
  zero angle and near pi.
- Unit quaternion: compact and efficient for composition and interpolation,
  but requires normalization and has the `q`/`-q` double cover.
- Euler angles: useful for display, but convention-dependent and vulnerable to
  gimbal lock; avoid them as the main internal representation.

## M1 mastery checklist

- Explain why a rotation quaternion has unit norm.
- Convert a unit axis and angle to a quaternion using the half-angle formula.
- Apply a quaternion to a vector in Eigen.
- Compose frame-labelled rotations in the correct order.
- Reverse a rotation with the inverse/conjugate.
- Explain why `q` and `-q` represent the same rotation.
- Normalize after operations that introduce numerical drift.
- Check `wxyz` versus `xyzw` and frame conventions at every interface.

Later VIO/state-estimation work will add quaternion integration, SO(3)
exponential/logarithmic maps, local perturbations, and Jacobians.
