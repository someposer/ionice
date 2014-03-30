all: ionice ionice_fork

ionice: ionice.o
	gcc -o ionice ionice.o

ionice_fork: ionice_fork.o
	gcc -o ionice_fork ionice_fork.o
