NAME := aie
BUILD := debug

STATIC_LIB := lib/lib$(NAME).a
DYNAMIC_LIB := lib/lib$(NAME).so

MAIN_SOURCES += $(wildcard src/*.c)
MAIN_OBJECTS = $(MAIN_SOURCES:.c=.o)

OBJECTS = $(MAIN_OBJECTS)

CFLAGS += -Wall -std=c11 -Isrc/ -fpic

ifeq ($(BUILD), debug)
  CFLAGS += -g -O0 -DDEBUG_MODE
else ifeq ($(BUILD), release)
  CFLAGS += -O3
endif

.PHONY: all clean

all: $(STATIC_LIB) $(DYNAMIC_LIB)

$(STATIC_LIB): $(OBJECTS)
	ar -qcv $@ $?

$(DYNAMIC_LIB): $(OBJECTS)
	ld -Bshareable -o $@ $?

clean:
	@-$(RM) -v $(OBJECTS)

cleandist:
	@-$(RM) -v $(STATIC_LIB) $(DYNAMIC_LIB)
