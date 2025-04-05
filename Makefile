CXX = g++
CXXFLAGS = -Wall -std=c++17

SRC_DIR = src
BIN_DIR = build

TARGET = $(BIN_DIR)/discordtool

SRCS = $(SRC_DIR)/*.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: all
	./$(TARGET)

update: $(UPDATER)
	./$(UPDATER)

clean:
	rm -rf $(BIN_DIR)
