CC=gcc
CFLAGS=-Wall -g -O0 -DDEBUG
LDFLAGS=

EXE_SRC=g500-dump-page.c \
	g500-read-block.c \
	g500-write-block.c \
	g500-fill-page.c \
	g500-write-page.c \
	g500-write-page-lgs.c \
	g500-mem-op.c \
	g500-set-profile.c \
	g500-parse-profile.c \
	g500-parse-macro.c \
	g500-edit-profile.c
LIB_SRC=logitech.c g500.c

SRC=$(EXE_SRC) $(LIB_SRC)
EXE_OBJ=$(EXE_SRC:.c=.o)
LIB_OBJ=$(LIB_SRC:.c=.o)
TARGETS=$(basename $(EXE_SRC))

all: $(TARGETS)

$(TARGETS): %: %.o $(LIB_OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.deps: %.c
	$(CC) -M $(CFLAGS) $< > $@

-include $(SRC:.c=.deps)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(SRC:.c=.o) $(SRC:.c=.deps)

