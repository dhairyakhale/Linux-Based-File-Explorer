CC = g++ 
CFLAGS = -Wall 
DEPS = config.h
OBJ = allFunctions.o main.o 
%.o: %.cpp $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
		$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o main

