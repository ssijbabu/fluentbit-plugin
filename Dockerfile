FROM debian:trixie-slim AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies (aligned with official Fluent Bit)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    curl \
    ca-certificates \
    pkg-config \
    libssl-dev \
    libcurl4-openssl-dev \
    libsasl2-dev \
    zlib1g-dev \
    libjson-c-dev \
    flex \
    bison \
    libyaml-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp

# Build Fluent Bit from source with minimal features
RUN cd /tmp && \
    git clone --depth 1 --branch v2.1.10 https://github.com/fluent/fluent-bit.git && \
    cd fluent-bit && \
    mkdir -p build && \
    cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
          -DCMAKE_BUILD_TYPE=Release \
          -DFLB_RELEASE=On \
          -DFLB_SHARED_LIB=On \
          -DFLB_EXAMPLES=Off \
          -DFLB_HTTP_SERVER=On \
          -DFLB_IN_EXEC=Off \
          -DFLB_CONFIG_YAML=Off \
          .. && \
    make -j$(nproc) && \
    make install && \
    cp -r /tmp/fluent-bit /fluent-bit-src && \
    cp -r /tmp/fluent-bit/build /fluent-bit-src/build && \
    cd /tmp && rm -rf fluent-bit

# Set working directory for plugin build
WORKDIR /app

# Copy project files
COPY . /app

# Create output directory first
RUN mkdir -p /app/output

# Build the plugins
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    find . -name "*.so" -exec cp {} /app/output/ \; 2>/dev/null || echo "Info: No .so files found" && \
    touch /app/output/.placeholder

# Runtime stage
FROM debian:trixie-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    ca-certificates \
    libssl3 \
    libcurl4 \
    libsasl2-2 \
    zlib1g \
    libjson-c5 \
    && rm -rf /var/lib/apt/lists/*

# Copy Fluent Bit and libraries from builder
COPY --from=builder /usr/local /usr/local

# Copy built plugins from builder
COPY --from=builder /app/output /app/plugins

# Set library path
ENV LD_LIBRARY_PATH=/usr/local/lib
ENV PATH=/usr/local/bin:$PATH

# Copy configuration
COPY config/fluent-bit.conf /etc/fluent-bit/fluent-bit.conf
COPY config/parsers.conf /etc/fluent-bit/parsers.conf
COPY config/plugins.conf /etc/fluent-bit/plugins.conf

WORKDIR /app

EXPOSE 2020

CMD ["/usr/local/bin/fluent-bit", "-c", "/etc/fluent-bit/fluent-bit.conf"]
