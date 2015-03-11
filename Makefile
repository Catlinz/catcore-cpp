CXX := g++
INCLUDE := -Iinclude
LIBS := -lpthread

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


OBJ_DIR := build
LIB_DIR := lib
INC_DIR := include
SRC_DIR := src

LIB := $(LIB_DIR)/libcatztoycore.a

CORE_SRC := core/corelib.cpp

UTIL_SRC := core/util/sharedptr.cpp core/util/vector.cpp core/util/list.cpp core/util/map.cpp core/util/array.cpp core/util/staticmap.cpp core/util/invasivestrongptr.cpp core/util/simplequeue.cpp core/util/internalmessage.cpp core/util/datanode.cpp core/util/datanodepool.cpp core/util/ptrnode.cpp core/util/ptrnodestore.cpp core/util/namegenerator.cpp core/util/stack.cpp core/util/datablob.cpp

STRING_SRC := core/string/hungrystring.cpp core/string/stringutils.cpp core/string/string.cpp core/string/unistring.cpp

MEMORY_SRC := core/memory/memorymanager.cpp core/memory/memoryallocator.cpp core/memory/poolmemoryallocator.cpp core/memory/stackmemoryallocator.cpp core/memory/chunkmemoryallocator.cpp core/memory/dynamicchunkmemoryallocator.cpp

MATH_SRC := core/math/mathcore.cpp core/math/vec2f.cpp core/math/vec3.cpp core/math/vec4.cpp core/math/mat3.cpp core/math/mat4.cpp core/math/quaternion.cpp core/math/angle.cpp

THREAD_SRC := core/threading/threaddefs.cpp core/threading/mutex.cpp core/threading/runnable.cpp core/threading/spinlock.cpp core/threading/conditionvariable.cpp core/threading/thread.cpp core/threading/threadmanager.cpp core/threading/asynctaskrunner.cpp core/threading/asynctask.cpp core/threading/asyncrunnable.cpp core/threading/asyncresult.cpp core/threading/atomic.cpp

PROCESS_SRC := core/threading/process.cpp core/threading/processqueue.cpp core/threading/processrunner.cpp core/threading/processmanager.cpp

TASK_SRC := core/threading/task.cpp core/threading/taskqueuenode.cpp core/threading/taskrunner.cpp core/threading/taskmanager.cpp

IO_SRC := core/io/filepath.cpp core/io/file.cpp core/io/filedescriptor.cpp core/io/datainputstream.cpp core/io/dataoutputstream.cpp core/io/fileinputstream.cpp core/io/fileoutputstream.cpp core/io/serialiser.cpp

ASYNC_IO_SRC := core/io/iomanager.cpp core/io/asyncinputtask.cpp core/io/asyncinputstream.cpp core/io/asyncdatainputstream.cpp core/io/asyncobjectinputstream.cpp core/io/asyncoutputtask.cpp core/io/asyncoutputstream.cpp core/io/asyncdataoutputstream.cpp core/io/asyncobjectoutputstream.cpp

GEOMETRY_SRC := core/geometry/point2i.cpp core/geometry/point2f.cpp core/geometry/recti.cpp core/geometry/rectf.cpp core/geometry/size2i.cpp core/geometry/size2f.cpp core/geometry/convexpoly2f.cpp core/geometry/convexpoly2i.cpp

TIME_SRC := core/time/time.cpp core/time/timekeeper.cpp core/time/clock.cpp

DEFER_SRC := core/defer/message.cpp core/defer/messagehandler.cpp  core/defer/messagequeue.cpp core/defer/timedaction.cpp core/defer/timer.cpp core/defer/deferredaction.cpp core/defer/deferredexec.cpp

SIGNAL_SRC := core/signal/signaldata.cpp core/signal/signalhandler.cpp core/signal/signalemitter.cpp

SYSTEM_SRC := core/sys/system.cpp

COLOR_SRC := core/color/color4b.cpp core/color/color4f.cpp core/color/packedcolor.cpp

EVENT_SRC := core/event/event.cpp core/event/eventqueue.cpp

SOURCES := ${CORE_SRC} ${UTIL_SRC} ${STRING_SRC} ${MEMORY_SRC} ${MATH_SRC} ${THREAD_SRC} ${PROCESS_SRC} ${TASK_SRC} ${IO_SRC} ${ASYNC_IO_SRC} ${GEOMETRY_SRC} ${TIME_SRC} ${DEFER_SRC} ${SIGNAL_SRC} ${SYSTEM_SRC} ${COLOR_SRC} ${EVENT_SRC}
OBJECTS := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

all: dirs $(LIB)

debug: dirs $(LIB)
release: dirs $(LIB)

$(LIB): $(OBJECTS)
	ar rcs $(LIB) $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(EXTRAFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/core/*
	rm -f $(LIB)

dirs:
	mkdir -p $(LIB_DIR)
	mkdir -p $(OBJ_DIR)/core

.PHONY: dirs all
