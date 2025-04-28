CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O3
src = $(shell find ./src/ -name '*.c')
executable = jdis

makefile_indicator = .\#makefile\#
RM = rm -f
objects = $(src:.c=.o)

.PHONY: all clean archive

all: $(executable)

clean:
	$(RM) $(executable)
	@$(RM) $(makefile_indicator) $(objects)

$(executable): $(src)
	$(CC) $(CFLAGS) -o $(executable) $^

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)

archive:
	tar -cvf ../projet.tar \
		--exclude='.git' \
		--exclude='.gitignore' \
		--exclude='textes' \
		--exclude='tests.sh' \
		--exclude='README.md' \
		.
