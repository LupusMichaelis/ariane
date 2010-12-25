
CXXFLAGS=-Wall \
		-Werror \
		`pkg-config --cflags sdl` \
		-g -ggdb

LDFLAGS= \
		`pkg-config --libs sdl` \
		-g -ggdb

SRCS= \
	  color.cpp \
	  surface.cpp \
	  canvas.cpp \
	  screen.cpp \
	  test_screen.cpp \

OBJS=$(SRCS:.cpp=.o)

TARGET=test_screen

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

test_screen: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -rf $(OBJS) $(TARGET)
