#!/usr/bin/env bash
# ABOUTME: Build script for ZMK firmware using Docker/Podman
# ABOUTME: Simplifies building firmware for split keyboard halves

set -e

# Change to project root (parent of zmk directory)
cd "$(dirname "$0")/.."

# Detect container runtime (podman or docker)
if command -v podman &> /dev/null; then
    CONTAINER_CMD="podman"
elif command -v docker &> /dev/null; then
    CONTAINER_CMD="docker"
else
    echo "Error: Neither podman nor docker found. Please install one of them."
    exit 1
fi

BOARD="nice_nano_v2"
SIDE="${1:-both}"  # left, right, or both
OUTPUT_DIR="$(pwd)/zmk/build"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

echo "Using container runtime: $CONTAINER_CMD"
echo "Building firmware for board: $BOARD, shield: sofle_${SIDE}"
echo "=============================================="

# Build container image
$CONTAINER_CMD build -f zmk/Dockerfile -t zmk-builder .

# Build function
build_side() {
    local side=$1
    echo "Building ${side} half..."

    # Clean the build directory for this side
    rm -rf "$OUTPUT_DIR/${side}"

    if [ "$CONTAINER_CMD" = "podman" ]; then
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw,Z" \
            zmk-builder \
            west build -s /workspace/zmk/app -d "build/${side}" -b "$BOARD" --pristine -- "-DSHIELD=sofle_${side}" "-DZMK_CONFIG=/workspace/config"
    else
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw" \
            zmk-builder \
            west build -s /workspace/zmk/app -d "build/${side}" -b "$BOARD" --pristine -- "-DSHIELD=sofle_${side}" "-DZMK_CONFIG=/workspace/config"
    fi

    # Copy and flatten output
    if [ "$CONTAINER_CMD" = "podman" ]; then
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw,Z" \
            zmk-builder \
            sh -c "cp build/${side}/zephyr/zmk.uf2 build/sofle_${side}.uf2"
    else
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw" \
            zmk-builder \
            sh -c "cp build/${side}/zephyr/zmk.uf2 build/sofle_${side}.uf2"
    fi
}

# Build based on requested side
if [ "$SIDE" = "both" ]; then
    build_side "left"
    build_side "right"
else
    build_side "$SIDE"
fi

echo ""
echo "Build complete! Firmware is in: $OUTPUT_DIR"
if [ "$SIDE" = "both" ]; then
    echo "Files: sofle_left.uf2, sofle_right.uf2"
else
    echo "File: sofle_${SIDE}.uf2"
fi
