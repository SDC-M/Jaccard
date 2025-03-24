.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" avl/* jdis_test/* op/* jdis/* holdall/* makefile

clean:
	$(MAKE) -C jdis_test clean
