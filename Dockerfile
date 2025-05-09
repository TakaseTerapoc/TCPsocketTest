FROM debian:bookworm

RUN apt-get update && apt-get install -y \
    g++-aarch64-linux-gnu \
    cmake \
    ninja-build \
    build-essential

WORKDIR /build