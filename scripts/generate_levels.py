#!/usr/bin/env python3
"""Generate LevelData C++ source from .level files."""

import argparse
import glob
import os
import sys

LEVEL_WIDTH = 30
LEVEL_HEIGHT = 20
VALID_DIRECTIONS = {"up": "Up", "down": "Down", "left": "Left", "right": "Right"}
VALID_CHARS = frozenset(".#")


def parse_level(filepath):
    filename = os.path.basename(filepath)
    with open(filepath) as f:
        content = f.read()

    if "---" not in content:
        raise ValueError(f"Error in {filename}: missing '---' separator")

    header, grid = content.split("---", 1)

    # Parse metadata
    meta = {}
    for line in header.strip().splitlines():
        key, _, value = line.partition(":")
        if not value:
            raise ValueError(f"Error in {filename}: malformed metadata line: {line}")
        meta[key.strip()] = value.strip()

    required = ["foodsToComplete", "growthPerFood", "start", "startDirection"]
    for key in required:
        if key not in meta:
            raise ValueError(f"Error in {filename}: missing metadata key '{key}'")

    try:
        foods_to_complete = int(meta["foodsToComplete"])
    except ValueError:
        raise ValueError(f"Error in {filename}: foodsToComplete must be an integer")
    if foods_to_complete <= 0:
        raise ValueError(f"Error in {filename}: foodsToComplete must be positive")

    try:
        growth_per_food = int(meta["growthPerFood"])
    except ValueError:
        raise ValueError(f"Error in {filename}: growthPerFood must be an integer")
    if growth_per_food <= 0:
        raise ValueError(f"Error in {filename}: growthPerFood must be positive")

    direction_str = meta["startDirection"].lower()
    if direction_str not in VALID_DIRECTIONS:
        raise ValueError(
            f"Error in {filename}: startDirection must be one of "
            f"{', '.join(VALID_DIRECTIONS)}, got '{meta['startDirection']}'"
        )
    direction = VALID_DIRECTIONS[direction_str]

    try:
        parts = meta["start"].split(",")
        start_x, start_y = int(parts[0].strip()), int(parts[1].strip())
    except (ValueError, IndexError):
        raise ValueError(f"Error in {filename}: start must be 'x, y' integers")

    # Parse grid rows
    rows = [line for line in grid.splitlines() if line]
    if len(rows) != LEVEL_HEIGHT:
        raise ValueError(
            f"Error in {filename}: expected {LEVEL_HEIGHT} grid rows, got {len(rows)}"
        )

    for i, row in enumerate(rows):
        if len(row) != LEVEL_WIDTH:
            raise ValueError(
                f"Error in {filename}: row {i} has {len(row)} chars, expected {LEVEL_WIDTH}"
            )
        bad = set(row) - VALID_CHARS
        if bad:
            raise ValueError(
                f"Error in {filename}: row {i} contains invalid chars: {bad}"
            )

    if not (0 <= start_x < LEVEL_WIDTH and 0 <= start_y < LEVEL_HEIGHT):
        raise ValueError(f"Error in {filename}: start position ({start_x}, {start_y}) out of bounds")

    if rows[start_y][start_x] != ".":
        raise ValueError(
            f"Error in {filename}: start position ({start_x}, {start_y}) is not a free cell"
        )

    return {
        "rows": rows,
        "foodsToComplete": foods_to_complete,
        "growthPerFood": growth_per_food,
        "startX": start_x,
        "startY": start_y,
        "direction": direction,
    }


def generate_cpp(levels):
    lines = [
        '#include "game/LevelData.hpp"',
        "",
        "namespace game::levels",
        "{",
        "",
        "    const std::vector<LevelDefinition> &all()",
        "    {",
        "        static const std::vector<LevelDefinition> definitions{",
    ]

    for i, level in enumerate(levels):
        lines.append("            {")
        lines.append("                {")
        for row in level["rows"]:
            lines.append(f'                    "{row}",')
        lines.append("                },")
        lines.append(f"                {level['foodsToComplete']},")
        lines.append(f"                {level['growthPerFood']},")
        lines.append(f"                {{{level['startX']}, {level['startY']}}},")
        lines.append(f"                Direction::{level['direction']},")
        lines.append("            },")

    lines += [
        "        };",
        "",
        "        return definitions;",
        "    }",
        "",
        "}",
        "",
    ]
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description="Generate LevelData C++ from .level files")
    parser.add_argument("--input-dir", required=True, help="Directory containing .level files")
    parser.add_argument("--output", required=True, help="Output C++ file path")
    args = parser.parse_args()

    level_files = sorted(glob.glob(os.path.join(args.input_dir, "*.level")))
    if not level_files:
        print(f"Error: no .level files found in {args.input_dir}", file=sys.stderr)
        sys.exit(1)

    levels = []
    for filepath in level_files:
        try:
            levels.append(parse_level(filepath))
        except ValueError as e:
            print(str(e), file=sys.stderr)
            sys.exit(1)

    os.makedirs(os.path.dirname(args.output), exist_ok=True)

    cpp = generate_cpp(levels)
    with open(args.output, "w") as f:
        f.write(cpp)

    print(f"Generated {args.output} with {len(levels)} level(s)")


if __name__ == "__main__":
    main()
