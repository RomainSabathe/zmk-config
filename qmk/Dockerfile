# ABOUTME: Dockerfile for building QMK keyboard firmware using Vial-QMK
# ABOUTME: Builds firmware for ID75 keyboard with custom keymaps

FROM docker.io/qmkfm/qmk_cli:latest

# Install git if not already present
RUN apt-get update && apt-get install -y git && rm -rf /var/lib/apt/lists/*

# Clone Vial-QMK firmware.
WORKDIR /qmk_firmware
RUN git clone --depth 1 https://github.com/vial-kb/vial-qmk.git . && \
    qmk setup -y -H /qmk_firmware

# Copy keyboard configurations into qmk_firmware.
COPY keyboards/ /qmk_firmware/keyboards/

WORKDIR /qmk_firmware
