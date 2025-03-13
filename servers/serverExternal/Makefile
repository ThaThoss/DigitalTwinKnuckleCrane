CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra -I/usr/local/include


LDFLAGS = -L/usr/local/lib -lixwebsocket -lz -lssl -lcrypto -lpthread


OBJ =  serverExternal.o ./../../lib/sendReceive.o ./../../lib/mySharedMemory.o
TARGET = serverExternal.out

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
