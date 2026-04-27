# Contract: Gameplay Input and State Transitions

## Inputs

- `DIR_UP`, `DIR_DOWN`, `DIR_LEFT`, `DIR_RIGHT`
- `PAUSE_TOGGLE`
- `RESTART`
- `EXIT`

## Allowed Direction Behavior

- Direction input is evaluated once per gameplay update step.
- At most one direction change is accepted per step.
- Immediate reverse direction relative to current movement is ignored.

## State Machine

- `START` -> `RUNNING`:
  - Trigger: first valid direction input or start action.
- `RUNNING` -> `PAUSED`:
  - Trigger: `PAUSE_TOGGLE`.
- `PAUSED` -> `RUNNING`:
  - Trigger: `PAUSE_TOGGLE`.
- `RUNNING` -> `GAME_OVER`:
  - Trigger: wall collision or self-collision.
- `RUNNING` -> `WIN`:
  - Trigger: no valid free food spawn cells remain.
- `GAME_OVER` -> `START`:
  - Trigger: `RESTART`.
- `WIN` -> `START`:
  - Trigger: `RESTART`.
- `START` -> `START`:
  - Trigger: `RESTART` reinitializes defaults idempotently.

## Invariants

- In `PAUSED`, snake head position and score do not change.
- In non-running states, gameplay step updates are not applied.
- Food must never occupy a coordinate used by any snake segment.
- Score increments exactly once per food consumption event.
