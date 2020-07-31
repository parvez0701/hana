TESTFILE := ASHIFT.v
SYNTHFILE := synth.v
GOLD := gold.out
TEST := test.out

TBSCRIPT := $(HANA_HOME)/test/simulation/simulate.pl
TESTTB := test_tb.v
GOLDTB := gold_tb.v

.PHONY: test

test: $(TEST) $(GOLD)
	$(HANA_HOME)/test/simulation/compare.pl $(GOLD) $(TEST) $(shell pwd)

$(TEST) : $(TESTTB)	
	iverilog $(TESTTB) $(SYNTHFILE) $(HANA_HOME)/src/vlib/generic.v ;\
	./a.out 

$(GOLD) : $(GOLDTB)	
	iverilog $(GOLDTB) $(TESTFILE) $(HANA_HOME)/src/vlib/generic.v;\
	./a.out 

$(TESTTB): $(SYNTHFILE) 
	$(TBSCRIPT) -r $(SYNTHFILE) -b $(TESTTB) -g -t $(TEST) 
$(GOLDTB) : $(TESTFILE)
	$(TBSCRIPT) -r $(TESTFILE) -b $(GOLDTB) -g -t $(GOLD) 
	

$(SYNTHFILE) : $(TESTFILE)
	$(HANA_HOME)/bin/hana -s $(SYNTHFILE) $(TESTFILE)

.PHONY: clean
clean :	
	\rm -f $(TEST) $(GOLD) $(TESTTB) $(GOLDTB) $(SYNTHFILE) a.out 
