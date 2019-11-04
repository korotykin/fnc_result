CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++11 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp,$(BIN)/%.o,$(SOURCES))
DEPENDS := $(patsubst $(SRC)/%.cpp,$(BIN)/%.d,$(SOURCES))

LIBRARIES	:=
EXECUTABLE	:= main

.PHONY: all clean

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

-include $(DEPENDS)

$(BIN)/%.o: $(SRC)/%.cpp Makefile
	$(CXX) -I$(INCLUDE) -MMD -MP -c $< -o $@

clean:
	-rm $(BIN)/*
