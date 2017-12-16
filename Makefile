quadmoe: quadmoe.o
	gcc -o $@ $^ $(shell curl-config --libs)

quadmoe.o: quadmoe.c json.h
	gcc -c $< $(shell curl-config --cflags)
