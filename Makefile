.PHONY: all clean

SRC := main.cpp pngLoad.cpp camera.cpp controls.cpp shader.cpp utils.cpp

BIN := vox.exe
OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

all: $(BIN)

CXXFLAGS := -MP -MMD -Wall -g
LDFLAGS := -Wall -lglfw -lGLEW -lpng

-include $(DEP)

.cpp.o:
	@g++ $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	@g++ $(LDFLAGS) -o $@ $^

clean:
	@rm -f $(BIN) $(OBJ)

