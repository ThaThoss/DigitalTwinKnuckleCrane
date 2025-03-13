cxx = g++
CXXFLAGS = -Wall 
INCLUDEPATH = -I./../lib
DEPS =  ./femComLoop.h ./../lib/sendReceive.h ./../lib/mySharedMemory.h ./../lib/femClient.h
LIBS = -lm -lpthread
OBJ = ./femMultiTest.o ./femComLoop.o ./../lib/sendReceive.o ./../lib/femClient.o ./../lib/mySharedMemory.o 
TARGET =  femMultiTest.out

${TARGET}: ${OBJ} ${DEPS}
	${cxx} ${CXXFLAGS} -o $@ ${OBJ} ${LIBS}
	
clean:
	rm -f ${OBJ} *~

