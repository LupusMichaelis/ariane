
CXXFLAGS=-Wall \
		-Werror \
		`pkg-config --cflags sdl` \
		-std=c++0x \
		-g -ggdb

LDFLAGS= \
		`pkg-config --libs sdl` \
		-lboost_signals \
		-g -ggdb

SRCS= \
	  color.cpp \
	  surface.cpp \
	  screen.cpp \
	  canvas.cpp \
	  image.cpp \
	  grid.cpp \
	  event.cpp \

#	  quest.cpp \

OBJS=$(SRCS:.cpp=.o)

TARGET=dotests tortoise

target: $(TARGET)

quest: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

tortoise: tortoise.o $(OBJS)
	$(CXX) -o $@ tortoise.o $(OBJS) $(LDFLAGS)

dotests: dotests.o test_screen.o $(OBJS)
	$(CXX) -o $@ dotests.o test_screen.o $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

clean:
	-rm -rf $(OBJS) $(TARGET)
