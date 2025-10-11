# Building QMK Firmware with Docker

This directory contains a Docker-based build system for QMK keyboard firmware using qmk_vial.

## Prerequisites

- Docker installed and running
- Your keyboard configuration in `keyboards/user/id75/`

## Quick Start

Build the firmware with the default keymap (`from_zmk_callum_mods`):

```bash
./build.sh
```

Build with a different keymap:

```bash
./build.sh via
./build.sh from_zmk_simple
./build.sh default
```

## Output

The compiled firmware will be in the `build/` directory:
- For RP2040 boards: `user_id75_<keymap>.uf2`
- For other boards: `user_id75_<keymap>.hex` or `user_id75_<keymap>.bin`

## Flashing

For RP2040-based boards (like yours):
1. Hold the BOOT button while plugging in the keyboard (or double-tap RESET)
2. The keyboard will appear as a USB drive
3. Copy the `.uf2` file to the drive
4. The keyboard will automatically reboot with the new firmware

## Manual Docker Commands

Build the Docker image:
```bash
docker build -t qmk-builder .
```

Run a build:
```bash
docker run --rm -v "$(pwd)/build:/qmk_firmware/.build:rw" qmk-builder qmk compile -kb user/id75 -km from_zmk_callum_mods
```

## Troubleshooting

If the build fails, try:
1. Remove the Docker image and rebuild: `docker rmi qmk-builder && docker build -t qmk-builder .`
2. Check that your keyboard configuration is valid
3. Make sure all required source files are in `keyboards/user/id75/`
