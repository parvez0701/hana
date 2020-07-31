TESTFILE := test.v
SYNTHFILE := synth.v
GOLD := gold.out
TEST := test.out

TBSCRIPT := $(HANA_HOME)/test/simulation/simulate.pl
TESTTB := test_tb.v
GOLDTB := gold_tb.v
GENLIB := $(HANA_HOME)/src/vlib/generic.v
TECHLIB := $(HANA_HOME)/src/vlib/tech.v
CELLLIB := tech.lib

.PHONY: test

test: $(TEST) $(GOLD)
	$(HANA_HOME)/test/simulation/compare.pl $(GOLD) $(TEST) $(shell pwd)

$(TEST) : $(TESTTB)	
	iverilog $(TESTTB) $(SYNTHFILE) $(TECHLIB) $(GENLIB);\
	./a.out 

$(GOLD) : $(GOLDTB)	
	iverilog $(GOLDTB) $(TESTFILE)  ;\
	./a.out 

$(TESTTB): $(SYNTHFILE) 
	$(TBSCRIPT) -r $(SYNTHFILE) -b $(TESTTB) -g -t $(TEST) 
$(GOLDTB) : $(TESTFILE)
	$(TBSCRIPT) -r $(TESTFILE) -b $(GOLDTB) -g -t $(GOLD) 
	

$(SYNTHFILE) : $(TESTFILE)
	$(HANA_HOME)/bin/hana -s $(SYNTHFILE) $(TESTFILE) -c $(CELLLIB)

.PHONY: clean
clean :	
	\rm -f $(TEST) $(GOLD) $(TESTTB) $(GOLDTB) $(SYNTHFILE) a.out 
