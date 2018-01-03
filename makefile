CC=gcc
CFLAGS= -c -DSFML_STATIC -std=c++14 -Wall -g3 -pthread -IC:\libraries\SFML-2.4.2_gcc64\include
LIBFLAGS=-LC:/libraries/SFML-2.4.2_gcc64/lib
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lstdc++

SOURCES=main.cpp cache.cpp keyboard.cpp tiles.cpp \
geometry.cpp object.cpp graphics_handler.cpp control_handler.cpp \
interact_handler.cpp object_collection.cpp surface.cpp world.cpp \
map_collection.cpp map_chank.cpp gui.cpp mouse.cpp interact.cpp \
interact_gui.cpp services.cpp graphics.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=map

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE):$(OBJECTS) 
	$(CC) $(LIBFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *.o	 $(EXECUTABLE)