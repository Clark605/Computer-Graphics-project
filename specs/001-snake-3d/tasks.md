# Tasks: Snake 3D

**Input**: Design documents from `/specs/001-snake-3d/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: Automated tests are not required by the spec. Validation tasks below use manual scenario checks from quickstart.md.

**Organization**: Tasks are grouped by user story so each story can be implemented and validated independently.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no direct dependency)
- **[Story]**: Which user story this task belongs to (`US1`, `US2`, `US3`)
- Each task includes exact file path(s)

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Establish code skeleton and run/build baseline for the feature

- [x] T001 Create source directories from the plan in `src/app/`, `src/gameplay/`, `src/render/`, `src/input/`, and `assets/textures/`
- [x] T002 Create header/source stubs for modules in `src/app/game_loop.h`, `src/app/game_loop.cpp`, `src/gameplay/arena.h`, `src/gameplay/arena.cpp`, `src/gameplay/snake.h`, `src/gameplay/snake.cpp`, `src/gameplay/food.h`, `src/gameplay/food.cpp`, `src/render/renderer.h`, `src/render/renderer.cpp`, `src/render/camera.h`, `src/render/camera.cpp`, `src/input/controls.h`, and `src/input/controls.cpp`
- [x] T003 Wire compilation entry to new modules by updating `main.cpp` and `runner.sh`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Build shared core systems used by all user stories

**⚠️ CRITICAL**: No user story work should begin until this phase is complete

- [x] T004 Implement shared game enums and constants in `src/app/game_loop.h` (`GameState`, `Direction`, arena defaults, base timing)
- [x] T005 [P] Implement arena coordinate validation and boundary helpers in `src/gameplay/arena.h` and `src/gameplay/arena.cpp`
- [x] T006 [P] Implement snake data structure, segment container, and reset behavior in `src/gameplay/snake.h` and `src/gameplay/snake.cpp`
- [x] T007 [P] Implement food entity structure and active position state in `src/gameplay/food.h` and `src/gameplay/food.cpp`
- [x] T008 Implement deterministic step timer and update loop scaffolding in `src/app/game_loop.cpp`
- [x] T009 Implement input capture abstraction and per-step direction queue contract in `src/input/controls.h` and `src/input/controls.cpp`
- [x] T010 Implement renderer scaffolding for board, snake segments, and food placeholders in `src/render/renderer.h` and `src/render/renderer.cpp`
- [x] T011 Implement fixed gameplay camera setup in `src/render/camera.h` and `src/render/camera.cpp`

**Checkpoint**: Foundation complete; user story slices can begin

---

## Phase 3: User Story 1 - Core Snake Gameplay (Priority: P1) 🎯 MVP

**Goal**: Deliver playable snake loop with movement, growth, score, and loss conditions

**Independent Test**: Start game, move snake, consume one food, confirm growth and score increase, then confirm wall/self collision triggers game-over

### Implementation for User Story 1

- [x] T012 [P] [US1] Implement one-cell-per-step head movement and body-follow logic in `src/gameplay/snake.cpp`
- [x] T013 [P] [US1] Implement valid direction change and reverse-input rejection in `src/input/controls.cpp` and `src/gameplay/snake.cpp`
- [x] T014 [US1] Implement food consumption detection and tail growth update in `src/app/game_loop.cpp`
- [x] T015 [US1] Implement score increment behavior for food events in `src/app/game_loop.cpp`
- [x] T016 [US1] Implement food spawn from free-cell set with no overlap in `src/gameplay/food.cpp` and `src/app/game_loop.cpp`
- [x] T017 [US1] Implement wall and self-collision detection with transition to `GAME_OVER` in `src/app/game_loop.cpp`
- [x] T018 [US1] Render score and game-over final score overlay in `src/render/renderer.cpp`
- [x] T019 [US1] Add manual validation notes for P1 scenarios in `specs/001-snake-3d/quickstart.md`

**Checkpoint**: US1 is fully playable and demoable as MVP

---

## Phase 4: User Story 2 - Clear Game State Flow (Priority: P2)

**Goal**: Add reliable pause/resume/restart transitions and stable behavior under repeated inputs

**Independent Test**: Pause during movement, verify freeze; resume; lose; restart to clean initial state

### Implementation for User Story 2

- [ ] T020 [P] [US2] Implement state transitions (`START`, `RUNNING`, `PAUSED`, `GAME_OVER`, `WIN`) in `src/app/game_loop.cpp`
- [ ] T021 [P] [US2] Implement pause toggle handling in `src/input/controls.cpp` and `src/app/game_loop.cpp`
- [ ] T022 [US2] Enforce paused invariants (no position/score updates) in `src/app/game_loop.cpp`
- [ ] T023 [US2] Implement restart-from-non-active-state reset flow in `src/app/game_loop.cpp`
- [ ] T024 [US2] Debounce/limit repeated state inputs to one transition per update step in `src/input/controls.cpp`
- [ ] T025 [US2] Render paused/start state overlays in `src/render/renderer.cpp`
- [ ] T026 [US2] Add manual validation notes for state-flow scenarios in `specs/001-snake-3d/quickstart.md`

**Checkpoint**: US1 and US2 both work independently with stable state behavior

---

## Phase 5: User Story 3 - Progressive Challenge (Priority: P3)

**Goal**: Add speed milestones and full-grid win condition

**Independent Test**: Reach score milestones and observe faster step rate; fill board and verify win state

### Implementation for User Story 3

- [ ] T027 [P] [US3] Implement speed milestone table and capped interval reduction in `src/app/game_loop.cpp`
- [ ] T028 [US3] Apply updated step interval at milestone boundaries in `src/app/game_loop.cpp`
- [ ] T029 [US3] Implement full-grid detection (`no free cells`) with transition to `WIN` in `src/app/game_loop.cpp` and `src/gameplay/food.cpp`
- [ ] T030 [US3] Render win-state overlay and final score in `src/render/renderer.cpp`
- [ ] T031 [US3] Add manual validation notes for speed and win scenarios in `specs/001-snake-3d/quickstart.md`

**Checkpoint**: All user stories complete and independently verifiable

---

## Phase 6: Polish & Cross-Cutting Concerns

**Purpose**: Improve readability, visuals, and final project delivery quality

- [ ] T032 [P] Add simple board/snake/food textures and binding lifecycle notes in `src/render/renderer.cpp` and `README.md`
- [ ] T033 Refactor oversized functions for readability and keep beginner-friendly naming in `src/app/game_loop.cpp` and `src/render/renderer.cpp`
- [ ] T034 Verify build/run instructions and controls are accurate in `README.md`
- [ ] T035 Execute full manual validation pass from `specs/001-snake-3d/quickstart.md` and record outcomes in `specs/001-snake-3d/quickstart.md`

---

## Dependencies & Execution Order

### Phase Dependencies

- **Phase 1 (Setup)**: Can start immediately
- **Phase 2 (Foundational)**: Depends on Phase 1; blocks all user stories
- **Phase 3 (US1)**: Depends on Phase 2; delivers MVP
- **Phase 4 (US2)**: Depends on Phase 2; may be done after or alongside late US1 polish
- **Phase 5 (US3)**: Depends on Phase 2 and benefits from US1 scoring loop
- **Phase 6 (Polish)**: Depends on completion of desired user stories

### User Story Dependencies

- **US1 (P1)**: No dependency on other user stories
- **US2 (P2)**: Uses shared loop/state but remains independently testable
- **US3 (P3)**: Uses scoring and food-spawn logic from US1

### Within Each User Story

- Core logic before rendering overlays
- Validation notes updated after behavior is implemented
- Keep each story demoable before moving to next

### Parallel Opportunities

- T005, T006, T007 can run in parallel after T004
- T012 and T013 can run in parallel within US1
- T020 and T021 can run in parallel within US2
- T027 and initial rendering prep for US3 can run in parallel

---

## Implementation Strategy

### MVP First (US1 only)

1. Complete Phase 1 and Phase 2
2. Complete Phase 3 (US1)
3. Run US1 independent validation
4. Demo playable Snake 3D MVP

### Incremental Delivery

1. Deliver US1 core gameplay
2. Add US2 game-state controls
3. Add US3 progressive challenge and win state
4. Finish with Phase 6 polish and documentation

### Team Parallel Strategy

1. Developer A: gameplay core (`snake`, `food`, collisions)
2. Developer B: rendering and overlays
3. Developer C: input/state transitions and docs
