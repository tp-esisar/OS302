COMP = gcc
OPTIONS =
OPTIONS_CC = -g -Wall

%.e: %.o segment_memoire.o
	$(COMP) -o $@ $^ $(OPTIONS)

	
%.o: %.c 
	$(COMP) -o $@ -c $< $(OPTIONS_CC)


clean :
	rm *.o *.e

