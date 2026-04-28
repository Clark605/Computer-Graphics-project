# Snake 3D: A Classroom-Friendly OpenGL Game

A simple, readable 3D Snake game implementation in C++ using OpenGL and GLUT. Designed for classroom instruction with clear code structure, fixed-speed gameplay, and comprehensive validation.

## Quick Start

### Prerequisites

- C++ compiler with C++17 support (e.g., g++.exe on Windows)
- OpenGL and GLUT/freeglut libraries installed

### Build and Run

1. Open a terminal at the project root
2. Run the build script:

```bash
C:\Ruby32-x64\msys64\usr\bin\bash.exe runner.sh
```

The game executable (`main.exe`) will compile and launch automatically.

## Controls

| Input | Action |
|-------|--------|
| **Arrow Keys** | Move snake (UP, DOWN, LEFT, RIGHT) |
| **P** | Pause/Resume game |
| **R** | Restart (from paused, game-over, or win states) |
| **Esc** | Exit game |

## Game Rules

### Core Gameplay (US1)
- Snake moves one cell per game step on a 20×20 grid
- Eating food grows the snake by one segment and increases score
- Colliding with walls or yourself triggers game-over

### Game States (US2)
- **START**: Press any arrow key to begin
- **RUNNING**: Active gameplay; pause with P
- **PAUSED**: No updates; press P to resume
- **GAME_OVER**: Hit a wall or yourself; press R to restart
- **WIN**: Fill the entire board; press R to restart

### Win Condition (US3)
- Movement speed remains fixed at 220ms per step
- Fill the board (no free cells remain) to trigger WIN state

## Project Structure

```
.
├── main.cpp                          # GLUT entry point
├── runner.sh                         # Build script
├── src/
│   ├── app/
│   │   ├── game_loop.h/cpp           # Central game loop and state machine
│   ├── gameplay/
│   │   ├── arena.h/cpp               # Grid boundaries and free-cell detection
│   │   ├── snake.h/cpp               # Snake entity and movement logic
│   │   ├── food.h/cpp                # Food entity and spawn logic
│   ├── input/
│   │   ├── controls.h/cpp            # Input capture and per-step direction queue
│   └── render/
│       ├── camera.h/cpp              # Fixed 3D camera setup
│       └── renderer.h/cpp            # Drawing board, snake, food, overlays
├── assets/                           # (Optional) Textures and models
├── specs/
│   └── 001-snake-3d/                 # Design documents and validation checklists
│       ├── spec.md                   # User stories and requirements
│       ├── plan.md                   # Technical architecture
│       ├── quickstart.md             # Build/run instructions and validation
│       └── tasks.md                  # Implementation task breakdown
└── README.md                         # This file
```

## Rendering

**Current Implementation**: Procedural textures (no external assets)
- Board: Checkerboard texture on grid surface
- Snake: Solid green texture (brighter head)
- Food: Solid red texture
- Overlays: Text via GLUT bitmap font

**Future Enhancement**: External texture files (optional)
- Texture files in `assets/textures/`
- Binding pattern documented in `src/render/renderer.cpp` (see "TEXTURE BINDING PATTERN" comment)

## Code Quality

- **Language**: C++17 standard
- **Compiler Flags**: `-std=c++17 -I./src -w -g` (warnings suppressed, debug symbols included)
- **Architecture**: Modular design with clear separation of concerns
  - Game loop handles state management and timing
  - Gameplay modules (snake, food, arena) are independent
  - Rendering abstracted into a single renderer class
  - Input handled via an event queue with per-step consumption
- **Readability**: Variable names and function signatures are explicit and beginner-friendly
- **Testing**: Manual validation scenarios documented in `specs/001-snake-3d/quickstart.md`

## Validation

See `specs/001-snake-3d/quickstart.md` for:
- **Manual validation checklist** (10 core scenarios)
- **Phase 3 validation notes** (US1: Core gameplay)
- **Phase 4 validation notes** (US2: State flow)
- **Phase 5 validation notes** (US3: Win condition)