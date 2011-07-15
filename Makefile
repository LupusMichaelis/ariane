
CXXFLAGS=-Wall \
		-Werror \
		`pkg-config --cflags sdl` \
		-std=c++0x \
		-g -ggdb

LDFLAGS= \
		`pkg-config --libs sdl` \
		-g -ggdb

SRCS= \
	  color.cpp \
	  surface.cpp \
	  screen.cpp \
	  canvas.cpp \
	  image.cpp \
	  grid.cpp \
	  test_screen.cpp \
	  dotests.cpp \

OBJS=$(SRCS:.cpp=.o)

TARGET=dotests

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

dotests: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -rf $(OBJS) $(TARGET)
