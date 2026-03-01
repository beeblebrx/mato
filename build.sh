#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build"
BUILD_TYPE="Release"
CLEAN_BUILD="false"

usage() {
    cat <<'EOF'
Usage: ./build.sh [options]

Options:
  -d, --dir <path>      Build directory (default: build)
  -t, --type <type>     CMake build type (default: Release)
  -c, --clean           Remove build directory before configuring
  -h, --help            Show this help

Examples:
  ./build.sh
  ./build.sh --type Debug
  ./build.sh --clean --dir build-release --type Release
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        -d|--dir)
            BUILD_DIR="${2:-}"
            if [[ -z "$BUILD_DIR" ]]; then
                echo "Error: --dir requires a value." >&2
                exit 1
            fi
            shift 2
            ;;
        -t|--type)
            BUILD_TYPE="${2:-}"
            if [[ -z "$BUILD_TYPE" ]]; then
                echo "Error: --type requires a value." >&2
                exit 1
            fi
            shift 2
            ;;
        -c|--clean)
            CLEAN_BUILD="true"
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage
            exit 1
            ;;
    esac
done

if [[ "$CLEAN_BUILD" == "true" ]]; then
    echo "Cleaning build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

echo "Configuring ($BUILD_TYPE) into $BUILD_DIR ..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "Building..."
cmake --build "$BUILD_DIR" --parallel

echo "Build complete: $BUILD_DIR/snake"