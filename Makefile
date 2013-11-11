.PHONY: all clean

SRC := osg.cpp chunk.cpp hud.cpp resources.cpp

BIN := osg.exe
OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

all: $(BIN)

CXXFLAGS := -MP -MMD -Wall -g
LDFLAGS := -Wall -losg -losgViewer -losgGA -losgDB -losgUtil -losgText

-include $(DEP)

.cpp.o:
	@g++ $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	@g++ -o $@ $^ $(LDFLAGS)

clean:
	@rm -f $(BIN) $(OBJ)

