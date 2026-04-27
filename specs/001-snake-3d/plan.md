# Implementation Plan: Snake 3D

**Branch**: `001-make-snake-3d` | **Date**: 2026-04-27 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/001-snake-3d/spec.md`

## Summary

Build a simple, classroom-friendly Snake 3D game in C++ with OpenGL where the player controls a snake on a grid plane rendered in 3D perspective. The MVP delivers core snake loop (move, eat, grow, lose), state controls (pause/resume/restart), and progressive speed milestones.

## Technical Context

**Language/Version**: C++17 (compatible with course compiler toolchain)  
**Primary Dependencies**: OpenGL, GLUT/freeglut, C++ standard library  
**Storage**: N/A (runtime memory only)  
**Testing**: Manual scenario tests + lightweight runtime assertions in debug builds  
**Target Platform**: Desktop classroom environment (Windows lab machines)  
**Project Type**: Single desktop game application  
**Performance Goals**: Smooth gameplay with stable frame updates and deterministic snake step timing  
**Constraints**: Keep code beginner-readable, avoid advanced engine patterns, preserve simple build/run flow  
**Scale/Scope**: Single-player, one arena mode, one camera style, no networking, no persistence

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **Readable Game Code**: PASS. Plan uses small modules and direct game loop logic.
- **Small Vertical Slices**: PASS. Implementation is staged by gameplay slices (core loop first).
- **OpenGL Resource Discipline**: PASS. Texture/state ownership defined and cleanup included.
- **Build, Run, Validate Locally**: PASS. Quickstart includes local build/run and manual checks.
- **Keep It Simple, Then Expand**: PASS. No unnecessary frameworks or architecture layers.

## Project Structure

### Documentation (this feature)

```text
specs/001-snake-3d/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   └── gameplay-input-state-contract.md
└── tasks.md
```

### Source Code (repository root)

```text
main.cpp
runner.sh
README.md

src/
├── app/
│   ├── game_loop.cpp
│   └── game_loop.h
├── gameplay/
│   ├── snake.cpp
│   ├── snake.h
│   ├── arena.cpp
│   ├── arena.h
│   ├── food.cpp
│   └── food.h
├── render/
│   ├── renderer.cpp
│   ├── renderer.h
│   ├── camera.cpp
│   └── camera.h
└── input/
    ├── controls.cpp
    └── controls.h

assets/
└── textures/
```

**Structure Decision**: Single-project layout with clear gameplay/render/input separation to keep code easy to follow while avoiding over-engineering.

## Phase Plan

### Phase 0: Research and Decisions

- Lock grid representation and movement timing model.
- Choose 3D camera behavior (fixed angle with optional slight orbit).
- Define food spawn algorithm for no-overlap guarantee.
- Define speed milestone table and cap.

### Phase 1: Design Artifacts

- Finalize data model for snake segments, game session, and arena occupancy.
- Define input/state contract for start, pause, resume, restart, and direction changes.
- Write quickstart validation sequence aligned to spec acceptance scenarios.

### Phase 2: Implementation Slices

- Slice A (P1): Movement, food, growth, collision, score.
- Slice B (P2): Pause/resume/restart state flow and UI text overlays.
- Slice C (P3): Speed milestones and end-state win condition.
- Slice D: Texture pass and visual polish without changing mechanics.

### Phase 3: Validation

- Run all acceptance scenarios from spec.
- Validate edge cases (reverse input, rapid state keys, full-grid win).
- Verify build/run steps from quickstart on clean environment.

## Complexity Tracking

No constitution violations currently identified; complexity exceptions are not required.
