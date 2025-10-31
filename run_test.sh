#!/bin/bash

# Clear old data
rm -rf reports

docker build -t woven_test -f Dockerfile.testing .
docker run --rm woven_test


# Create stopped repo to pull test reports and design output
docker create --name temp woven_test
mkdir -p "reports"

docker cp temp:/app/design/output ./design/
docker cp temp:/app/coverage ./reports
docker cp temp:/app/unit_test ./reports/unit_test

docker rm temp
