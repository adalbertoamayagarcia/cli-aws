# Overview
## Two bash scripts are provided:
  ### 1) run_solution.sh
    - Runs the CLI tool
  ### 2) run_test.sh
    - Generates: unit_test report, code coverage report and detailed design (doxygen)

## run_solution.sh script takes following arguments:
  - Arg 1  -> [local|docker] : Run application locally or using Docker.
  - Arg 2: -> [URL] : Web location of file to parse, ensure to use quotation marks ("").
  - Arg 3: -> [N] : Number of ids which should be reported to stdout, corresponding to the 'n' largest numbers.

  - ### AWS S3 URL -> "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt"

## run_test.sh script does not take arguments:
  - Note: By default will use hardcoded URL for simplicity/testing purposes, and run using docker.

## Project directory structure
  - design -> detailed design (updated after run_tests.sh execution)
  - include -> header files
  - src -> source code
  - testing -> unit test source code
  - reports -> testing results (updated after run_tests.sh execution)

# How-to
## Run CLI solution
  - ./run_solution.sh [local|docker] [URL] [N]
## Run Testing
  - ./run_test.sh

## Examples
### 1) Run CLI with Docker
  - ./run_solution.sh docker "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt" 5
### 2) Run CLI locally
  - ./run_solution.sh local "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt" 5
### 3) Testing (docker)
  - ./run_test.sh


## Constraints by Woven
  - Start reading at 500th byte
  - Input file hosted on AWS S3
  - Each line of the file will have the following form:
      <unique record identifier><white_space><numeric value>
  - Assume numeric value fits within a 32-bit unsigned int

## Considerations (Adalberto)
  - Tried to use an image with a small size but that still has required support and compatibility
  - Reduce data transfer over the network by reading chunks of data with curl (relevant when dealing with extremely large files)
  - Considered diferent DSA structures to avoid sorting (not-optimal), decided for a min-heap type, always keep size = N.
    and just replace top value if numeric_value greater than it.

### Pending
  - What happens if N is greater than the total number of rows in txt file? No protection at the moment...
