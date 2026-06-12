CXX ?= g++
CXXFLAGS ?= -O2 -std=c++17
TARGET = bin_packing_metaheuristic
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) 15

clean:
	rm -f $(TARGET)
