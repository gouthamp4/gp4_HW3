Instructions to compile and execute the code :


Compile : gcc -o search gp4_HW3.c
Execute : ./search -s <file_size> -E "<unix-cmd>" <file/directory>
	    ./search -f <string_pattern> <depth> -E "<unix-cmd>" <file/directory>
	    ./search -s <filesize> -e "<unix-cmd>" <file/directory>
	    ./search -f <stringpattern> <depth> -e "<unix-cmd>" <file/directory>
	    
If makefile is used,
Compile : make 
Execute : ./search