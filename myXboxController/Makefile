cxx = g++
CXXFLAGS = -Wall 
INCLUDEPATH = -I./../lib
DEPS = ./../lib/sendReceive.h ./myXboxController.h ./../lib/myLinearMapping.h ./../lib/femClient.h
LIBS = -lm -lpthread
OBJ = ./myXboxController.o ./../lib/sendReceive.o ./../lib/myLinearMapping.o ./../lib/femClient.o
TARGET =  xbox.out

${TARGET}: ${OBJ} ${DEPS}
	${cxx} ${CXXFLAGS} -o $@ ${OBJ} ${LIBS}
	
clean:
	rm -f ${OBJ} *~
