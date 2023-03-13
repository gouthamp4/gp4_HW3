search: search.o
	gcc search.o -o search  

search.o: gp4_HW3.c
	gcc -c gp4_HW3.c -o search.o