COMP = gcc
OPTIONS =
OPTIONS_CC = -g -Wall

ex%.e: ex%.o
	$(COMP) -o $@ $^ $(OPTIONS)

	
%.o: %.c 
	$(COMP) -o $@ -c $< $(OPTIONS_CC)


clean :
	$(RM) *.o *.e

