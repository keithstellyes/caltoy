CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -pedantic -O2 -MMD -MP

# Targets
TARGETS := tests lunar-for-year

# Source files
TEST_SRCS := tests.cpp
LUNAR_SRCS := lunar-for-year.cpp

# Object files
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
LUNAR_OBJS := $(LUNAR_SRCS:.cpp=.o)

# Dependency files
DEPS := $(TEST_OBJS:.o=.d) $(LUNAR_OBJS:.o=.d)

# Default target
all: $(TARGETS)

# Link targets
tests: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgtest

lunar-for-year: $(LUNAR_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files if they exist
-include $(DEPS)

# Clean
clean:
	rm -f *.o *.d $(TARGETS)

rebuild: clean all

.PHONY: all clean rebuild
