# Compiler & flags
CXX := g++
CXXFLAGS := -std=c++23 #And more if you wish

# Directories and target
MAKEFILE_DIR := $(shell pwd)
BIN := ./app/cft

# Source and object files
SOURCES := $(wildcard ./src/*.cpp) ./util/misc.cpp ./main.cpp
OBJECTS := $(SOURCES:.cpp=.o)


#Sym-link to be created.
SYM_LINK := /usr/local/bin/cft

all: install

install: build seed
	@echo "Installation completed successfully."

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
