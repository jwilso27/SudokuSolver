all: solver
	
solver: solver.cpp Puzzle.h
	g++ solver.cpp -o solver
	
clean:
	rm *.o

cleanall:
	rm *.o solver
