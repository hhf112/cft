Occasional updates. <br>
<img src="https://img.shields.io/github/actions/workflow/status/hhf112/cft/c-cpp.yml">

- [ ] Automated test case fetching orchestration.
- [ ] Configurability.
- [x] build and run tests (error handling to be added. g++ error logs are displayed regardless)


## Usage 
1. running `cft init <number>`: 
    1. creates `<number>` files with alphabetical filename (a.cpp, b.cpp, c.cpp ...) 
    2. creates:
        1. `input.txt`: input to run the binaries
        2.  `out.txt`: expected output
        3. `output.txt`: output returned by the binary
        4. `report.txt`: comparision of your output and expected output by test number.
        3. `cl`: a bash script to clean up all the files created including the binaries built if any using the `cft` command (run `sh cl`)

2. running `cft <filename of a .cpp without extension>` anywhere in your terminal: 
    1. builds a binary with the same name
    2. copies the built binary to a convinient location for windows interface. (optional)
    2. runs the tests if `input.txt`, `out.txt` exist and are not empty.
    3. generates a log of tests in `report.txt` which compares outputs and test number.


## Installation
1. Download the prebuilt binary in `app/` directory and add it to your path.

#### OR

2. Clone the repo, cd into it and run `make`
#### build dependencies:
- C++23
- g++

