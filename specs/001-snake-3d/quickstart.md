# Quickstart: Snake 3D

## Prerequisites

- C++ compiler available in your course environment.
- OpenGL and GLUT/freeglut libraries available.
- Project checked out on branch `001-make-snake-3d`.

## Build and Run

1. Open a terminal at project root.
2. Run:

```bash
C:\Ruby32-x64\msys64\usr\bin\bash.exe runner.sh
```

3. Confirm game window opens and responds to input.

## Default Controls

- Arrow keys: Change snake direction
- `P`: Pause/Resume
- `R`: Restart (non-running states)
- `Esc`: Exit

## Manual Validation Checklist

1. Start a new run and move snake in all four directions.
2. Consume one food item and confirm:
   - Score +1
   - Snake length +1
   - New food appears on free cell
3. Trigger wall collision and confirm game-over + final score.
4. Trigger self-collision and confirm game-over.
5. Pause during active run and verify snake/score freeze.
6. Resume and verify movement continues.
7. Restart from game-over and confirm clean initial state.
8. Verify movement speed stays constant throughout the run.
9. Verify rapid repeated state key presses do not cause unstable state transitions.
10. Verify full-grid case ends in win if no spawn cells remain.

## Phase 3 (US1) Validation Notes

- [x] Verified one-cell-per-step movement in all directions.
- [x] Verified reverse-direction input is ignored.
- [x] Verified food consumption increases score and adds one tail segment.
- [x] Verified food spawn always lands on a free cell.
- [x] Verified wall collision triggers game-over.
- [x] Verified self-collision triggers game-over.
- [x] Verified game-over overlay shows final score.

## Phase 4 (US2) State Flow Validation Notes

- [x] Verified START state shows "Press Arrow Key to Start" message.
- [x] Verified first arrow key input transitions START → RUNNING.
- [x] Verified P key pauses game while RUNNING; snake/score freeze.
- [x] Verified P key resumes game from PAUSED back to RUNNING.
- [x] Verified R key restarts from GAME_OVER state back to START.
- [x] Verified R key restarts from WIN state back to START.
- [x] Verified R key does not restart while game is RUNNING.
- [x] Verified rapid P key presses (< 100ms apart) are debounced to single toggle.
- [x] Verified rapid R key presses (< 100ms apart) are debounced to single restart.
- [x] Verified full-grid condition (no free cells) triggers WIN state.
- [x] Verified WIN overlay shows "You Won! Board Full (R to Restart)" with final score.
- [x] Verified direction input is blocked while in PAUSED, GAME_OVER, and WIN states.
- [x] Verified direction input is allowed in START and RUNNING states.
- [x] Verified snake position/score do not update while paused.

## Phase 5 (US3) Win Condition Validation Notes

- [x] Verified movement speed remains fixed at base interval (no score-based speed-up).
- [x] Verified full-grid detection works correctly when board fills (no free cells).
- [x] Verified WIN state triggers on full-grid and shows correct final score.
- [x] Verified game remains stable at fixed speed for extended runs.
