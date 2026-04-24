# Keyboard Firmware

Personal keyboard firmware configs for ZMK and Vial-QMK. Builds are local and
Docker/Podman based; this repo does not use GitHub Actions or Nix flakes.

## Layout

```text
zmk/    ZMK configs, Dockerfile, and build script
qmk/    Vial-QMK keyboard definitions, Dockerfile, and build script
utils/  Small maintenance scripts
```

## Commands

Run `make help` to list every supported target.

ZMK targets:

```bash
make zmk-sofle
make zmk-sofle-left
make zmk-sofle-right
make zmk-chocofi
make zmk-chocofi-left
make zmk-chocofi-right
```

QMK targets:

```bash
make qmk-user-id75-from-zmk-callum-mods
make qmk-user-id75-from-zmk-simple
make qmk-user-id75-from-zmk
make qmk-user-id75-via
make qmk-user-id75-default
make qmk-mt-id75-via
make qmk-mt-id75-default
```

Formatting:

```bash
make format
```

## Requirements

- Docker or Podman
- `make`
- `uv`, for `utils/format_keymap.py`

See [zmk/README.md](zmk/README.md) and [qmk/README.md](qmk/README.md) for
firmware-specific details.
