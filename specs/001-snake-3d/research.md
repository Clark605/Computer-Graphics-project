# Research: Snake 3D

## Decision 1: Gameplay Space Representation

- **Decision**: Use a discrete 2D grid for gameplay logic, rendered as a 3D board.
- **Rationale**: Matches classic Snake behavior and simplifies collision, growth, and food spawning.
- **Alternatives Considered**:
  - Continuous 3D movement with physics: rejected as unnecessarily complex for course scope.
  - Multi-layer 3D snake movement: rejected due to increased control and collision complexity.

## Decision 2: Timing Model

- **Decision**: Separate render cadence from snake step cadence (fixed gameplay step interval).
- **Rationale**: Keeps control deterministic and supports clean speed milestone changes.
- **Alternatives Considered**:
  - Per-frame movement tied directly to FPS: rejected due to variable behavior across machines.

## Decision 3: Food Spawn Validity

- **Decision**: Randomly select from all free cells, not from entire board with indefinite retries.
- **Rationale**: Prevents excessive retry loops as snake grows and makes full-grid detection trivial.
- **Alternatives Considered**:
  - Retry random coordinates until valid: acceptable early, but less predictable late-game.

## Decision 4: Camera and Presentation

- **Decision**: Use a fixed perspective camera looking at the board with optional mild zoom.
- **Rationale**: Easy to understand, easy to debug, and keeps player orientation clear.
- **Alternatives Considered**:
  - Fully free camera: rejected due to unnecessary input complexity.
  - First-person snake head camera: rejected due to visibility and control difficulty.

## Decision 5: Difficulty Scaling

- **Decision**: Increase speed at score milestones (example: every 5 points) up to safe cap.
- **Rationale**: Simple progression with predictable behavior and easy testing.
- **Alternatives Considered**:
  - Dynamic adaptive difficulty: rejected as overkill for v1.

## Decision 6: UI Feedback

- **Decision**: Render text overlays for score and state labels (Paused, Game Over, You Win).
- **Rationale**: Needed for usability and acceptance tests with minimal implementation overhead.
