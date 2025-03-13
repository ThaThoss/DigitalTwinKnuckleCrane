CC = gcc

LIBS = -lm

qd: femqd.o \
		qdmemory.o \
                qdreader.o \
	        qdid.o \
	        lm.o \
                qdkasmbl.o\
				qdkasmblStress.o\
		dotx.o\
	        matx.o\
	        matxt.o\
		kasmbl.o\
		quadb.o\
		qdwriter.o\
		qdshape.o\
		solve.o\
		qdArea.o\
		qdCalcForce.o
	$(CC) -o qd femqd.o \
		qdmemory.o \
		qdreader.o \
                qdid.o \
                lm.o \
                qdkasmbl.o\
				qdkasmblStress.o\
		dotx.o\
		matx.o\
	        matxt.o\
		kasmbl.o\
	        quadb.o\
		qdwriter.o\
              	qdshape.o\
		solve.o\
		qdArea.o\
		qdCalcForce.o $(LIBS) 
qdmemory.o: qdmemory.c
	$(CC) -c qdmemory.c

qdreader.o: qdreader.c
	$(CC) -c qdreader.c

qdshape.o:  qdshape.c
	$(CC) -c qdshape.c

qdid.o:  qdid.c
	$(CC) -c qdid.c

lm.o:  lm.c
	$(CC) -c lm.c

qdkasmbl.o:  qdkasmbl.c
	$(CC) -c qdkasmbl.c

qdkasmblStress.o:  qdkasmblStress.c
	$(CC) -c qdkasmblStress.c

dotx.o:  dotx.c
	$(CC) -c dotx.c

matx.o:  matx.c
	$(CC) -c matx.c

matxt.o:  matxt.c
	$(CC) -c matxt.c

kasmbl.o:  kasmbl.c
	$(CC) -c kasmbl.c

quadb.o:  quadb.c
	$(CC) -c quadb.c

qdwriter.o: qdwriter.c
	$(CC) -c qdwriter.c

solve.o:  solve.c
	$(CC) -c solve.c

qdArea.o:  qdArea.c
	$(CC) -c qdArea.c

qdCalcForce.o:  qdCalcForce.c
	$(CC) -c qdCalcForce.c

femqd.o: femqd.c
	$(CC) -c femqd.c

clean:
	-rm *.o $(PROGS)
