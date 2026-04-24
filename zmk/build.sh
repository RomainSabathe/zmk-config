#!/usr/bin/env bash
# ABOUTME: Build script for ZMK firmware using Docker/Podman
# ABOUTME: Supports sofle and chocofi keyboard targets

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
KEYBOARD="${1:-sofle}"  # sofle or chocofi
SIDE="${2:-both}"       # left, right, or both
OUTPUT_DIR="$(pwd)/zmk/build"

# Map keyboard name to shield name
case "$KEYBOARD" in
    sofle)
        shield_for_side() { echo "sofle_${1}"; }
        ;;
    chocofi)
        shield_for_side() { echo "corne_${1} nice_view_adapter nice_view"; }
        ;;
    *)
        echo "Error: Unknown keyboard '${KEYBOARD}'. Use 'sofle' or 'chocofi'."
        exit 1
        ;;
esac

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

echo "Using container runtime: $CONTAINER_CMD"
echo "Building firmware for keyboard: $KEYBOARD, board: $BOARD, side: $SIDE"
echo "=============================================="

# Build container image
$CONTAINER_CMD build -f zmk/Dockerfile -t zmk-builder .

# Build function
build_side() {
    local side=$1
    local shield
    shield="$(shield_for_side "$side")"
    echo "Building ${side} half (shield: ${shield})..."

    # Clean the build directory for this side
    rm -rf "$OUTPUT_DIR/${KEYBOARD}_${side}"

    if [ "$CONTAINER_CMD" = "podman" ]; then
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw,Z" \
            zmk-builder \
            west build -s /workspace/zmk/app -d "build/${KEYBOARD}_${side}" -b "$BOARD" --pristine -- "-DSHIELD=${shield}" "-DZMK_CONFIG=/workspace/config"
    else
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw" \
            zmk-builder \
            west build -s /workspace/zmk/app -d "build/${KEYBOARD}_${side}" -b "$BOARD" --pristine -- "-DSHIELD=${shield}" "-DZMK_CONFIG=/workspace/config"
    fi

    # Copy and flatten output
    if [ "$CONTAINER_CMD" = "podman" ]; then
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw,Z" \
            zmk-builder \
            sh -c "cp build/${KEYBOARD}_${side}/zephyr/zmk.uf2 build/${KEYBOARD}_${side}.uf2"
    else
        $CONTAINER_CMD run --rm \
            -v "$OUTPUT_DIR:/workspace/build:rw" \
            zmk-builder \
            sh -c "cp build/${KEYBOARD}_${side}/zephyr/zmk.uf2 build/${KEYBOARD}_${side}.uf2"
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
    echo "Files: ${KEYBOARD}_left.uf2, ${KEYBOARD}_right.uf2"
else
    echo "File: ${KEYBOARD}_${SIDE}.uf2"
fi
