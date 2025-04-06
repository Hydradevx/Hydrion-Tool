CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

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

clean:
	rm -rf $(BIN_DIR)
