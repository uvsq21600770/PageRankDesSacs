PROJECT_NAME = test
CC=gcc -O3
PROJECT_SRC = .
vpath %.c $(PROJECT_SRC)

########################################################################
SRCS = Main.c
SRCS += Modes.c Parser.c MatrixVectorCalc.c Bombs.c QuickSort.c

########################################################################
INC_DIRS = Include
INCLUDE = $(addprefix -I ,$(INC_DIRS))
########################################################################

CFLAGS= -Wall \
       -pedantic \
	   -g


LDFLAGS=

########################################################################

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRCS)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $< -c $(INCLUDE) $(CFLAGS)

clean:
	rm -f *.o $(PROJECT_NAME)
