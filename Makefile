
CXXFLAGS=-Wall \
		-Werror \
		`pkg-config --cflags sdl` \
		-std=c++0x \
		-g -ggdb

LDFLAGS= \
		`pkg-config --libs sdl` -lSDL_ttf \
		-lboost_signals \
		-lboost_system \
		-lboost_filesystem \
		-g -ggdb

SRCS= \
	  map.cpp \
	  \
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
	  grid_surface.cpp \
	  grid.cpp \
	  \
	  color.cpp \
	  style.cpp \
	  \
	  surface.cpp \
	  text_surface.cpp \
	  font.cpp \
	  \
	  event.cpp \
	  gui.cpp \
	  gui_layout.cpp \
	  font_manager.cpp \
	  \
	  interface.cpp \
	  quest_interface.cpp \
	  engine.cpp \
	  quest_engine.cpp \
	  menu.cpp \
	  tortoise.cpp \
	  map_editor.cpp \
	  game.cpp \


OBJS=$(SRCS:.cpp=.o)

TARGET=test_map test_tree test_widget test_layout dotests quest

target: $(TARGET)

quest: quest.o $(OBJS)
	$(CXX) -o $@ quest.o $(OBJS) $(LDFLAGS)

dotests: dotests.o test_screen.o $(OBJS)
	$(CXX) -o $@ dotests.o test_screen.o $(OBJS) $(LDFLAGS)

test_layout: test_gui_layout.o $(OBJS)
	$(CXX) -o $@ test_gui_layout.o $(OBJS) $(LDFLAGS)

test_tree: test_tree.o node.o parent.o visitor.o
	$(CXX) -o $@ test_tree.o node.o parent.o visitor.o $(LDFLAGS)

test_map: test_map.o map.o
	$(CXX) -o $@ test_map.o map.o $(LDFLAGS)

test_widget: test_widget.o $(OBJS)
	$(CXX) -o $@ test_widget.o $(OBJS) $(LDFLAGS)

clean:
	-rm -rf $(OBJS) $(TARGET) $(TARGET:=.o) test_screen.o test_gui_layout.o
