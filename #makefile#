CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O0 -g3
src = $(shell find ./src/ -name '*.c')
executable = jdis

src_test = tests/main.c tests/greatest/greatest.h $(shell find ./src/ -name '*.c' -not -name 'main.c')
executable_test = $(executable)_test
makefile_indicator = .\#makefile\#

.PHONY: all clean test

all: $(executable)

clean:
	$(RM) $(executable)
	$(RM) $(executable_test)
	$(RM) *.gcov *.gcno *.gcda
	@$(RM) $(makefile_indicator)

test: $(executable_test)
	./$(executable_test)


$(executable): $(src)
	$(CC) $(CFLAGS) -o $(executable) $^


$(executable_test): $(src_test)
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -o $(executable_test) -DTEST_MAIN $^

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)

