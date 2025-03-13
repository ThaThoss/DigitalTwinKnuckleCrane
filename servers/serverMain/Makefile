cxx = g++
CXXFLAGS = -Wall 
INCLUDEPATH = -I./../lib
DEPS = ./../../lib/mySharedMemory.h ./../../lib/myPortsAndIP.h
LIBS = -lm -lpthread
OBJ = ./server.o ./../../lib/sendReceive.o ./../../lib/mySharedMemory.o 
TARGET =  ./../server.out

${TARGET}: ${OBJ} ${DEPS}
	${cxx} ${CXXFLAGS} -o $@ ${OBJ} ${LIBS}
	
clean:
	rm -f ${OBJ} *~
