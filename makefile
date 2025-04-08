CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2
src = $(shell find ./src/ -name '*.c')
executable = jdis

makefile_indicator = .\#makefile\#
RM = rm -f
objects = $(src:.c=.o)

.PHONY: all clean test archive

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
	tar --exclude='jdis' --exclude='.git' --exclude='.gitignore' --exclude='./.#makefile#' \
	          --exclude='./rendu/contre-rendu.aux' --exclude='./rendu/contre-rendu.log' \
	          --exclude='./rendu/contre-rendu.out' --exclude='./rendu/contre-rendu.synctex.gz' \
	          --exclude='./rendu/contre-rendu.tex' --exclude='./rendu/contre-rendu.toc' -czvf ../projet.tar.gz .


