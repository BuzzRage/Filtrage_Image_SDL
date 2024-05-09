CPP	= gcc
CFLAGS  = `sdl2-config --cflags`
LIBS	= `sdl2-config --libs` -lm -lSDL2_image
EXEC	= main

all: ${EXEC}

${EXEC}: ${EXEC}.o
	${CPP} ${EXEC}.c traitements.c objgraf.c $(CFLAGS) ${LIBS} -o Filtrage_Image
# 	${CPP} $(CFLAGS) -o ${EXEC} ${EXEC}.o ${LDFLAGS}
# ${EXEC}.o: ${EXEC}.c
# 	${CPP} ${EXEC}.c traitements.c $(CFLAGS) -o ${EXEC}.o
# 	${CPP} $(CFLAGS) -o ${EXEC}.o -c ${EXEC}.c traitements.c
clean:	
	rm -fr *.o

mrproper: clean
	rm -fr ${EXEC}
