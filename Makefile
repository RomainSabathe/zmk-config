# Makefile
.PHONY: build flash clean format

all: format build

build:
	nix build

flash:
	nix run .#flash

format:
	uv run python utils/format_keymap.py

clean:
	nix store gc
