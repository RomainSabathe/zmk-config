-include config.mk

# Default values
WINDOWS_USERNAME ?= $(USER)
WINDOWS_PATH ?= Desktop/firmware

.PHONY: build flash clean copy-windows dev dev-wsl

build:
	nix build

flash: build
	nix run .#flash

clean:
	nix store gc

# Copy firmware to Windows (WSL only)
copy-windows: build
	@if [ -f /proc/version ] && grep -qi microsoft /proc/version; then \
		echo "Copying firmware to Windows..."; \
		mkdir -p "/mnt/c/Users/$(WINDOWS_USERNAME)/$(WINDOWS_PATH)"; \
		find -L result -name "*.uf2" -type f -exec cp -v {} "/mnt/c/Users/$(WINDOWS_USERNAME)/$(WINDOWS_PATH)/" \; ; \
		echo "Firmware copied to /mnt/c/Users/$(WINDOWS_USERNAME)/$(WINDOWS_PATH)"; \
	else \
		echo "Not in WSL, skipping Windows copy"; \
	fi
format:

	uv run python utils/format_keymap.py

# Development workflows
dev: build flash

dev-wsl: build copy-windows flash

help:
	@echo "Available targets:"
	@echo "  build        - Build firmware"
	@echo "  flash        - Build and flash firmware"
	@echo "  copy-windows - Copy firmware to Windows (WSL only)"
	@echo "  dev          - Build and flash"
	@echo "  dev-wsl      - Build, copy to Windows, and flash"
	@echo "  clean        - Clean nix store"
