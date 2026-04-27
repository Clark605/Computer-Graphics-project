<!--
Sync Impact Report
Version change: template -> 1.0.0
Modified principles: all placeholders replaced with five project principles
Added sections: Project Constraints; Development Workflow
Removed sections: none
Templates reviewed: ✅ .specify/templates/plan-template.md (no update needed)
Templates reviewed: ✅ .specify/templates/spec-template.md (no update needed)
Templates reviewed: ✅ .specify/templates/tasks-template.md (no update needed)
Runtime docs reviewed: ✅ README.md (no update needed)
Deferred items: none
-->

# Computer Graphics Course Project Constitution

## Core Principles

### I. Readable Game Code
All code MUST be easy for a classmate to follow without senior-level context.
Prefer clear names, small functions, shallow nesting, and direct control flow.
If a helper, abstraction, or pattern makes the code harder to read, do not use it.

### II. Small Vertical Slices
Build the game as small playable slices: movement, camera, textures, animation,
collision, scoring, and UI. Every change MUST leave the project runnable and
obviously improved. Large rewrites are discouraged unless they unblock a simpler
design.

### III. OpenGL Resource Discipline
Textures, models, animation data, and OpenGL state MUST have explicit ownership
and cleanup. Prefer simple loaders and reusable helpers over hidden side effects.
Resource lifetime bugs are treated as defects, not implementation details.

### IV. Build, Run, and Validate Locally
The project MUST compile and run on the course machine using the documented local
workflow. Every feature MUST have a quick validation path, even if that path is
manual gameplay check. If the run or asset flow changes, update the project
instructions in the same change.

### V. Keep It Simple, Then Expand
Choose the smallest design that proves the game idea. Do not add dependencies,
architecture layers, or advanced rendering techniques unless they solve a real
problem. The current goal is a clean, understandable 3D game, not an elaborate
engine.

## Project Constraints

- Target stack: C++ with OpenGL and GLUT, using local textures and basic
	animation support suitable for a classroom project.
- The game concept is intentionally flexible for now; features MUST support
	discovery of the final idea instead of locking the project into a premature
	design.
- Code style MUST stay approachable: short modules, descriptive identifiers, and
	minimal but useful comments where the logic is not obvious.
- Asset names, paths, and animation clips MUST be documented clearly enough for a
	teammate to follow the project without extra explanation.

## Development Workflow

- Start from the simplest playable prototype, then add one mechanic at a time.
- Keep implementation changes small enough to review in one sitting.
- Validate every visible change by building and running the project locally.
- When a feature affects controls, camera, textures, or animation timing, verify
	the behavior in the running game before moving on.
- Update README.md when setup, run steps, or required assets change.

## Governance

This constitution takes precedence over informal habits, chat guidance, and any
older project notes when they conflict.

Amendments require editing this file, updating any dependent templates or runtime
docs that are affected, and recording the rationale in the Sync Impact Report.
Versioning follows semantic rules: MAJOR for incompatible governance changes or
principle redefinitions, MINOR for new principles or materially expanded policy,
and PATCH for wording clarifications.

All contributors MUST keep the code understandable, keep the project runnable,
and avoid unnecessary complexity. Reviews SHOULD reject changes that are clever
but harder to explain than the problem they solve.

**Version**: 1.0.0 | **Ratified**: 2026-04-27 | **Last Amended**: 2026-04-27
