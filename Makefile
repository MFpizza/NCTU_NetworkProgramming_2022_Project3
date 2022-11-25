CXX=g++
CXXFLAGS=-std=c++14 -Wall -pedantic -pthread -lboost_system
CXX_INCLUDE_DIRS=/usr/local/include 
CXX_INCLUDE_DIRS+=C:\MinGW\include
CXX_INCLUDE_PARAMS=$(addprefix -I , $(CXX_INCLUDE_DIRS))
CXX_LIB_DIRS=/usr/local/lib
CXX_LIB_DIRS+=C:\MinGW\lib
CXX_LIB_PARAMS=$(addprefix -L , $(CXX_LIB_DIRS))

part1: 
	$(CXX) source/part1/console.cpp -g -o console.cgi $(CXX_INCLUDE_PARAMS) $(CXX_LIB_PARAMS) $(CXXFLAGS)
	$(CXX) source/part1/http_server.cpp -g -o http_server $(CXX_INCLUDE_PARAMS) $(CXX_LIB_PARAMS) $(CXXFLAGS)

part2:
	$(CXX) source/part2/cgi_server.cpp source/part2/htmlSession.cpp source/part2/npSession.cpp source/part2/panel.cpp source/part2/server.cpp source/part2/console.cpp -g -o cgi_server $(CXX_INCLUDE_PARAMS) $(CXX_LIB_PARAMS) $(CXXFLAGS) -lwsock32 -lws2_32
	
clean:
	rm -f http_server
	rm -f console.cgi