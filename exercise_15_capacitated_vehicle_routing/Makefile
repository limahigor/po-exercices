CXX ?= g++
CXXFLAGS ?= -O2 -std=c++17 -DIL_STD
CPLEX_STUDIO ?=

ifeq ($(strip $(CPLEX_STUDIO)),)
$(error Set CPLEX_STUDIO to your IBM ILOG CPLEX Optimization Studio path, for example: make CPLEX_STUDIO=/path/to/CPLEX_Studio2211)
endif

CONCERT_INC = $(CPLEX_STUDIO)/concert/include
CPLEX_INC = $(CPLEX_STUDIO)/cplex/include
CONCERT_LIB = $(CPLEX_STUDIO)/concert/lib/x86-64_linux/static_pic
CPLEX_LIB = $(CPLEX_STUDIO)/cplex/lib/x86-64_linux/static_pic
TARGET = main
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(CONCERT_INC) -I$(CPLEX_INC) $(SRC) -L$(CONCERT_LIB) -L$(CPLEX_LIB) -lilocplex -lconcert -lcplex -lm -lpthread -ldl -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
