
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
	  \
	  node.cpp \
	  parent.cpp \
	  visitor.cpp \
	  \
	  widget.cpp \
	  screen.cpp \
	  box.cpp \
	  text_box.cpp \
	  \
	  grid.cpp \
	  \
	  color.cpp \
	  style.cpp \
	  \
	  surface.cpp \
	  \
	  event.cpp \
	  gui.cpp \
	  gui_layout.cpp \

OBJS=$(SRCS:.cpp=.o)

TARGET=test_tree test_widget test_layout dotests tortoise #quest

target: $(TARGET)

quest: quest.o $(OBJS)
	$(CXX) -o $@ quest.o $(OBJS) $(LDFLAGS)

tortoise: tortoise.o $(OBJS)
	$(CXX) -o $@ tortoise.o $(OBJS) $(LDFLAGS)

dotests: dotests.o test_screen.o $(OBJS)
	$(CXX) -o $@ dotests.o test_screen.o $(OBJS) $(LDFLAGS)

test_layout: test_gui_layout.o grid.o surface.o gui_layout.o api.o style.o color.o videomode.o
	$(CXX) -o $@ test_gui_layout.o grid.o surface.o gui_layout.o api.o style.o color.o videomode.o $(LDFLAGS)

test_tree: test_tree.o node.o parent.o visitor.o
	$(CXX) -o $@  test_tree.o node.o parent.o visitor.o $(LDFLAGS)

test_widget: test_widget.o $(OBJS)
	$(CXX) -o $@  test_widget.o $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

clean:
	-rm -rf $(OBJS) $(TARGET) $(TARGET:=.o) test_screen.o test_gui_layout.o
