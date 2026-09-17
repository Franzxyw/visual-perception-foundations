# Visual Perception Foundations

A learning repository for building the C++ and geometric-computer-vision
foundation needed for visual odometry, SLAM, VIO, robotics perception, and
later learning-based vision work.

This is deliberately a small, cumulative project. Each milestone must leave a
runnable result, tests, and a short learning note before the next milestone
starts.

## Current status

- Completed milestone: **M0 — toolchain and first Eigen program** (2026-09-17)
- Repository scaffold: created
- Environment: Ubuntu 24.04 WSL2 with GCC 13.3, CMake 3.28.3, and Eigen 3.4
- Build verification: demo built and produced the hand-calculated result
  `(1, 3, 0)`; CTest passed 1/1 tests on 2026-09-17
- Current milestone: **M1 — frames, SO(3), and SE(3)**
- Next step: complete M1.0 frame-notation reasoning before adding new code

Because the source repository is on a Windows-mounted drive, generated build
files live in the Linux filesystem:

```bash
cmake -S . -B ~/build/visual-perception-foundations
cmake --build ~/build/visual-perception-foundations
~/build/visual-perception-foundations/transform_demo
ctest --test-dir ~/build/visual-perception-foundations --output-on-failure
```

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
