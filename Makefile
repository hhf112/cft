SOURCES := $(wildcard ./src/*.cpp) ./main.cpp

CXX := g++ 
CXX_VERSION := -std=c++23 
MAKEFILE_DIR := $(shell pwd)
BIN := ./app/cft


install: build seed 
	@echo "Installation completed successfully."

build: ${BIN}

${BIN}: ${SOURCES}
	@echo "Building ..."
	@${CXX} ${CXX_VERSION} ${SOURCES} -o $@


seed:
	@echo "Installing ..."
	@sudo ln -sf $(MAKEFILE_DIR)/${BIN} /usr/local/bin/cft

clean:
	@echo "Uninstalling binary ..."
	@rm -f ${BIN}
	@sudo rm -f /usr/local/bin/cft
