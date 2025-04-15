CC = gcc
CFLAGS = -Wall

OUTPUT = parser
SOURCES = parser.c

all: force_build $(OUTPUT)

force_build:
	@if [ -e $(OUTPUT) ]; then rm -f $(OUTPUT); fi

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES) -g -DPRINT_PARSE

clean:
	rm -f $(OUTPUT)

.PHONY: all clean force_build