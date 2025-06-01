Work in Progress in author's free time.

Currently only functions as a native linux CLI and only for C++ files. 

- [ ] Competitive Companion Orchestration
- [ ] Configurable build flags
- [ ] Configurable template
- [ ] Configurable file I/O
- [x] build .cpp files (error handling to be added. g++ error logs are displayed regardless)
- [x] run tests 
- [x] ship built binary to convinient location for windows interface

# CFTeser


## Usage 
1. running `cft init <number>: 
    1. creates <number> files with alphabetical filename (a.cpp, b.cpp, c.cpp ...) 
    2. creates:
        1. `input.txt`: input to run the binaries
        2.  `out.txt`: expected output
        3. `output.txt`: output returned by the binary
        4. `report.txt`: comparision of your output and expected output by test number.
        3. `cl`: a bash script to clean up all the files created including the binaries built if any using the `cft` command (run `sh cl`)

2. running `cft <filename of a .cpp without extension>` anywhere in your terminal: 
    1. builds a binary with the same name
    2. runs the tests if `input.txt`, `out.txt`exist and are not empty.


## Installation
1. Download the prebuilt binary in `app/` directory and add it to your path.

### OR

2. Clone the repo and cd into it and run make.

