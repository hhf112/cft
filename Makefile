CXX := g++
CXXFLAGS := -std=c++23 

MAKEFILE_DIR := $(shell pwd)
BIN := ./app/cft

SOURCES := $(wildcard ./src/*.cpp) ./util/misc.cpp ./main.cpp
OBJECTS := $(SOURCES:.cpp=.o)


SYM_LINK := /usr/local/bin/cft

all: install

install: build seed
	@echo ""
	@echo ""
	@echo " ██████╗███████╗████████╗███████╗███████╗████████╗███████╗██████╗      ██╗ ██╗ "
	@echo "██╔════╝██╔════╝╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██╔════╝██╔══██╗    ████████╗"
	@echo "██║     █████╗     ██║   █████╗  ███████╗   ██║   █████╗  ██████╔╝    ╚██╔═██╔╝"
	@echo "██║     ██╔══╝     ██║   ██╔══╝  ╚════██║   ██║   ██╔══╝  ██╔══██╗    ████████╗"
	@echo "╚██████╗██║        ██║   ███████╗███████║   ██║   ███████╗██║  ██║    ╚██╔═██╔╝"
	@echo " ╚═════╝╚═╝        ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝     ╚═╝ ╚═╝ "
	@echo "                                                                               "
	@echo "\t Installation completed successfully."
	

build: $(BIN)

$(BIN): $(OBJECTS)
	@echo "Linking binary..."
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

seed: 
	@echo "Seeding path ..."
	@sudo ln -sf $(MAKEFILE_DIR)/$(BIN) $(SYM_LINK)

clean:
	@echo "Cleaning up..."
	@rm -f $(OBJECTS) $(BIN)
	@sudo rm -f /usr/local/bin/cft

.PHONY: all install build seed clean
