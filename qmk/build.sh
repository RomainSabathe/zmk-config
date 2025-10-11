#!/usr/bin/env bash
# ABOUTME: Build script for QMK firmware using Docker/Podman
# ABOUTME: Simplifies building firmware with different keymaps

set -e

# Detect container runtime (podman or docker)
if command -v podman &> /dev/null; then
    CONTAINER_CMD="podman"
elif command -v docker &> /dev/null; then
    CONTAINER_CMD="docker"
else
    echo "Error: Neither podman nor docker found. Please install one of them."
    exit 1
fi

KEYBOARD="user/id75"
KEYMAP="${1:-from_zmk_callum_mods}"
OUTPUT_DIR="$(pwd)/build"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

echo "Using container runtime: $CONTAINER_CMD"
echo "Building firmware for keyboard: $KEYBOARD, keymap: $KEYMAP"
echo "=============================================="

# Build container image
$CONTAINER_CMD build -t qmk-builder .

# Run the build
# Use :Z for podman SELinux labeling
if [ "$CONTAINER_CMD" = "podman" ]; then
    $CONTAINER_CMD run --rm \
        -v "$OUTPUT_DIR:/qmk_firmware/.build:rw,Z" \
        qmk-builder \
        qmk compile -kb "$KEYBOARD" -km "$KEYMAP"
else
    $CONTAINER_CMD run --rm \
        -v "$OUTPUT_DIR:/qmk_firmware/.build:rw" \
        qmk-builder \
        qmk compile -kb "$KEYBOARD" -km "$KEYMAP"
fi

echo ""
echo "Build complete! Firmware is in: $OUTPUT_DIR"
echo "Look for files like: user_id75_${KEYMAP}.uf2"
