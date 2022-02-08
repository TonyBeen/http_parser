CC := g++
CXXFLAGS := -std=c++11 -Wall -g
SOFLAGS := -fPIC

RG := ragel

INSTALL_PATH := /usr/local

SRC_GENERATE := \
	http_response_parser.cpp \
	http_request_parser.cpp

OBJ_LIST := \
	http_response_parser.o \
	http_request_parser.o

HEADER_LIST := $(wildcard ./*.h)

INCLUDE := -I./

SO_PATH   := -L/usr/local/lib
SO_DEPEND :=

TARGET := libhttp_parser.so

default : $(TARGET)

install : $(TARGET)
	-if [ ! -d "$(INSTALL_PATH)/include/http_parser/" ]; then sudo mkdir $(INSTALL_PATH)/include/http_parser/; fi
	-sudo cp -r -a $(HEADER_LIST) $(INSTALL_PATH)/include/http_parser
	-sudo mv $(TARGET) $(INSTALL_PATH)/lib
	-sudo ldconfig

$(TARGET) : $(OBJ_LIST)
	$(CC) $^ -o $@ -shared $(SO_PATH) $(SO_DEPEND)
	rm -rf $(OBJ_LIST) $(SRC_GENERATE)

%.o : %.cpp
	$(CC) $^ -o $@ -c $(CXXFLAGS) $(INCLUDE) $(SOFLAGS)

http_response_parser.cpp : http_response_parser.rl
	$(RG) $^ -o $@
http_request_parser.cpp : http_request_parser.rl
	$(RG) $^ -o $@

test :
	make test_parse_response
	make test_parse_request

# 生成测试
test_parse_response : test_parse_response.cc http_response_parser.cpp
	g++ $^ -o $@
	-rm -rf http_response_parser.cpp

test_parse_request : test_parse_request.cc http_request_parser.cpp
	g++ $^ -o $@
	-rm -rf http_request_parser.cpp

.PHONY: clean $(TARGET) all install uninstall

clean:
	-rm -rf $(OBJ_LIST) $(SRC_GENERATE) test_parse_response test_parse_request