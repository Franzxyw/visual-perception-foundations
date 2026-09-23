# M2 — Pinhole Camera and Calibration

## Purpose

Build a camera model from the equations upward. The milestone starts with an
ideal pinhole camera implemented using Eigen, then adds distortion,
reprojection error, and a small OpenCV calibration exercise. Library calls
must remain connected to explicit coordinate-frame and unit conventions.

## Convention used in this repository

For the first exercises, the camera frame follows a common computer-vision
image convention:

```text
x points right
y points down
z points forward through the image plane
```

A camera-frame point is `p_C = (X, Y, Z)`. Ideal perspective projection first
forms normalized image coordinates:

```text
x_n = X / Z
y_n = Y / Z
```

and then pixel coordinates:

```text
u = f_x * x_n + c_x
v = f_y * y_n + c_y
```

Equivalently:

```text
[u_h, v_h, w_h]^T = K * [X, Y, Z]^T
[u, v]^T = [u_h / w_h, v_h / w_h]^T

K = [f_x   0   c_x]
    [ 0   f_y  c_y]
    [ 0    0    1 ]
```

Here `f_x`, `f_y`, `c_x`, and `c_y` are expressed in pixels. Distortion is
deliberately excluded until the ideal model is understood and tested.

## M2.0 — Projection reasoning before code

Use:

```text
f_x = 400 px
f_y = 420 px
c_x = 320 px
c_y = 240 px
p_C = (0.5 m, -0.2 m, 2.0 m)
```

Before writing C++, answer:

1. What are `(x_n, y_n)` and the projected pixel `(u, v)`?
2. Why does `3 * p_C` project to the same pixel?
3. Which quantities use metres, pixels, or no units?
4. Why should the initial API reject `Z <= 0` rather than silently project it?

Success criterion: derive the pixel by hand and explain why pinhole projection
loses absolute depth.

## M2.1 — Minimal pinhole projection API

Add a small `PinholeCamera` type containing `f_x`, `f_y`, `c_x`, and `c_y`.
Implement projection from one camera-frame 3D point to one pixel without
OpenCV.

The API must make failure explicit for a point on or behind the camera plane.
Do not use an arbitrary epsilon without documenting what it means.

Success criterion: the hand-calculated M2.0 example passes, and scaling a
point along the same camera ray leaves its pixel unchanged.

## M2.2 — Projection tests and invalid inputs

Use table-driven cases to test:

- the principal-axis point `(0, 0, Z)` maps to `(c_x, c_y)`;
- positive and negative image offsets;
- ray-scale invariance;
- `Z == 0` and `Z < 0` are rejected;
- a deliberately wrong focal length or principal point fails a known answer.

Success criterion: observe one intended mutation failure, restore the model,
and return CTest to 100% passing.

## M2.3 — Back-projection to a ray

Implement normalized back-projection:

```text
x_n = (u - c_x) / f_x
y_n = (v - c_y) / f_y
ray_C = (x_n, y_n, 1)
```

The result is a ray, not a unique 3D point. Given an explicit positive depth
`Z`, recover `(X, Y, Z)` and test the projection/back-projection round trip.

Success criterion: explain what information is lost during projection and what
extra information is required to recover metric scale.

## M2.4 — Matrix form and frame composition

Relate the scalar equations to `K * p_C`, then introduce a world-to-camera
transform:

```text
p_C = T_CW * p_W
pixel = project(p_C)
```

Keep the rigid transform and camera intrinsics conceptually separate. Test a
known world point through both stages.

Success criterion: distinguish extrinsics from intrinsics and state every
input/output frame.

## M2.5 — Lens distortion

Add radial distortion first, then tangential distortion only after the radial
case is verified. Compare undistorted and distorted normalized coordinates and
visualize how displacement changes across the image.

Success criterion: the learner can explain why distortion acts on normalized
image coordinates and why points near the image edge are usually affected
more strongly.

## M2.6 — Reprojection error and OpenCV calibration

Install OpenCV only at this point. Generate or use a small controlled set of
3D/2D correspondences, estimate camera parameters, and compute per-point and
root-mean-square reprojection error. Compare OpenCV output with the repository's
own projection model.

Success criterion: report parameter error and reprojection error separately,
and document at least one weak or degenerate calibration setup.

## M2 checkpoint

M2 is complete only after:

- ideal projection and back-projection are implemented and tested;
- invalid depth is handled explicitly;
- intrinsics and extrinsics are explained separately;
- distortion has a verified numerical or visual example;
- reprojection error is measured on a controlled calibration exercise;
- one intended mutation failure and a fresh full CTest run are recorded.
