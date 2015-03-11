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
LDFLAGS := -L../../../../lib -lcatztoycore -lstdc++ -lc -lpthread

OBJ_DIR := ../build/threading
BIN_DIR := ../bin/threading

THREADING_TESTS := mutex_tests.cpp spinlock_tests.cpp conditionvariable_tests.cpp thread_tests.cpp asynctaskrunner_tests.cpp asynctask_tests.cpp threadmanager_tests.cpp asyncresult_tests.cpp runnable_tests.cpp

PROCESS_TESTS := process_tests.cpp processqueue_tests.cpp processmanagersinglethread_tests.cpp processmanagermultithread_tests.cpp

TASK_TESTS := task_tests.cpp taskrunnersinglethread_tests.cpp taskrunnermultithread_tests.cpp taskmanager_tests.cpp

SOURCES := ${THREADING_TESTS} ${PROCESS_TESTS} ${TASK_TESTS}
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
