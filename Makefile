
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

#	  quest.cpp \

OBJS=$(SRCS:.cpp=.o)

TARGET=dotests

target: $(TARGET)

quest: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

dotests: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

clean:
	-rm -rf $(OBJS) $(TARGET)
