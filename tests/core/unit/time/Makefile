CXX := g++
INCLUDE := -I../../../../include

releaseFlags := -Wall -02
debugFlags := -Wall -DDEBUG -g -fno-unsafe-loop-optimizations -fno-unroll-loops -fno-peel-loops -fno-move-loop-invariants -fno-unswitch-loops

EXTRAFLAGS :=

ifeq ($(MAKECMDGOALS), release)
CXXFLAGS := $(releaseFlags)
else 
CXXFLAGS := $(debugFlags)
endif

ifdef RELEASE
CXXFLAGS := $(releaseFlags)
else

endif
LDFLAGS := -L../../../../lib -lcatztoycore -lstdc++ -lc -framework CoreServices

OBJ_DIR := ../build/time
BIN_DIR := ../bin/time

TIME_TESTS := time_tests.cpp timekeeper_tests.cpp 
SOURCES := ${TIME_TESTS}
EXECUTABLES := $(SOURCES:%.cpp=%_TEST)
OBJECTS := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

all: dirs $(EXECUTABLES)

debug: dirs $(EXECUTABLES)

release: dirs $(EXECUTABLES)

%_TEST: $(OBJ_DIR)/%.o
	$(CXX) $(LDFLAGS) $< -o $(BIN_DIR)/$@

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(EXTRAFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/*
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)


.PHONY: dirs all

.PRECIOUS: $(OBJ_DIR)/%.o
