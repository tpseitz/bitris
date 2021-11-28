CXX = g++
RM = rm -f
MKDIR = mkdir -p
RMDIR = rmdir -p
CPPFLAGS = -g -c -std=c++14
LDFLAGS_GUI=-g -lpthread -lcurses -lstdc++fs
LDFLAGS_TUI=-g -lpthread -lX11 -lGL -lpng -lstdc++fs
CPPFLAGS=-g -c -std=c++17

SOURCE = src
TARGET = bin


.PHONY: all clean


all: tetris tetristui test


tetristui: tetris.o tetris_tui.o
	$(CXX) $^ -o $(TARGET)/$@ $(LDFLAGS_GUI)


tetris: tetris.o tetris_gui.o
	$(CXX) $^ -o $(TARGET)/$@ $(LDFLAGS_TUI)

test: test.o
	$(CXX) $^ -o $(TARGET)/$@


%.o: $(SOURCE)/%.cpp $(TARGET)
	$(CXX) $< -o $@ $(CPPFLAGS)


$(TARGET):
	$(MKDIR) $(TARGET)


clean: $(DIRECTORIES)
	$(RM) *.o
	$(RM) $(TARGET)/*
	$(RMDIR) $(TARGET)

