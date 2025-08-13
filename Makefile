CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra -pthread -MMD -MP -O2

SRC_DIR = src
BUILD_DIR = build
TARGET = my_redis_server

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean: 
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all 

run: all
	./$(TARGET)