MAKE=make

SUBDIRS := $(wildcard ex*/)

clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ clean

.PHONY: clean $(SUBDIRS)