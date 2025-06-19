
Occasional updates for features. <br>

- [ ] Automated test case fetching orchestration.
- [ ] Configurability.
- [x] build and run tests (error handling to be added. g++ error logs are displayed regardless)

# CFTester
[!build status](https://img.shields.io/github/actions/workflow/status/hhf112/cft/c-cpp.yml)
.cpp file workflow for running I/O tests focused towards CP. 

## Usage 
```
usage: cft <command> [option]

<command>:
init [n]            touch input.txt
                    touch out.txt in
                    touch output.txt
                    touch report.txt
                    create cl 
                    create [n] ./[a-z].cpp template(s) 
                    by default n = 1

<file>              build ./<file>.cpp
                    run ./<file>.cpp
                    run tests on ./<file>.cpp
                    logs in terminal and report.txt
```

## Installation
1. Download the prebuilt binary in `app/` directory and add it to your path.

#### OR
2. Clone the repo, cd into it and run `make`
#### build dependencies:
- C++23
- g++

