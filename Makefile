NAME := aie
BUILD := debug

STATIC_LIB := lib/lib$(NAME).a
DYNAMIC_LIB := lib/lib$(NAME).so

CFLAGS += -std=c99 -Iinclude/ -fpic -Wall -pedantic -fno-diagnostics-show-caret

ifeq ($(BUILD), debug)
  CFLAGS += -O0 -g3 -DDEBUG_MODE
else ifeq ($(BUILD), release)
  CFLAGS += -O3
endif

MAIN_SOURCES += $(wildcard src/*.c)
MAIN_OBJECTS = $(MAIN_SOURCES:.c=.o)
FORMATS_SOURCES += $(wildcard src/formats/*.c)
FORMATS_OBJECTS += $(FORMATS_SOURCES:.c=.o)

SOURCES := $(MAIN_SOURCES) $(FORMATS_SOURCES)
OBJECTS = $(MAIN_OBJECTS) $(FORMATS_OBJECTS)

.PHONY: all debug release static_lib dynamic_lib core formats test runtest lint
.PHONY: clean cleantest cleanmost

all: static_lib dynamic_lib test

include test/makefile.inc

debug: clean
	$(MAKE) BUILD=debug all

release: clean
	$(MAKE) BUILD=release all

static_lib: $(STATIC_LIB)

dynamic_lib: $(DYNAMIC_LIB)

core: $(MAIN_OBJECTS)

formats: $(FORMATS_OBJECTS)

# test: static_lib dynamic_lib
#	$(MAKE) -C test/ all

# runtest: all
#	$(MAKE) -C test/ run

lint:
	$(CC) $(CFLAGS) -fsyntax-only $(SOURCES)

lint-headers:
#	$(CC) $(CFLAGS) -v -fsyntax-only $(wildcard include/*)
	$(CC) -x c-header -o /dev/null $(CFLAGS) $(wildcard include/*)

$(STATIC_LIB): $(OBJECTS)
	$(AR) -rcuv $@ $?

$(DYNAMIC_LIB): $(OBJECTS)
	$(CC) -shared -fpic -o $@ $(OBJECTS)

$(FORMATS_OBJECTS): $(MAIN_OBJECTS)

clean: cleantest
	@-$(RM) -v $(OBJECTS) ;\
	  $(RM) -v $(STATIC_LIB) $(DYNAMIC_LIB)

cleanmost:
	@-$(RM) -v $(OBJECTS)

cleantest:
#	$(MAKE) -C test/ clean cleandist

help:
	@echo "libaie 0.03+0.09i" ;\
	 echo "===================" ;\
	 echo "Acceptable targets:" ;\
	 echo "  all          make everytrhing" ;\
	 echo "  debug        clean everything and make with BUILD=debug" ;\
	 echo "               (this is the default for now)";\
	 echo "  release      clean everything and make with BUILD=release" ;\
	 echo "  static_lib   make static lib" ;\
	 echo "  dynamic_lib  make dynamic lib" ;\
	 echo "  test         make tests" ;\
	 echo "  runtest      make and run tests" ;\
	 echo "	 lint         just check source files, no compiling"
	 echo "  lint-headers check headers" ;\
	 echo "  core         compile core library objects" ;\
	 echo "  formats      compile formatters objects" ;\
	 echo "  clean        clean everything" ;\
	 echo "  cleanmost    clean everything but libraries" ;\
	 echo "  cleantest    clean tests" ;\
	 echo

