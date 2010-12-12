
CXXFLAGS=-Wall \
		-Werror \
		`pkg-config --cflags sdl`

LDFLAGS= \
		`pkg-config --libs sdl`

SRCS= \
	  surface.cpp \
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
