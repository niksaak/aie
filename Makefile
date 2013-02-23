NAME := aie
BUILD := debug

STATIC_LIB := lib/lib$(NAME).a
DYNAMIC_LIB := lib/lib$(NAME).so

CFLAGS += -Wall -std=c11 -Iinclude/ -fpic

ifeq ($(BUILD), debug)
  CFLAGS += -g -O0 -DDEBUG_MODE
else ifeq ($(BUILD), release)
  CFLAGS += -O3
endif

MAIN_SOURCES += $(wildcard src/*.c)
MAIN_OBJECTS = $(MAIN_SOURCES:.c=.o)
FORMATS_SOURCES += $(wildcard src/formats/*.c)
FORMATS_OBJECTS += $(FORMATS_SOURCES:.c=.o)

OBJECTS = $(FORMATS_OBJECTS) $(MAIN_OBJECTS)

.PHONY: all debug release static_lib dynamic_lib clean cleandist

all: static_lib dynamic_lib

debug: clean cleandist
	$(MAKE) BUILD=debug all

release: clean cleandist
	$(MAKE) BUILD=release all

static_lib: $(STATIC_LIB)

dynamic_lib: $(DYNAMIC_LIB)

$(STATIC_LIB): $(MAIN_OBJECTS) $(FORMATS_OBJECTS)
	ar -rcuv $@ $?

$(DYNAMIC_LIB): $(MAIN_OBJECTS) $(FORMATS_OBJECTS)
	ld -Bshareable -o $@ $?

$(FORMATS_OBJECTS): $(MAIN_OBJECTS)

clean:
	@-$(RM) -v $(OBJECTS)

cleandist:
	@-$(RM) -v $(STATIC_LIB) $(DYNAMIC_LIB)

