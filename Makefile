CXX = g++
CXXFLAGS = -Wall -std=c++17

SRC_DIR = src
BIN_DIR = bin

TARGET = $(BIN_DIR)/discordtool
UPDATER = $(BIN_DIR)/updater

SRCS = $(SRC_DIR)/main.cpp
UPDATER_SRC = $(SRC_DIR)/updater.cpp

all: $(TARGET) $(UPDATER)

$(TARGET): $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(UPDATER): $(UPDATER_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: all
	./$(TARGET)

update: $(UPDATER)
	./$(UPDATER)

clean:
	rm -rf $(BIN_DIR)
