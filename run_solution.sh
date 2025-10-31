#!/bin/bash

# ARGUMENTS: {1} {2} {3}
# 1 -> Mode = "local" or "docker"
# 2 = URL -> "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt"
# 3 = N

MODE=$1
shift # Dont want MODE passed to args "$@"

URL=$1
N=$2

if [ "$MODE" == "local" ]; then
    # Avoid installing if packages already exist, using cmake as example, perhaps not optimal...
    if [ ! command -y cmake &> /dev/null ]; then
        sudo apt-get update -y && sudo apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        g++ \
        libcurl4-openssl-dev
    fi
    
    # Avoid rebuild
    if [ ! -f build/woven_challenge ]; then
        mkdir -p build && cd build
        cmake ..
        make
        cd ..
    fi

    ./build/woven_challenge "$@"

elif [ "$MODE" == "docker" ]; then
    docker build -t woven_challenge -f Dockerfile . > /dev/null 2>&1 # Avoid unnecesary prints for clean output
    docker run --rm woven_challenge "$@"
else
    echo "Usage: ./run_solution.sh [local|docker] ["URL"] [N]"
fi
