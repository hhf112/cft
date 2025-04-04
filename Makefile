SOURCES := $(wildcard ./src/*.cpp) ./main.cpp

CXX := g++ 
CXX_VERSION := -std=c++23 
MAKEFILE_DIR := $(shell pwd)
BIN := ./app/cft

all: build install

build: ${BIN}

${BIN}: ${SOURCES}
	@echo "Building ..."
	@${CXX} ${CXX_VERSION} ${SOURCES} -o $@


install:
	@echo "Installing ..."
	@sudo ln -sf $(MAKEFILE_DIR)/${BIN} /usr/local/bin/cft
