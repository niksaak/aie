NAME := aie
BUILD := debug

STATIC_LIB := lib/lib$(NAME).a
DYNAMIC_LIB := lib/lib$(NAME).so

CFLAGS += -Wall -std=c99 -pedantic -Iinclude/ -fpic

ifeq ($(BUILD), debug)
  CFLAGS += -g -O0 -DDEBUG_MODE
else ifeq ($(BUILD), release)
  CFLAGS += -O3
endif

MAIN_SOURCES += $(wildcard src/*.c)
MAIN_OBJECTS = $(MAIN_SOURCES:.c=.o)
FORMATS_SOURCES += $(wildcard src/formats/*.c)
FORMATS_OBJECTS += $(FORMATS_SOURCES:.c=.o)

OBJECTS = $(MAIN_OBJECTS) $(FORMATS_OBJECTS)

.PHONY: all debug release static_lib dynamic_lib core formats test
.PHONY: clean cleantest cleandist

all: static_lib dynamic_lib

debug: clean cleandist
	$(MAKE) BUILD=debug all

release: clean cleandist
	$(MAKE) BUILD=release all

static_lib: $(STATIC_LIB)

dynamic_lib: $(DYNAMIC_LIB)

core: $(MAIN_OBJECTS)

formats: $(FORMATS_OBJECTS)

test: all
	$(MAKE) -C test/ all

runtest: all
	$(MAKE) -C test/ run

$(STATIC_LIB): $(OBJECTS)
	ar -rcuv $@ $?

$(DYNAMIC_LIB): $(OBJECTS)
	ld -Bshareable -o $@ $(OBJECTS)

$(FORMATS_OBJECTS): $(MAIN_OBJECTS)

clean:
	@-$(RM) -v $(OBJECTS)

cleantest:
	$(MAKE) -C test/ clean cleandist

cleandist: cleantest
	@-$(RM) -v $(STATIC_LIB) $(DYNAMIC_LIB)

