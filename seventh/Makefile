CC = gcc
CFLAGS = -Wall

OUTPUT = file_copy
SOURCES = file_copy.c

all: force_build $(OUTPUT)

force_build:
	@if [ -e $(OUTPUT) ]; then rm -f $(OUTPUT); fi

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	rm -f $(OUTPUT)

.PHONY: all clean force_build