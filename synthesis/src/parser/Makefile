include Makefile.in

$(EXE): $(PARSERLIB) main.o
	$(CC) -o $@ main.o $(LIBS)

$(PARSERLIB): $(OBJS)
	$(AR) rcs $(PARSERLIB) $(OBJS); 

%.o:%.cpp
	$(CC) $(CPPFLAGS)  $< -o $@

parser.tab.cc: parser.y
	$(YACC)  -d -oparser.tab.cc parser.y; \
	\rm -f .parser; \
	touch .parser; \
	echo '#include "parsehelp.h"'  >> .parser; \
	cat parser.tab.hh >> .parser; \
	mv .parser parser.tab.hh
lex.yy.cc: lex.l  parser.tab.cc
	$(LEX) -o lex.yy.cc lex.l

clean:
	\rm -rf *.o bulah parser.tab.cc lex.yy.cc parser.tab.hh $(PARSERLIB) $(EXE)
