CC      := ia16-elf-gcc
CCFLAGS := -mcmodel=medium -march=i286
LFLAGS  := -li86

SRCDIR   := .
INCL_DIR := ../../00_include

TARGET = ../../DOS_EXEs/FPU_TEST.EXE

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