# Feature Specification: Snake 3D

**Feature Branch**: `001-make-snake-3d`  
**Created**: 2026-04-27  
**Status**: Draft  
**Input**: User description: "lets try to make Snake 3d"

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Core Snake Gameplay (Priority: P1)

As a player, I can control a snake in a 3D arena, collect food, grow in length, and lose when I collide with myself or the arena boundary.

**Why this priority**: This is the main game loop and the minimum feature set that makes the game recognizable and playable.

**Independent Test**: Start a new game, move the snake, collect at least one food item, verify the snake grows, and verify the run ends on collision.

**Acceptance Scenarios**:

1. **Given** a new game is started, **When** the player changes movement direction, **Then** the snake follows the new valid direction on the next movement step.
2. **Given** the snake reaches a food item, **When** the food is consumed, **Then** the score increases, the snake length increases by one segment, and a new food item appears in a valid location.
3. **Given** the snake collides with the arena wall or its own body, **When** the collision occurs, **Then** the game ends and the player is shown the final score.

---

### User Story 2 - Clear Game State Flow (Priority: P2)

As a player, I can start, pause, resume, and restart a run so I can control when I play and recover quickly after losing.

**Why this priority**: Reliable game-state control improves usability and makes testing and classroom demos straightforward.

**Independent Test**: Start a run, pause and resume once, intentionally lose, then restart and confirm a clean new run begins.

**Acceptance Scenarios**:

1. **Given** a run is active, **When** the player pauses the game, **Then** snake movement and scoring stop until resumed.
2. **Given** the game-over screen is shown, **When** the player selects restart, **Then** score, snake length, and snake position reset to starting values.

---

### User Story 3 - Progressive Challenge (Priority: P3)

As a player, I experience increasing challenge as my run continues so the game stays engaging.

**Why this priority**: Difficulty scaling improves replay value while remaining simple to explain in a course project.

**Independent Test**: Play a run where multiple food items are consumed and verify movement speed increases at defined score milestones.

**Acceptance Scenarios**:

1. **Given** the player reaches a defined score milestone, **When** the next movement interval is applied, **Then** the snake moves faster than before.

---

### Edge Cases

- If randomly chosen food spawn coordinates overlap with the snake body, the system retries until a valid free coordinate is found.
- If a direction input would immediately reverse the snake into its neck segment, the input is ignored and current movement direction is preserved.
- If pause/resume/restart is pressed repeatedly in quick succession, only the first valid state transition per update step is applied.
- If restart is requested at the start screen or game-over screen, the system starts a clean new run from initial defaults.
- If the snake occupies all playable cells, the run ends in a win state and final score is shown.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST provide a bounded 3D play arena with a visible snake head, body segments, and food item.
- **FR-002**: System MUST move the snake exactly one cell per update step at a fixed interval during active gameplay.
- **FR-003**: Users MUST be able to change snake direction during active gameplay, with at most one accepted direction change per update step.
- **FR-004**: System MUST reject invalid immediate reverse-direction input that would cause instant self-collision.
- **FR-005**: System MUST detect food consumption and increase both score and snake length by one segment.
- **FR-006**: System MUST respawn food only in unoccupied valid arena positions and MUST NOT place food on any snake segment.
- **FR-007**: System MUST detect collisions with arena boundaries and with snake body segments.
- **FR-008**: System MUST transition to game-over state when a collision loss condition occurs.
- **FR-009**: Users MUST be able to pause and resume an active game, and while paused the snake position and score MUST remain unchanged.
- **FR-010**: Users MUST be able to restart the game from non-active states into a clean initial state.
- **FR-011**: System MUST display current score during gameplay and final score at game over.
- **FR-012**: System MUST increase movement speed at predefined score milestones to raise difficulty gradually.
- **FR-013**: System MUST end the run in a win state when no valid food spawn position remains and MUST display the final score.

### Key Entities *(include if feature involves data)*

- **Snake**: Player-controlled entity composed of an ordered list of segments, with direction, current speed tier, and alive/game-over state.
- **Food Item**: Collectible target with arena position and active state.
- **Arena Cell/Position**: Valid coordinate in the bounded play space used for snake placement, movement, and food spawn validation.
- **Game Session**: Runtime state containing score, pause state, elapsed run state, and restart lifecycle.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: New players can start a run and perform their first valid movement input within 15 seconds.
- **SC-002**: In playtesting, at least 95% of food pickups correctly increase both score and snake length with no invalid spawn overlap.
- **SC-003**: Pause and resume actions take effect within one movement step in 100% of test runs.
- **SC-004**: In instructor/demo testing, at least 90% of participants can complete one full cycle of start, play, lose, and restart without assistance.
- **SC-005**: Difficulty increase is noticeable by players by mid-run, with at least two speed milestones reached in a 3-minute successful session.

## Assumptions

- The feature is for a single local player using keyboard controls.
- Snake movement is on one gameplay plane within a 3D-rendered scene to keep scope suitable for the course timeline.
- No online play, account system, or persistent profile storage is required for this feature.
- Audio is optional and not required for feature completion.
- One default arena configuration is sufficient for the first release of Snake 3D.
- Gameplay clarity and readability are prioritized over advanced visual effects.
