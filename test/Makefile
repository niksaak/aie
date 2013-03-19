NAME := test

AIE_INCLUDE_PATH := $(realpath ../include/)
AIE_LIB_PATH := $(realpath ../lib/)

CFLAGS += -g -Wall -O0 -std=c99 -pedantic -I$(AIE_INCLUDE_PATH)
LDFLAGS += -Wl,-Bstatic -L$(AIE_LIB_PATH) -laie -Wl,-Bdynamic

SOURCES = test.c test_util.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all run clean cleandist

all: $(NAME)

run: $(NAME)
	./$<

$(NAME): $(OBJECTS)
	$(CC) -o $@ $? $(LDFLAGS)

clean:
	@-$(RM) -v $(OBJECTS)

cleandist:
	@-$(RM) -v $(NAME)
