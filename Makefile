CC = gcc
CFLAGS = -Wall -g
SOURCES = xstring.c ui-common.c ctimeline.c
OBJECTS = ${SOURCES:.c=.o}

CTIMELINE_PATH = /var/www/html/ctimeline
CTIMELINE_ELF = ctimeline.cgi
CTIMELINE_CSS = ctimeline.css

.PHONY: all clean install uninstall

all: $(CTIMELINE_ELF)

$(CTIMELINE_ELF): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(CTIMELINE_ELF) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(CTIMELINE_ELF) $(OBJECTS)

install: all
	install -m 755 -d $(CTIMELINE_PATH)
	install -m 755 $(CTIMELINE_ELF) $(CTIMELINE_PATH)/$(CTIMELINE_ELF)
	install -m 755 $(CTIMELINE_CSS) $(CTIMELINE_PATH)/$(CTIMELINE_CSS)

uninstall:
	rm -f $(CTIMELINE_PATH)/$(CTIMELINE_ELF)
	rm -f $(CTIMELINE_PATH)/$(CTIMELINE_CSS)
