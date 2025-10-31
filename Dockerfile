# Lightweight (~60-80mb), debian support, arm64 / GCC/g++ version 10.2
# arm64v8/debian --- similar size, also considered...
FROM debian:bullseye-slim

# Update and install required dependencies
# git used to pull google test
RUN apt-get update -y && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    g++ \
    libcurl4-openssl-dev

# Usual workdir
WORKDIR /app

# Copy all directory content
COPY . .

# Compile app
RUN mkdir build_docker && \
    cd build_docker && \
    cmake .. && \
    make

# RUN CLI app
ENTRYPOINT ["./build_docker/woven_challenge"]
CMD []
