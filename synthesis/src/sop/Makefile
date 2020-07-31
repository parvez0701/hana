include Makefile.in

.PHONY: buildsrcs
buildsrcs: $(OBJS)
	
%.o:%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) $< -o $@


clean:
	\rm -rf *.o 
