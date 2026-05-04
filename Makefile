CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -pedantic -O2 -MMD -MP -fPIC
CXXFLAGS += $(shell pkg-config --cflags Qt6Widgets)

BUILD_DIR := build/

# Targets
TARGETS := tests lunar-for-year lunar-calendar-qt

# Source files
TEST_SRCS := src/tests.cpp
LUNAR_SRCS := src/lunar-for-year.cpp
LUNAR_CAL_SRCS := src/lunar-calendar-qt.cpp

# Object files
TEST_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))
LUNAR_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(LUNAR_SRCS))
LUNAR_CAL_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(LUNAR_CAL_SRCS))
# Dependency files
DEPS := $(TEST_OBJS:.o=.d) $(LUNAR_OBJS:.o=.d) $(LUNAR_CAL_OBJS:.o=.d)

# Default target
all: $(TARGETS)

# Link targets
tests: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgtest

lunar-for-year: $(LUNAR_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

lunar-calendar-qt: $(LUNAR_CAL_OBJS)
	$(CXX) $(CXXFLAGS) $(shell pkg-config --libs Qt6Widgets) -o $@ $^

# Compile rule
$(BUILD_DIR)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files if they exist
-include $(DEPS)

# Clean
clean:
	rm -f $(BUILD_DIR)/* $(TARGETS)

rebuild: clean all

.PHONY: all clean rebuild
