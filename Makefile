.PHONY: all clean

SRC := main.cpp camera.cpp chunk.cpp controls.cpp pngLoad.cpp shader.cpp \
utils.cpp hud.cpp

BIN := vox.exe
OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

all: $(BIN)

CXXFLAGS := -MP -MMD -Wall -g $(shell freetype-config --cflags)
LDFLAGS := -Wall -lglfw -lGLEW -lpng -lftgl

-include $(DEP)

.cpp.o:
	@g++ $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	@g++ -o $@ $^ $(LDFLAGS)

clean:
	@rm -f $(BIN) $(OBJ)

