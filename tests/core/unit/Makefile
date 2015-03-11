CXX := g++
INCLUDE := -I../../../include

releaseFlags := -Wall -02
debugFlags := -Wall -DDEBUG -g -fno-unsafe-loop-optimizations -fno-unroll-loops -fno-peel-loops -fno-move-loop-invariants -fno-unswitch-loops


ifeq ($(MAKECMDGOALS), release)
CXXFLAGS := $(releaseFlags)
else 
CXXFLAGS := $(debugFlags)
endif

ifdef RELEASE
CXXFLAGS := $(releaseFlags)
else

endif
LDFLAGS := -L../../../lib -lcatztoycore -lstdc++ -lc

OBJ_DIR := build
BIN_DIR := bin

UTIL_TESTS := sharedptr_tests.cpp vector_tests.cpp list_tests.cpp objlist_tests.cpp objmap_tests.cpp
MEMORY_TESTS := memorymanager_tests.cpp poolmemoryallocator_tests.cpp stackmemoryallocator_tests.cpp chunkmemoryallocator_tests.cpp dynamicchunkmemoryallocator_tests.cpp
MATH_TESTS := vec3_tests.cpp vec4_tests.cpp mat3_tests.cpp mat4_tests.cpp quaternion_tests.cpp angle_tests.cpp
THREADING_TESTS := mutex_tests.cpp spinlock_tests.cpp conditionvariable_tests.cpp thread_tests.cpp asynctaskrunner_tests.cpp asynctask_tests.cpp threadmanager_tests.cpp asyncresult_tests.cpp runnable_tests.cpp
IO_TESTS := file_tests.cpp filedescriptor_tests.cpp fileinputstream_tests.cpp fileoutputstream_tests.cpp 
ASYNC_IO_TESTS := iomanager_tests.cpp asyncinputstream_tests.cpp asyncdatainputstream_tests.cpp asyncobjectinputstream_tests.cpp asyncoutputstream_tests.cpp asyncdataoutputstream_tests.cpp asyncobjectoutputstream_tests.cpp
GEOMETRY_TESTS := convexpoly2_tests.cpp

SOURCES := ${UTIL_TESTS} ${GEOMETRY_TESTS} ${MEMORY_TESTS} ${MATH_TESTS} ${THREADING_TESTS} ${IO_TESTS} ${ASYNC_IO_TESTS}
EXECUTABLES := $(SOURCES:%.cpp=%_TEST)
OBJECTS := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

all: dirs $(EXECUTABLES)

debug: dirs $(EXECUTABLES)

release: dirs $(EXECUTABLES)

%_TEST: $(OBJ_DIR)/%.o
	$(CXX) $(LDFLAGS) $< -o $(BIN_DIR)/$@

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/*

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

.PHONY: dirs all

.PRECIOUS: $(OBJ_DIR)/%.o
