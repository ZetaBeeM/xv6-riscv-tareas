FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    build-essential gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu \
    qemu-system-misc bc perl wget git ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /src

CMD ["/bin/bash"]
