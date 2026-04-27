# Data Model: Snake 3D

## Entity: Arena

- **Purpose**: Defines valid gameplay bounds and coordinate system.
- **Fields**:
  - `width` (int): Number of columns.
  - `height` (int): Number of rows.
  - `cells` (derived): Coordinate set from `(0,0)` to `(width-1,height-1)`.
- **Rules**:
  - Coordinates outside bounds are invalid.

## Entity: SnakeSegment

- **Purpose**: One body element of snake.
- **Fields**:
  - `x` (int)
  - `y` (int)

## Entity: Snake

- **Purpose**: Player-controlled snake state.
- **Fields**:
  - `segments` (ordered list of `SnakeSegment`): Head at index 0.
  - `direction` (enum): `UP`, `DOWN`, `LEFT`, `RIGHT`.
  - `pendingDirection` (enum/optional): Direction request waiting for next step.
- **Rules**:
  - Reverse direction into neck is invalid.
  - Growth adds one segment at tail on successful food event.

## Entity: Food

- **Purpose**: Collectible target.
- **Fields**:
  - `x` (int)
  - `y` (int)
  - `isActive` (bool)
- **Rules**:
  - Must not overlap any snake segment.

## Entity: GameSession

- **Purpose**: Tracks run lifecycle and scoring.
- **Fields**:
  - `score` (int)
  - `state` (enum): `START`, `RUNNING`, `PAUSED`, `GAME_OVER`, `WIN`.
  - `stepIntervalMs` (int): Current movement step interval.
  - `baseStepIntervalMs` (int)
  - `speedTier` (int)
- **Rules**:
  - Score increments by exactly 1 per food consumed.
  - State transitions follow contract document.

## Derived Data

- **OccupiedCells**: Set of all coordinates currently used by snake segments.
- **FreeCells**: `Arena.cells - OccupiedCells`.
- **SpawnAvailability**: `FreeCells.size() > 0`.
