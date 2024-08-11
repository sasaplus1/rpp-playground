FROM debian:bookworm

RUN apt-get --yes update && apt-get --yes install \
  gcc-arm-none-eabi \
  libnewlib-arm-none-eabi \
  cmake \
  build-essential \
  git \
  ninja-build \
  python3 \
  && apt-get clean

RUN git clone -b master https://github.com/raspberrypi/pico-sdk.git /pico-sdk \
  && cd /pico-sdk \
  && git submodule update --init --recursive

WORKDIR /rpi

ENV PICO_SDK_PATH=/pico-sdk
ENV PICO_TOOLCHAIN_PATH=/usr/bin/arm-none-eabi-gcc

ENTRYPOINT ["/bin/bash"]
