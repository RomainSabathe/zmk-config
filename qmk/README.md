# QMK Firmware

Use Vial-QMK, not upstream `qmk_firmware`.

This directory contains Docker/Podman build tooling and local ID75 keyboard
definitions.

## Layout

```text
Dockerfile
build.sh
keyboards/
  user/id75/
  mt/id75/
```

Both keyboard definitions are kept. The `user/id75` definition has the current
ZMK-derived keymaps; `mt/id75` is retained for compatibility.

## Build

From the repo root:

```bash
make qmk-user-id75-from-zmk-callum-mods
make qmk-user-id75-from-zmk-simple
make qmk-user-id75-from-zmk
make qmk-user-id75-via
make qmk-user-id75-default
make qmk-mt-id75-via
make qmk-mt-id75-default
```

Or from this directory:

```bash
./build.sh user/id75 from_zmk_callum_mods
./build.sh user/id75 from_zmk_simple
./build.sh user/id75 from_zmk
./build.sh user/id75 via
./build.sh user/id75 default
./build.sh mt/id75 via
./build.sh mt/id75 default
```

Both the keyboard path and keymap are required.

## Output

Compiled firmware is written to `qmk/build/`.

For RP2040 boards, look for `.uf2` files such as:

```text
user_id75_from_zmk_callum_mods.uf2
mt_id75_via.uf2
```

For other boards, QMK may emit `.hex` or `.bin` files instead.

## Flashing

For RP2040 boards:

1. Hold BOOT while plugging in the keyboard, or double-tap RESET.
2. Copy the generated `.uf2` file to the mounted USB drive.
3. The keyboard reboots into the new firmware.
