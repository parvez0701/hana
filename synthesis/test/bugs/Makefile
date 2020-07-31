.SILENT :
	
DIRS := parse2synthtrans parser intermout synthesizability simulation
MAKE := /usr/bin/gmake
HANA_HOME/test/regression.pl := $(shell pwd)/regression.pl

.EXPORT_ALL_VARIABLES :

.PHONY: test
.PHONY: update
.PHONY: commit
.PHONY: clean

test:
	for d in $(DIRS); do \
		if [ ! -d $$d ]; then \
			continue; \
		fi;   \
		$(MAKE) -C $$d; \
	done

update:
	for d in $(DIRS); do \
		if [ ! -d $$d ]; then \
			continue; \
		fi;   \
		$(MAKE) update -C $$d; \
	done

commit:
	for d in $(DIRS); do \
		if [ ! -d $$d ]; then \
			continue; \
		fi;   \
		$(MAKE) commit -C $$d; \
	done

clean:
	for d in $(DIRS); do \
		if [ ! -d $$d ]; then \
			continue; \
		fi;   \
		$(MAKE) clean -C $$d; \
	done
