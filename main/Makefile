CXX ?= g++
CXXFLAGS ?= -O2 -std=c++17 -DIL_STD
CPPFLAGS += -DBUILD_ALL_EXERCISES
CPLEX_STUDIO ?=

ifeq ($(strip $(CPLEX_STUDIO)),)
$(error Set CPLEX_STUDIO to your IBM ILOG CPLEX Optimization Studio path, for example: make CPLEX_STUDIO=/path/to/CPLEX_Studio2211)
endif

CONCERT_INC = $(CPLEX_STUDIO)/concert/include
CPLEX_INC = $(CPLEX_STUDIO)/cplex/include
CONCERT_LIB = $(CPLEX_STUDIO)/concert/lib/x86-64_linux/static_pic
CPLEX_LIB = $(CPLEX_STUDIO)/cplex/lib/x86-64_linux/static_pic
OUTPUT = run_cplex
SRC = main.cpp \
	../exercise_01_feed_mix/main.cpp \
	../exercise_02_diet_problem/main.cpp \
	../exercise_03_crop_planning/main.cpp \
	../exercise_04_paint_blending/main.cpp \
	../exercise_05_transportation/main.cpp \
	../exercise_06_max_flow/main.cpp \
	../exercise_07_shift_scheduling/main.cpp \
	../exercise_08_set_covering/main.cpp \
	../exercise_09_binary_knapsack/main.cpp \
	../exercise_10_maximum_clique/main.cpp \
	../exercise_11_cutting_patterns/main.cpp \
	../exercise_12_facility_location/main.cpp \
	../exercise_13_graph_coloring/main.cpp \
	../exercise_14_traveling_salesman/main.cpp \
	../exercise_15_capacitated_vehicle_routing/main.cpp \
	../exercise_16_bin_packing/main.cpp \
	../exercise_17_shortest_path/main.cpp

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(CONCERT_INC) -I$(CPLEX_INC) $(SRC) -L$(CONCERT_LIB) -L$(CPLEX_LIB) -lilocplex -lconcert -lcplex -lm -lpthread -ldl -o $(OUTPUT)

run: $(OUTPUT)
	./$(OUTPUT)

clean:
	rm -f $(OUTPUT)
