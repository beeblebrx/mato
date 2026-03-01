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

- Edit level order, wall layouts, foods-to-complete, growth-per-food, and worm start position/direction in `src/game/LevelData.cpp`.
- Level dimensions are fixed at `30x20` and defined in `src/game/LevelData.hpp`.
- Use `#` for wall cells and `.` for free cells in each layout row.

## Build on Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y build-essential cmake libsfml-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/snake
```

The build copies `assets/` next to the executable. The game expects `assets/fonts/Comfortaa-Regular.otf` for on-screen status text.
