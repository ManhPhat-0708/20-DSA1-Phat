FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y \
        g++ \
        cmake \
        wget \
        lcov && \
    mkdir -p /usr/local/include/doctest && \
    wget https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h \
         -O /usr/local/include/doctest/doctest.h && \
    apt clean && rm -rf /var/lib/apt/lists/*