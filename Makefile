
# Windows nmake makefile

CXX=cl
LINK=link

FLAGS=/EHsc /std:c++17 /I/lib/boost/ /I./src/ /c /Fo:./obj/
TESTFLAGS=/EHsc /std:c++17 /I/lib/boost/ /I./src/ /I./test/ /c /Fo:./obj/

SRC=./src/main.cpp ./src/board.cpp ./src/control.cpp ./src/game.cpp ./src/player.cpp ./src/ui.cpp
OBJ=./obj/main.obj ./obj/board.obj ./obj/control.obj ./obj/game.obj ./obj/player.obj ./obj/ui.obj

TESTSRC=./test/test.cpp ./src/board.cpp ./src/control.cpp ./src/game.cpp ./src/player.cpp ./src/ui.cpp
TESTOBJ=./obj/test.obj ./obj/board.obj ./obj/control.obj ./obj/game.obj ./obj/player.obj ./obj/ui.obj

INC=./src/board.hpp ./src/control.hpp ./src/game.hpp ./src/player.hpp ./src/ui.hpp ./src/menace.hpp
TESTINC=./test/test-board.hpp ./test/test-game.hpp ./test/test-player.hpp ./test/test-menace.hpp

TARGET=menace.exe
TEST=test.exe

all: dir $(TARGET)
test: dir $(TEST)

dir: 
	-@ if NOT EXIST "./bin/" mkdir "./bin/"
	-@ if NOT EXIST "./obj/" mkdir "./obj"

$(TARGET):	$(OBJ)
	$(LINK) /OUT:./bin/$(TARGET) $(OBJ)

$(TEST):	$(TESTOBJ)
	$(LINK) /OUT:./bin/$(TEST) $(TESTOBJ)

obj/main.obj: ./src/main.cpp $(INC)
  $(CXX) $(FLAGS) ./src/main.cpp

obj/board.obj: ./src/board.cpp ./src/board.hpp ./src/menace.hpp
  $(CXX) $(FLAGS) ./src/board.cpp

obj/control.obj: ./src/control.cpp $(INC)
  $(CXX) $(FLAGS) ./src/control.cpp

obj/game.obj: ./src/game.cpp ./src/board.hpp ./src/menace.hpp ./src/player.hpp
  $(CXX) $(FLAGS) ./src/game.cpp

obj/player.obj: ./src/player.cpp ./src/board.hpp
  $(CXX) $(FLAGS) ./src/player.cpp

obj/ui.obj: ./src/ui.cpp ./src/board.hpp ./src/menace.hpp
  $(CXX) $(FLAGS) ./src/ui.cpp

obj/test.obj: ./test/test.cpp $(INC) $(TESTINC)
  $(CXX) $(TESTFLAGS) ./test/test.cpp
