#!/usr/bin/env bash
# ABOUTME: Build script for QMK firmware using Docker/Podman
# ABOUTME: Simplifies building firmware with different keymaps

set -euo pipefail

usage() {
    cat <<'USAGE'
Usage:
  ./build.sh user/id75 from_zmk_callum_mods
  ./build.sh user/id75 from_zmk_simple
  ./build.sh user/id75 from_zmk
  ./build.sh user/id75 via
  ./build.sh user/id75 default
  ./build.sh mt/id75 via
  ./build.sh mt/id75 default
USAGE
}

if [ "$#" -ne 2 ]; then
    usage
    exit 2
fi

cd "$(dirname "$0")"

KEYBOARD="$1"
KEYMAP="$2"
OUTPUT_DIR="$(pwd)/build"

if [ ! -d "keyboards/$KEYBOARD" ]; then
    echo "Error: Unknown keyboard '${KEYBOARD}'."
    usage
    exit 2
fi

if [ ! -d "keyboards/$KEYBOARD/keymaps/$KEYMAP" ]; then
    echo "Error: Unknown keymap '${KEYMAP}' for keyboard '${KEYBOARD}'."
    usage
    exit 2
fi

# Detect container runtime (podman or docker)
if command -v podman > /dev/null 2>&1; then
    CONTAINER_CMD="podman"
elif command -v docker > /dev/null 2>&1; then
    CONTAINER_CMD="docker"
else
    echo "Error: Neither podman nor docker found. Please install one of them."
    exit 1
fi

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
echo "Look for files like: ${KEYBOARD//\//_}_${KEYMAP}.uf2"
