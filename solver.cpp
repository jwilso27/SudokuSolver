//Jenna Wilson, CSE20212 Lab 6 Part 2 3/6/15

//This is the driver function that demonstrates the templated class Puzzle

#include <iostream>
#include <typeinfo>
#include "Puzzle.h"
using namespace std;

int main(){
	string infile;
	int m,n;
	
	cout << "Please enter a filename: " << endl;
	cin >> infile;
	
	cout << "Enter 1 for Sudoku (ints) or 2 for Wordoku (chars): " << endl;
	cin >> m;
	
	cout << "How many rows does this sudoku have?" << endl;
	cin >> n;
	
	if(m==1){
		Puzzle<int> p(infile,n); //input sudoku
		cout << p << endl;
		p.solveMain();
	}else if(m==2){
		Puzzle<char> p(infile,n); //input wordoku
		cout << p << endl;
		p.solveMain();
	} //end if(m==1)...else if(m==2)

}