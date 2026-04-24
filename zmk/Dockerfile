# ABOUTME: Dockerfile for building ZMK keyboard firmware
# ABOUTME: Builds firmware for local ZMK keyboard configs

FROM docker.io/zmkfirmware/zmk-build-arm:stable

# Install git if not already present
RUN apt-get update && apt-get install -y git && rm -rf /var/lib/apt/lists/*

# Set up workspace
WORKDIR /workspace

# Copy west config first
COPY config/west.yml config/west.yml

# Initialize west workspace and get dependencies
RUN west init -l config && \
    west update && \
    west zephyr-export

# Copy all config files.
COPY config/ /workspace/config/
