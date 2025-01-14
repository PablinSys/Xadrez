
$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(BUILD_DIR))

UTILS_DIR := utils
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
MAIN := main

OBJETOS := $(BUILD_DIR)/main.o $(BUILD_DIR)/tabuleiro.o $(BUILD_DIR)/peças.o $(BUILD_DIR)/gameUI.o $(BUILD_DIR)/gameController.o $(BUILD_DIR)/game.o $(BUILD_DIR)/bot.o $(BUILD_DIR)/algoritmo.o
CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

INCLUDE_DIRS := -I/usr/include -Iinclude
LIB_DIR := -L/usr/lib

LIBS := -lsfml-graphics -lsfml-window -lsfml-system

all: $(BIN_DIR)/$(MAIN)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/tabuleiro.o: $(SRC_DIR)/tabuleiro.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/tabuleiro.cpp -o $@

$(BUILD_DIR)/peças.o: $(SRC_DIR)/peças.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/peças.cpp -o $@

$(BUILD_DIR)/gameController.o: $(SRC_DIR)/gameController.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/gameController.cpp -o $@

$(BUILD_DIR)/gameUI.o: $(SRC_DIR)/gameUI.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/gameUI.cpp -o $@

$(BUILD_DIR)/game.o: $(SRC_DIR)/game.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/game.cpp -o $@

$(BUILD_DIR)/bot.o: $(SRC_DIR)/bot.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/bot.cpp -o $@

$(BUILD_DIR)/algoritmo.o: $(SRC_DIR)/algoritmo.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC_DIR)/algoritmo.cpp -o $@

$(BIN_DIR)/$(MAIN): $(OBJETOS)
	$(CXX) $(OBJETOS) $(LIB_DIR) $(LIBS) -o $@

# CLEAN
clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/$(MAIN)

.PHONY: all clean
