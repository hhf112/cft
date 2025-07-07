
Occasional updates for features. <br>

- [ ] custom http implementation for lightweight binary
- [ ] custom json parser for lightweight binary
- [x] Comeptitive Comapnion orchestration  
- [ ] Configurability.
- [x] build and run tests

# CFTester <img src ="https://img.shields.io/github/actions/workflow/status/hhf112/cft/c-cpp.yml" alt="build status">
.cpp file workflow for running I/O tests focused towards CP. 

## Installation
bash
```bash
curl -sSL  https://raw.githubusercontent.com/hhf112/cft/refs/heads/master/install.sh | bash
```

## Usage 
```
usage: cft <command> [option]

<command>:
init [n]            touch in.txt
                    touch out.txt in
                    touch output.txt
                    touch report.txt
                    ./cpmp [start competitive companion server]
                    create cl 
                    create [n] ./[a-zA-Z].cpp template(s) 
                    {default n = 1}

<name>              g++  ./<name>.cpp -o <name>
                    ./<name>.cpp
```


#### build dependencies:
- c++20
- g++
- [competitive comapnion](https://github.com/jmerle/competitive-companion#)

