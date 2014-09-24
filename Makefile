CC=gcc
INCLUDES=-g -I. -Wall -Werror -W -Wno-unused-parameter  \
	-Wreturn-type -Wcast-qual -Wswitch -Wshadow -Wcast-align 
#	-Wmissing-prototypes -Wpointer-arith \
#	-Wwrite-strings -Wchar-subscripts -Winline -Wnested-externs \
#	-Wredundant-decls
# -Wstrict-prototypes \

#-Wall $(shell sdl-config --cflags) -I. -I/sw/include \
#	-I/usr/X11R6/include 
#	$(shell pkg-config --cflags theora vorbis oggz ogg)

#LIBS=$(shell sdl-config --libs) \
#	-L/usr/X11R6/lib -lSDL_image -lSDL_mixer -lvorbis -ltheora -loggz -lpng -lz

OBJS=message.o

all: clean wildwest

clean:
	rm -rf *.o wildwest wildwest.dSYM

distclean: clean

message.o:
	$(CC) $(INCLUDES) -c message.c

wildwest: $(OBJS)
	$(CC) -o wildwest $(INCLUDES) -DTESTPERSON main.c $(OBJS) $(LIBS)

