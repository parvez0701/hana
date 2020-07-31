include Makefile.in

.PHONY: buildsrcs
buildsrcs: $(OBJS)
	
%.o:%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) $< -o $@

celllib.tab.cpp celllib.tab.hpp :	celllib.y 
	$(YACC) -p CellLib -d -ocelllib.tab.cpp celllib.y
	
cellliblex.yy.cpp: celllib.l celllib.tab.hpp celllib.tab.cpp
	$(LEX) -PCellLib -t celllib.l > $@

clean:
	\rm -rf *.o celllib.tab.cpp celllib.tab.hpp cellliblex.yy.cpp
