
Occasional updates for features. <br>

- [x] Comeptitive Comapnion orchestration  
- [ ] Configurability.
- [x] build and run tests

# CFTester <img src ="https://img.shields.io/github/actions/workflow/status/hhf112/cft/c-cpp.yml" alt="build status">
.cpp file workflow for running I/O tests focused towards CP. 

## Installation

## Usage 
```
usage: cft <command> [option]

<command>:
init [n]            touch input.txt
                    touch out.txt in
                    touch output.txt
                    touch report.txt
                    ./cpmp
                    touch logs.txt
                    create cl 
                    create [n] ./[a-zA-Z].cpp template(s) 
                    {default n = 1}

<name>              build ./<name>.cpp
                    run tests -> ./<name>.cpp
                    logs: terminal and report.txt
```

## Installation
1. Download the prebuilt binary in `app/` directory and add it to your path.

#### OR
2. Clone the repo, cd into it and run `make`

#### external dependencies: (could implement custom http server handling for optimizing binary size)
- https://github.com/nlohmann/json
- https://github.com/yhirose/cpp-httplib
#### build dependencies:
- c++20
- g++

