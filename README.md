# Mato (Worm Game)

A minimal worm/snake game written in C++ with SFML.

## Rules

- Move the worm with arrow keys or `WASD`.
- Eat apples to grow.
- Level completes after collecting a configured number of foods.
- Growth per food is configured per level.
- Levels can include walls.
- Game ends on wall collision or self collision.
- Completing the final level wins the game.

## Level configuration

Levels are defined as plain text files in `levels/`, named `01.level`, `02.level`, etc. (filename sort = level order). A Python script converts them to C++ at build time.

Each `.level` file has this format:

```
foodsToComplete: 8
growthPerFood: 5
start: 4, 10
startDirection: right
---
..............................
..............................
(20 rows of exactly 30 chars, # = wall, . = free)
```

- `foodsToComplete` — number of foods to collect to finish the level
- `growthPerFood` — snake cells added per food eaten
- `start` — initial head position as `x, y` (must be a free cell)
- `startDirection` — one of `up`, `down`, `left`, `right`
- Grid dimensions are fixed at 30x20 (defined in `src/game/LevelData.hpp`)

To add a level, create a new `.level` file in `levels/` and rebuild.

## Build on Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y build-essential cmake python3 libsfml-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/snake
```

The build copies `assets/` next to the executable. The game expects `assets/fonts/Comfortaa-Regular.otf` for on-screen status text.
