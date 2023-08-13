CC = gcc
CFLAGS = -I./include -Wall 
LIBS := -lm
OBJDIR = objects
SRCDIR = source

OBJECTS = $(OBJDIR)/tDocument.o $(OBJDIR)/tNewsLetter.o $(OBJDIR)/tWord.o $(OBJDIR)/tClass.o
ALL_OBJECTS = $(shell find . -type f -name "*.o" -print) 

all: prog1 prog2 prog3 build_libs

build_libs: $(ALL_OBJECTS)
	ar -rcs libTP2.a $(ALL_OBJECTS)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	
prog1: $(OBJDIR) $(OBJECTS) main1.c
	$(CC) $(CFLAGS) -o prog1 main1.c $(OBJECTS) $(LIBS)

prog2: $(OBJDIR) $(OBJECTS) main2.c
	$(CC) $(CFLAGS) -o prog2 main2.c $(OBJECTS) $(LIBS)

prog3: $(OBJDIR) $(OBJECTS) main3.c
	$(CC) $(CFLAGS) -o prog3 main3.c $(OBJECTS) $(LIBS)

$(OBJDIR)/tDocument.o: $(SRCDIR)/tDocument.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/tDocument.c -o $(OBJDIR)/tDocument.o

$(OBJDIR)/tNewsLetter.o: $(SRCDIR)/tNewsLetter.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/tNewsLetter.c -o $(OBJDIR)/tNewsLetter.o

$(OBJDIR)/tWord.o: $(SRCDIR)/tWord.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/tWord.c -o $(OBJDIR)/tWord.o

$(OBJDIR)/tClass.o: $(SRCDIR)/tClass.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/tClass.c -o $(OBJDIR)/tClass.o


clean:

	rm prog1 prog2 prog3 $(OBJDIR)/*.o 
	rm -r $(OBJDIR)
	rm -f *.bin
	rm -f *.txt
	rm -f *.a
