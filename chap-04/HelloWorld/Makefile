CC      := ia16-elf-gcc
CCFLAGS := 
LFLAGS  := 

SRCDIR   := .
INCL_DIR := ../../00_include

TARGET = ../../DOS_EXEs/HELLO.COM

SRCS := $(wildcard $(INCL_DIR)/*.c)
SRCS += $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:.c=.o)

all: clean $(OBJS)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS) $(LFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

run: all
	@./$(TARGET)

clean:
	rm -fr *.o $(INCL_DIR)/*.o