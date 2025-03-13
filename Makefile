CXX = g++
MINGW_CXX = x86_64-w32-mingw32-g++

TARGET_UNIX = quicksand
TARGET_WINDOWS = quicksand.exe

CPP_SOURCES = src/uci.cpp src/engine.cpp src/move/generateMoves.cpp src/move/makeMove.cpp src/move/unmakeMove.cpp src/move/magicBitboards.cpp src/eval/evaluateBoard.cpp src/utils/loadFen.cpp src/utils/annotateMove.cpp src/utils/popLSB.cpp src/utils/viewBoard.cpp

CPP_OBJECTS = $(CPP_SOURCES:.cpp=.o)
OBJECTS = $(CPP_OBJECTS)

%.o: %.cpp
	$(CXX) -c $< -o $@

%.win.o: %.cpp
	$(MINGW_CXX) -c $< -o $@

$(TARGET_UNIX): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET_UNIX)

win_objects = $(CPP_SOURCES:.cpp=.win.o)
$(TARGET_WINDOWS): $(win_objects)
	$(MINGW_CXX) $(win_objects) -o $(TARGET_WINDOWS)

clean:
	rm -f $(OBJECTS) $(win_objects) $(TARGET_UNIX) $(TARGET_WINDOWS)

.PHONY: all clean unix windows

all: unix windows

unix: $(TARGET_UNIX)

windows: $(TARGET_WINDOWS)
