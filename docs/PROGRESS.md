# Progress

Last updated: 2026-09-17

## Status legend

- `[ ]` not started
- `[~]` in progress or written but not verified
- `[x]` verified with evidence

## Current milestone: M0

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
- `[ ]` M0 reflection completed in `LEARNING_LOG.md`.

## Evidence log

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

## Checkpoint rule

At each checkpoint, record:

1. exact command run;
2. important output or error;
3. what the result proves;
4. the next smallest step.
