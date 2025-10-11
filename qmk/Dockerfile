# ABOUTME: Dockerfile for building QMK keyboard firmware using qmk_vial
# ABOUTME: Builds firmware for ID75 keyboard with custom keymaps

FROM docker.io/qmkfm/qmk_cli:latest

# Install git if not already present
RUN apt-get update && apt-get install -y git && rm -rf /var/lib/apt/lists/*

# Clone qmk_vial firmware
WORKDIR /qmk_firmware
RUN git clone --depth 1 https://github.com/vial-kb/vial-qmk.git . && \
    qmk setup -y -H /qmk_firmware

# Copy keyboard configuration into qmk_firmware
COPY keyboards/user/id75 /qmk_firmware/keyboards/user/id75

# Set working directory
WORKDIR /qmk_firmware

# Default command: build the from_zmk_callum_mods keymap
CMD ["qmk", "compile", "-kb", "user/id75", "-km", "from_zmk_callum_mods"]
