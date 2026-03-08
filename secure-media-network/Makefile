CXX ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra
CPPFLAGS := -I./protocol -I./security -I./network -I./common -I../include
LDFLAGS :=
LDLIBS := -lssl -lcrypto
MSYS2_BIN := C:/msys64/mingw64/bin

ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
.SHELLFLAGS := /C
EXE := .exe
LDLIBS += -lws2_32
ifneq ("$(wildcard C:/msys64/mingw64/bin/g++.exe)","")
CXX := C:/msys64/mingw64/bin/g++.exe
endif
else
EXE :=
endif

COMMON_SOURCES := \
	network/socket_utils.cpp \
	protocol/serializer.cpp \
	security/aes_encrypt.cpp \
	security/sha256.cpp

CLIENT_SOURCES := client/client.cpp $(COMMON_SOURCES)
SERVER_SOURCES := server/server.cpp $(COMMON_SOURCES)

CLIENT_BIN := client_app$(EXE)
SERVER_BIN := server_app$(EXE)

all: client server

msys-build:
	set "PATH=$(MSYS2_BIN);%PATH%" && $(MSYS2_BIN)/g++.exe $(CPPFLAGS) $(CXXFLAGS) $(CLIENT_SOURCES) -o client_app.exe $(LDFLAGS) $(LDLIBS)
	set "PATH=$(MSYS2_BIN);%PATH%" && $(MSYS2_BIN)/g++.exe $(CPPFLAGS) $(CXXFLAGS) $(SERVER_SOURCES) -o server_app.exe $(LDFLAGS) $(LDLIBS)

client:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CLIENT_SOURCES) -o $(CLIENT_BIN) $(LDFLAGS) $(LDLIBS)

server:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SERVER_SOURCES) -o $(SERVER_BIN) $(LDFLAGS) $(LDLIBS)

clean:
ifeq ($(OS),Windows_NT)
	-del /Q client_app.exe server_app.exe client_app server_app 2>NUL
else
	rm -f client_app server_app
endif
