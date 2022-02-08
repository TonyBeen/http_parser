CC := g++
CXXFLAGS := -std=c++11 -Wall -g
SOFLAGS := -fPIC

RG := ragel

SRC_LIST := 

SRC_GENERATE := \
	http_response_parser.cpp

OBJ_LIST := \
	http_response_parser.o

INCLUDE := -I./

SO_PATH   := -L/usr/local/lib
SO_DEPEND :=

TARGET := libhttp_parser.so

$(TARGET) : $(OBJ_LIST)
	$(CC) $^ -o $@ $(SO_PATH) $(SO_DEPEND)

%.o : %.cpp
	$(CC) $^ -o $@ -c $(CXXFLAGS) $(INCLUDE) $(SOFLAGS)

http_response_parser.cpp : http_response_parser.rl
	$(RG) $^ -o $@

.PHONY: clean $(TARGET) all install uninstall

clean:
	-rm -rf $(OBJ_LIST) $(SRC_GENERATE)