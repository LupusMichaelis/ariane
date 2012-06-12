
CXXFLAGS=-Wall \
		-Werror \
		`pkg-config --cflags sdl` \
		-std=c++0x \
		-g -ggdb

LDFLAGS= \
		`pkg-config --libs sdl` -lSDL_ttf \
		-lboost_signals \
		-g -ggdb

SRCS= \
	  api.cpp \
	  videomode.cpp \
	  widget.cpp \
	  color.cpp \
	  surface.cpp \
	  style.cpp \
	  image.cpp \
	  grid.cpp \
	  event.cpp \
	  gui.cpp \

OBJS=$(SRCS:.cpp=.o)

TARGET=dotests tortoise quest

target: $(TARGET)

quest: quest.o $(OBJS)
	$(CXX) -o $@ quest.o $(OBJS) $(LDFLAGS)

tortoise: tortoise.o $(OBJS)
	$(CXX) -o $@ tortoise.o $(OBJS) $(LDFLAGS)

dotests: dotests.o test_screen.o $(OBJS)
	$(CXX) -o $@ dotests.o test_screen.o $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

clean:
	-rm -rf $(OBJS) $(TARGET) $(TARGET:=.o) test_screen.o
