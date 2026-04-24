# ZMK Firmware

This directory contains the ZMK build environment and keyboard configs.

## Layout

```text
Dockerfile
build.sh
config/
  west.yml
  sofle.keymap
  sofle.conf
  chocofi.keymap
  corne.keymap
  corne.conf
```

The local build uses `nice_nano_v2` for all ZMK targets.

## Build

From the repo root:

```bash
make zmk-sofle
make zmk-sofle-left
make zmk-sofle-right
make zmk-chocofi
make zmk-chocofi-left
make zmk-chocofi-right
```

Or from this directory:

```bash
./build.sh sofle
./build.sh sofle left
./build.sh sofle right
./build.sh chocofi
./build.sh chocofi left
./build.sh chocofi right
```

The keyboard argument is required. If the side is omitted, both halves are
built.

## Targets

```text
sofle left    -> sofle_left
sofle right   -> sofle_right
chocofi left  -> corne_left nice_view_adapter nice_view
chocofi right -> corne_right nice_view_adapter nice_view
```

## Output

Firmware files are written to `zmk/build/`:

```text
sofle_left.uf2
sofle_right.uf2
chocofi_left.uf2
chocofi_right.uf2
```

To flash a half, put the controller into bootloader mode and copy the matching
`.uf2` file to the mounted drive.
