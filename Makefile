
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
	  image.cpp \
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
