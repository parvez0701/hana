include Makefile.in

$(EXE): $(SYNTHLIB) main.o
	$(CC) -o $@ main.o $(LIBS)

$(SYNTHLIB): $(OBJS) buildsrcs
	$(AR) rcs $(SYNTHLIB) $(OBJS) $(LEXOBJS) $(PARSEOBJS)

.PHONY : buildsrcs

buildsrcs:
	for d in $(SRCDIRS); do \
		if [ ! -d $$d ]; then \
			continue; \
		fi;   \
		$(MAKE) -C $$d; \
	done

%.o:%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) $< -o $@

cleanp:
	$(MAKE) clean -C parser

clean:
	for d in $(SRCDIRS); do \
		if [ ! -d $$d ]; then \
			continue; \
		fi;   \
		if [ $$d == parser ]; then \
			continue; \
		fi; \
		$(MAKE) clean -C $$d; \
	done ; \
	\rm -rf *.o $(SYNTHLIB) $(EXE) 
