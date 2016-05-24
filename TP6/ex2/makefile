COMP = gcc
OPTIONS =
OPTIONS_CC = -g -Wall

%.e: %.o
	$(COMP) -o $@ $^ $(OPTIONS)

	
%.o: %.c 
	$(COMP) -o $@ -c $< $(OPTIONS_CC)


clean :
	rm *.o *.e

