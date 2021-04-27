CC = gcc
CFLAGS = -Wall -g
SOURCES = xstring.c ctimeline.c
OBJECTS = ${SOURCES:.c=.o}
EXECUTABLE = app

all:
	@$(CC) $(DEFINES) $(CFLAGS) -c $(SOURCES)
	@$(CC) $(DEFINES) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)
