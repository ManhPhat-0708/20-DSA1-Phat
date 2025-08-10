FROM ubuntu:22.04

# Install essential development tools and dependencies
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y \
    g++-11 \
    gdb \
    cmake \
    git \
    curl \
    wget \
    unzip \
    build-essential \
    pkg-config \
    nano \
    lldb \
    lcov \
    python3 \
    python3-pip

# Set GCC/G++ version 11 as the default compiler
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100

# Set working directory inside container
WORKDIR /workspace

# Download and install doctest (single-header library)
RUN mkdir -p /usr/local/include/doctest && \
    wget https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -O /usr/local/include/doctest/doctest.h

# Clean up to reduce image size
RUN apt-get clean && rm -rf /var/lib/apt/lists/*