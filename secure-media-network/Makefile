CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./protocol -I./security -I./network -I./common
LDFLAGS :=

# TODO(phase-2): Split build outputs into a dedicated build directory.
# TODO(phase-3): Re-introduce crypto/protocol objects once packet security is implemented.

all: client server

client:
	$(CXX) $(CXXFLAGS) client/client.cpp network/socket_utils.cpp -o client_app $(LDFLAGS)

server:
	$(CXX) $(CXXFLAGS) server/server.cpp network/socket_utils.cpp -o server_app $(LDFLAGS)

clean:
	rm -f client_app server_app
