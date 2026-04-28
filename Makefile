CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -O2
LDFLAGS = -lgtest

TARGET = tests lunar-for-year

SRCS = tests.cpp LunarPhase.cpp lunar-for-year.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp LunarPhase.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
