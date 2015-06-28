
CXX = "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\cl.exe"
INCLUDE = ..\..\..\include

OBJ_DIR = build
BIN_DIR = bin

DIR_SRC = common

EXEC_NAME = meow.exe

SOURCES = $(DIR_SRC)\CxVector_tests.cpp

{$(DIR_SRC)}.cpp{$(OBJ_DIR)}.obj ::
	@echo Compiling...
	$(CXX) /c /EHsc /Fo$(OBJ_DIR)\ /I$(INCLUDE) $<

$(EXEC_NAME) : $(OBJ_DIR)\*.obj
	@echo Linking $(EXEC_NAME)...
	link /out:$(BIN_DIR)\$(EXEC_NAME) $(OBJ_DIR)\*.obj

test: $(EXEC_NAME)

create_dirs:
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

clean:
	@if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)
	@if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR)

all: create_dirs test

