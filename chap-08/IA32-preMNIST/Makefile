CXX      := gcc
CXXFLAGS := 

SRCDIR    = .
OBJDIR    = .
INCL_DIR := 

SRCS  := ../../00_include/exmath.c ../../00_include/nn_data_io.c
SRCS  += $(wildcard $(SRCDIR)/*.c)
TARGET = preMNIST.out
OBJS  := $(SRCS:.c=.o)

all: clean $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

run: all
	@./$(TARGET)

clean:
	rm -fr *.o $(INCL_DIR)/*.o