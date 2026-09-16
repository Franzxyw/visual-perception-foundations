# Progress

Last updated: 2026-09-16

## Status legend

- `[ ]` not started
- `[~]` in progress or written but not verified
- `[x]` verified with evidence

## Current milestone: M0

- `[x]` Local Git repository created on branch `main`.
- `[x]` Learning roadmap and progress-check structure created.
- `[~]` Minimal CMake/Eigen demo and executable test written.
- `[ ]` Development toolchain selected and installed.
- `[ ]` CMake configure succeeds from a fresh terminal.
- `[ ]` Project builds without warnings/errors.
- `[ ]` `transform_demo` output checked against a hand calculation.
- `[ ]` `ctest --test-dir build --output-on-failure` passes.
- `[ ]` Learner changes the transform or test and explains why.
- `[ ]` M0 reflection completed in `LEARNING_LOG.md`.

## Evidence log

### 2026-09-16 — Initial audit

- Windows PATH: Git available; CMake and C++ compilers not found.
- WSL: only the internal `docker-desktop` distribution was listed; no Ubuntu
  development distribution was available.
- Docker CLI: not found on Windows PATH or at the standard Docker Desktop path.
- Consequence: source files exist, but no build/test success is claimed yet.

## Checkpoint rule

At each checkpoint, record:

1. exact command run;
2. important output or error;
3. what the result proves;
4. the next smallest step.

