# Visual Perception Foundations

A learning repository for building the C++ and geometric-computer-vision
foundation needed for visual odometry, SLAM, VIO, robotics perception, and
later learning-based vision work.

This is deliberately a small, cumulative project. Each milestone must leave a
runnable result, tests, and a short learning note before the next milestone
starts.

## Current status

- Current milestone: **M0 — toolchain and first Eigen program**
- Repository scaffold: created
- Build verification: **not run yet** (no C++ compiler/CMake was found on the
  current Windows PATH, and no Ubuntu WSL distribution is installed)
- Next decision: choose and install the development environment described in
  [M0](docs/milestones/M0-toolchain-and-eigen.md)

## Repository map

```text
app/                 small executable programs
tests/               executable checks and later unit tests
docs/ROADMAP.md      milestone sequence and scope
docs/PROGRESS.md     source of truth for current progress
docs/LEARNING_LOG.md short reflections written by the learner
docs/milestones/     step-by-step plan for each active milestone
```

## Working agreement

For every step:

1. Read the goal and predict what should happen.
2. Run or change one small thing yourself.
3. Compare the result with the stated success criterion.
4. Explain the result in your own words in `docs/LEARNING_LOG.md`.
5. Update `docs/PROGRESS.md` only with evidence.

The repository should not claim that something works merely because the files
exist. "Written", "built", "tested", and "understood" are separate states.

