
.PHONY: test

.PHONY: update
	
.PHONY: commit

.PHONY: clean
.SILENT :
		
test:
	$(HANA_HOME)/test/regression.pl

update:
	$(HANA_HOME)/test/regression.pl update

commit:
	$(HANA_HOME)/test/regression.pl commit

clean:
	\rm -f *.diff test.log test_dec.v

