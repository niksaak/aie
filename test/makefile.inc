TEST_DIR = test
TEST_PROC = $(TEST_DIR)/test
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
TEST_CFLAGS = $(CFLAGS)
TEST_LDFLAGS = $(LDFLAGS) -static -Llib/ -laie

test: $(TEST_PROC)

runtest: test
	$(TEST_PROC)

$(TEST_PROC): $(TEST_OBJECTS)
	$(CC) -o $@ $(TEST_OBJECTS) $(TEST_LDFLAGS)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(TEST_CFLAGS) -c $? -o $@

cleantest:
	@-$(RM) -v $(TEST_OBJECTS) ; \
	  $(RM) -v $(TEST_PROC)

# vim:filetype=make
