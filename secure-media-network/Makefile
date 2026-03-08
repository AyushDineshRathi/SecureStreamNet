CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./protocol -I./security -I./network -I./common -I../include
LDFLAGS :=

ifeq ($(OS),Windows_NT)
LDFLAGS += -lws2_32
endif

# TODO(phase-2): Split build outputs into a dedicated build directory.
# TODO(phase-3): Re-introduce crypto/protocol objects once packet security is implemented.

all: client server

client:
	$(CXX) $(CXXFLAGS) client/client.cpp network/socket_utils.cpp -o client_app $(LDFLAGS)

server:
	$(CXX) $(CXXFLAGS) server/server.cpp network/socket_utils.cpp -o server_app $(LDFLAGS)

clean:
ifeq ($(OS),Windows_NT)
	-del /Q client_app.exe server_app.exe client_app server_app 2>NUL
else
	rm -f client_app server_app
endif
