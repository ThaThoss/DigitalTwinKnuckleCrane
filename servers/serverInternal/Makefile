cxx = g++
CXXFLAGS = -Wall 
INCLUDEPATH = -I./../lib
DEPS = ./../../lib/mySharedMemory.h ./controller/xBoxComLoop.h ./knuckle/knuckleComLoop.h ./FEM/femComLoop.h ./../../lib/mySharedMemory.h ./../../lib/commonDataSizes.h ./../../lib/femClient.h ./../../lib/forceKnuckleToFEMLoop.h
LIBS = -lm -lpthread
OBJ = ./serverInternal.o ./FEM/femComLoop.o ./../../lib/sendReceive.o ./controller/xBoxComLoop.o ./knuckle/knuckleComLoop.o ./../../lib/mySharedMemory.o ./../../lib/femClient.o ./../../lib/forceKnuckleToFEMLoop.o
TARGET =  serverInternal.out

${TARGET}: ${OBJ} ${DEPS}
	${cxx} ${CXXFLAGS} -o $@ ${OBJ} ${LIBS}
	
clean:
	rm -f ${OBJ} *~

