all: solver clean
	
solver: solver.cpp Puzzle.h
	g++ solver.cpp -o solver
	
clean:
	rm *.o
