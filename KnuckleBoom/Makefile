cxx = g++
CXXFLAGS = -Wall -Wextra
INCLUDEPATH = -I./lib -I./../lib/
DEPS = ./../lib/myTimeFunctions.h 
LIBS = -lm -lpthread  
OBJ = ./knuckle.o ./lib/dynfunctions.o ./lib/craneControllAndCom.o ./../lib/myTimeFunctions.o ./../lib/sendReceive.o ./lib/getMAandMom.o
TARGET =  knuckle.out

${TARGET}: ${OBJ} ${DEPS}
	${cxx} ${CXXFLAGS} -o $@ ${OBJ} ${LIBS}
	
clean:
	rm -f ${OBJ} *~
