
CXXFLAGS=-Wall \
		-Werror \
		`pkg-config --cflags sdl` \
		-std=c++0x \
		-g -ggdb \
		-Isrc/ \

LDFLAGS= \
		`pkg-config --libs sdl` -lSDL_ttf \
		-lboost_signals \
		-lboost_system \
		-lboost_filesystem \
		-g -ggdb \

SRCS= \
	  src/map.cpp \
	  \
	  src/api.cpp \
	  src/videomode.cpp \
	  \
	  src/node.cpp \
	  src/parent.cpp \
	  src/visitor.cpp \
	  \
	  src/widget.cpp \
	  src/screen.cpp \
	  src/box.cpp \
	  src/text_box.cpp \
	  src/image_box.cpp \
	  \
	  src/grid_surface.cpp \
	  src/grid.cpp \
	  \
	  src/color.cpp \
	  src/style.cpp \
	  \
	  src/surface.cpp \
	  src/text_surface.cpp \
	  src/font.cpp \
	  \
	  src/event.cpp \
	  src/gui.cpp \
	  src/gui_layout.cpp \
	  src/font_manager.cpp \
	  src/motif_library.cpp \
	  \
	  src/interface.cpp \
	  src/quest_interface.cpp \
	  src/engine.cpp \
	  src/quest_engine.cpp \
	  src/menu.cpp \
	  src/tortoise.cpp \
	  src/adventure.cpp \
	  src/map_editor.cpp \


OBJS=$(SRCS:.cpp=.o)

TARGET=test_map test_tree test_widget test_layout dotests quest

target: $(TARGET)

quest: quest.o $(OBJS)
	$(CXX) -o $@ quest.o $(OBJS) $(LDFLAGS)

dotests: dotests.o test_screen.o $(OBJS)
	$(CXX) -o $@ dotests.o test_screen.o $(OBJS) $(LDFLAGS)

test_layout: test_gui_layout.o $(OBJS)
	$(CXX) -o $@ test_gui_layout.o $(OBJS) $(LDFLAGS)

test_tree: test_tree.o src/node.o src/parent.o src/visitor.o
	$(CXX) -o $@ test_tree.o src/node.o src/parent.o src/visitor.o $(LDFLAGS)

test_map: test_map.o src/map.o
	$(CXX) -o $@ test_map.o src/map.o $(LDFLAGS)

test_widget: test_widget.o $(OBJS)
	$(CXX) -o $@ test_widget.o $(OBJS) $(LDFLAGS)

clean:
	-$(RM) $(OBJS) $(TARGET) $(TARGET:=.o) test_screen.o test_gui_layout.o
