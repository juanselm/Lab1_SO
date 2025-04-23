TARGET = psinfo
SRCS = psinfo.c infoPid.c infoFile.c
CC = gcc
CFLAGS = -Wall -g
all: $(TARGET)
$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)