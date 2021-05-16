PROJECT_NAME = test
CC=gcc -O3
PROJECT_SRC = .
vpath %.c $(PROJECT_SRC)

########################################################################
SRCS = main.c
SRCS += modes.c parser.c matrixVectorCalc.c bombs.c display.c pagerank.c

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
	./$(PROJECT_NAME)

run: $(PROJECT_NAME)
	./$(PROJECT_NAME)

%.o: %.c
	$(CC) $< -c $(INCLUDE) $(CFLAGS)

clean:
	rm -f *.o $(PROJECT_NAME)
