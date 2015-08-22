//Jenna Wilson, CSE20212 Lab 6 Part 1 3/6/15

//This is the class declaration for the templated class Puzzle.

/* Data member possible (initialized in constructor); helper functions setPossible(),
solve(),solveHard(), and isPossible(); and member function solveMain() were added
from lab 5 to provide necessary functionality to solve both simple and difficult puzzles.

This template was made to be used with both int and char puzzle boards, as well as boards
of any square size such that the length of the rows/columns has an integer root (i.e. 4x4,
9x9, 16x16, etc).
*/

#ifndef PUZZLE_H
#define PUZZLE_H

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <typeinfo>
#include "Puzzle.h"
using namespace std;

template <typename T>
class Puzzle{
	public:
		Puzzle(string="sudoku.txt",int=9); //default constructor with fall backs
		int isValid(T,int,int); //check that T can be placed
		int isFull(); //check if sudoku is full
		int placeNum(); //place number/char on board
		int getSize() const; //return numRows
		void solveMain();
		T operator[](int); //returns inputs[i]
		T operator()(int,int); //returns value of position
	private:
		void setPossible(); //traverse vector to check and save possible values
		Puzzle<T> solve(); //uses algorithm to solve
		vector<Puzzle<T> > solveHard(); //extra algorithm to solve hard puzzles
		int isPossible(); //check that there are still possible moves
		
		vector<vector<T> > board;
		vector<vector<int> > mask; //saves position of given numbers so that they can't be changed
		vector<vector<vector<int> > > possible; //saves possible values as Puzzle p[row][column][position of possible value]=possible value
		vector<T> inputs; //vector to save possible inputs
		const char *c; //for T type
		T empty; //for char/int indicating an empty space
		int numRows; //number of rows/columns
};

template <typename T>
//default constructor with fall backs
Puzzle<T>::Puzzle(string s, int val){
	//declare temp parameters
	T x;
	vector<T> temp; //to create board
	vector<int> tmpMask; //to create mask
	vector<vector<int> > tmpBoard; //to create mask
	
	//set T type and empty
	c=typeid(x).name();
	if(*c=='i') empty=0;
	else if(*c=='c') empty='0';
	
	numRows=val; //set number of rows
	int numLines=0; //line counter
	int i,j,k; //iterators
	
	//create 3d vector of size 9x9x9 of possible values
	for(i=0;i<numRows;i++) tmpMask.push_back(0);
	for(i=0;i<numRows;i++) tmpBoard.push_back(tmpMask);
	for(i=0;i<numRows;i++) possible.push_back(tmpBoard);
	
	tmpMask.clear(); //clear temp to reuse
	tmpBoard.clear(); //clear temp
	
	//open file
	ifstream input;
	input.open(s.c_str());
	if(input.fail()) cout << "Error reading file" << endl;
	else{
		while(!input.eof()){
			for(j=0;j<numRows;j++){
				input >> x; //save value to x
				temp.push_back(x); //create vector of Ts
				if(x!=empty) tmpMask.push_back(1); //add 1 to mask if not empty
				else if(x==empty) tmpMask.push_back(0);
			} //end for(int j=0;j<numRows;j++)
			board.push_back(temp); //add temp vector to board
			mask.push_back(tmpMask); //add tmpMask to mask
			temp.clear(); //clear temp to reuse
			tmpMask.clear(); //clear tmpMask to reuse
			numLines++; //increment line counter
		} //end for(int i=0;i<numRows;i++)
		input.close(); //close file

		if((numLines-1)>numRows){ //to account for 1st line in wordoku
			for(j=0;j<numRows;j++){
				board[j].swap(board[j+1]); //moves first row to last
				mask[j].swap(mask[j+1]);
			} //end for(j=0;j<numRows;j++)
			for(j=0;j<numRows;j++)
				inputs.push_back(board[numRows].at(j)); //saves chars of word for wordoku
			board.pop_back(); //removes extra line from board
			mask.pop_back();
		}else{
			for(j=0;j<numRows;j++)
				inputs.push_back(j+1); //saves int values for sudoku
		}//end if(numLines!=numRows)
	} //end else
	
	//set possible to 2 where values already set
	for(i=0;i<numRows;i++) //rows
		for(j=0;j<numRows;j++) //columns
			if(mask[i].at(j))
				for(k=0;k<numRows;k++) //possible value position
					if(board[i].at(j)==inputs[k])
						possible[k][i].at(j)=2;
}

//check that T can be placed
template <typename T>
int Puzzle<T>::isValid(T t,int row,int col){
	int good=1,i,j,k=0,l=0;
	
	//check that t is a possible value, i.e. check against inputs vector
	for(i=0;i<numRows;i++){
		if(t==inputs[i]){good=1; break;} //break loop if it matches any of the possible inputs
		else good=0;
	} //end for(i=0;i<numRows;i++)
	
	//check that number can be changed, i.e. not originally on board
	if(mask[row-1].at(col-1)==1) good=0;
	
	//check row for same number
	for(i=0;i<numRows;i++)
		if(board[row-1].at(i)==t) good=0;
	
	//check column for same number
	for(i=0;i<numRows;i++)
		if(board[i].at(col-1)==t) good=0;
	
	//find which box it is in
	while(row>k*sqrt(numRows))
		k++;
	while(col>l*sqrt(numRows))
		l++;
	
	//check box for same number
	for(i=(k-1)*sqrt(numRows);i<k*sqrt(numRows);i++)
		for(j=(l-1)*sqrt(numRows);j<l*sqrt(numRows);j++)
			if(board[i].at(j)==t) good=0;
	
	return good;
}

//check if sudoku is full
template <typename T>
int Puzzle<T>::isFull(){
	int ifEmpty=1; //empty spot indicator
	
	for(int i=0;i<numRows;i++)
		for(int j=0;j<numRows;j++)
			if(board[i].at(j)==empty)
				ifEmpty=0;
				
	return ifEmpty;
}

//place T value on board
template <typename T>
int Puzzle<T>::placeNum(){
	T i;
	int j,k;
	
	cout << "What value would you like to place?" << endl;
	cin >> i;
	if(i!=empty){ //check that user is not quitting
		cout << "What row would you like to place it in?" << endl;
		cin >> j;
		if(j!=0){ //check that user is not quitting
			cout << "What column would you like to place it in?" << endl;
			cin >> k;
		} //end if(j!=0)
	} //end if(i!=empty)
	
	if(i==empty || j==0 || k==0) //check that user is not quitting
		return 0;
	else{
		if(isValid(i,j,k))
			board[j-1].at(k-1)=i; //set value
		else
			cout << "That is an invalid move" << endl;
		return 1;
	} //end else
}

//return numRows
template <typename T>
int Puzzle<T>::getSize() const{
	return numRows;
}

//traverse vector to check and save possible values
template <typename T>
void Puzzle<T>::setPossible(){
	int i,j,k;
	
	for(k=0;k<numRows;k++) //possible value position
		for(i=0;i<numRows;i++) //rows
			for(j=0;j<numRows;j++) //columns
				if(possible[k][i].at(j)!=2) //if position already set
					possible[k][i].at(j)=isValid(inputs[k],i+1,j+1); //sets possible value to 1 if valid && 0 if not
	
// 	//print possible
// 	for(i=0;i<numRows;i++){
// 		cout << i+1 << endl;
// 		for(j=0;j<numRows;j++){
// 			for(k=0;k<numRows;k++)
// 				cout << possible[i][j].at(k) << " ";
// 			cout << endl;
// 		}
// 		cout << endl;
// 	}
// 	cout << endl;
}

//puts functions together to solve completely
template <typename T>
void Puzzle<T>::solveMain(){
	cout << "Solving..." << endl;
	Puzzle<T> pNew=solve(); //solve as much as possible with solve()
	if(!pNew.isFull()){ //if solve() doesn't fill the puzzle
		vector<vector<Puzzle<T> > > pVec;
		pVec.push_back(pNew.solveHard());
		
		int i,j,inSize,outSize=pVec.size();
		
		for(i=0;i<outSize;i++){
			inSize=pVec[i].size();
			for(j=0;j<inSize;j++){
				if(pVec[i][j].isFull()){
					pNew=pVec[i][j]; //save full puzzle
					i=outSize; //break loop
					j=inSize; //break loop
				}else if(pVec[i][j].isPossible()) //if moves still possible
					pVec.push_back(pVec[i][j].solveHard()); //get next possibilities
			} //end for(j=0;j<inSize;j++)
			outSize=pVec.size();
		} //end for(i=0;i<outSize;i++)		
	} //end if(!pNew.isFull())
	cout << pNew << endl; //display completed board
}

//use algorithm to solve puzzle
template <typename T>
Puzzle<T> Puzzle<T>::solve(){
	int i,j,k,row,col,pos,vbox,hbox,count,same=0;
	vector<vector<T> > tmpBoard;
	
	while(!isFull() && !same){
		tmpBoard=board;
		same=1;
		setPossible();
		
		//check for single value
		for(i=0;i<numRows;i++) //rows
			for(j=0;j<numRows;j++){ //column
				count=0; //reset count
				for(k=0;k<numRows;k++){ //possible value position
					if(possible[k][i].at(j)){
						count++; //increment count
						row=i; //save row number
						col=j; //save column number
						pos=k; //save possible value position
					} //end if(t!=empty)
				} //end for(k=0;k<numRows;k++)
				if(count==1){
					board[row].at(col)=inputs[pos]; //set board to corresponding value of inputs vector only one possible solution
					mask[row].at(col)=1; //set mask to 1 so that numbers placed cannot be changed
					possible[pos][row].at(col)=2; //set possible to 2, indicating set number
				} //end if(count==1)
			} //end for(j=0;j<numRows;j++)
		
		//check columns for singleton
		for(k=0;k<numRows;k++) //possible value position
			for(j=0;j<numRows;j++){ //column
				count=0; //reset count
				for(i=0;i<numRows;i++){ //row
					if(possible[k][i].at(j)){
						count++; //increment count
						row=i; //save row number
						col=j; //save column number
						pos=k; //save possible value position
					} //end if(t!=empty)
				} //end for(k=0;k<numRows;k++)
				if(count==1){
					board[row].at(col)=inputs[pos]; //set board to corresponding value of inputs vector only one possible solution
					mask[row].at(col)=1; //set mask to 1 so that numbers placed cannot be changed
					possible[pos][row].at(col)=2; //set possible to 2, indicating set number
				} //end if(count==1)
			} //end for(j=0;j<numRows;j++)
			
		//check rows for singleton
		for(k=0;k<numRows;k++) //possible value position
			for(i=0;i<numRows;i++){ //row
				count=0; //reset count
				for(j=0;j<numRows;j++){ //column
					if(possible[k][i].at(j)){
						count++; //increment count
						row=i; //save row number
						col=j; //save column number
						pos=k; //save possible value position
					} //end if(t!=empty)
				} //end for(i=0;i<numRows;i++)
				if(count==1){
					board[row].at(col)=inputs[pos]; //set board to corresponding value of inputs vector only one possible solution
					mask[row].at(col)=1; //set mask to 1 so that numbers placed cannot be changed
					possible[pos][row].at(col)=2; //set possible to 2, indicating set number
				} //end if(count==1)			
			} //end for(j=0;j<numRows;j++)
			
		//check boxes for singleton
		for(k=0;k<numRows;k++) //possible value position
			for(vbox=0;vbox<3;vbox++) //box row
				for(hbox=0;hbox<3;hbox++){ //box column
					count=0; //reset count
					for(i=vbox*sqrt(numRows);i<(vbox+1)*sqrt(numRows);i++) //row
						for(j=hbox*sqrt(numRows);j<(hbox+1)*sqrt(numRows);j++) //column
							if(possible[k][i].at(j)){
								count++; //increment count
								row=i; //save row
								col=j; //save col
								pos=k; //save possible value position
							} //end if(t!=empty)
					if(count==1){
						board[row].at(col)=inputs[pos]; //set board to corresponding value of inputs vector only one possible solution
						mask[row].at(col)=1; //set mask to 1 so that numbers placed cannot be changed
						possible[pos][row].at(col)=2; //set possible to 2, indicating set number
					} //end if(count==1)
				} //end for(hbox=0;hbox<3;hbox++)
		
		//check to see if board has changed in last iteration
		for(i=0;i<numRows;i++)
			for(j=0;j<numRows;j++)
				if(tmpBoard[i].at(j)!=board[i].at(j)) same=0;
	} //end while(!isFull() && !same)
	
	return (*this);
}

//extra algorithm for hard puzzles
template <typename T>
vector<Puzzle<T> > Puzzle<T>::solveHard(){
	int i,j,k,imin,jmin,minCount=numRows,count=0;
	vector<Puzzle<T> > p; //vector of possible puzzles to return
	vector<vector<int> > tmpMask=mask; //keep copy of current mask
	vector<vector<T> > tmpBoard=board; //keep copy of current board
	vector<vector<vector<int> > > tmpPossible=possible; //keep copy of possibilities
	
	//find position that has the least number of possible values
	for(i=0;i<numRows;i++){
		for(j=0;j<numRows;j++){
			count=0;
			for(k=0;k<numRows;k++)
				if(possible[k][i].at(j)==1) count++;
			if(count<minCount && count>0){
				imin=i;
				jmin=j;
				minCount=count;
			} //end if(count<minCount)
		} //end for(j=0;j<numRows;j++)
	} //end for(i=0;i<numRows;i++)

	//create vector of boards, each with different value for position with the least number of possibilities
	for(k=0;k<numRows;k++){
		if(possible[k][imin].at(jmin)){
		
			//create board with guess
			board[imin].at(jmin)=inputs[k];
			mask[imin].at(jmin)=1;
			possible[k][imin].at(jmin)=2;
			setPossible();
			p.push_back(solve());
			
		} //end if(possible[k][i].at(j))
		
		//reset board to current board
		board=tmpBoard;
		mask=tmpMask;
		possible=tmpPossible;
		setPossible();
	} //end for(k=0;k<numRows;k++)
	
	return (p);
}

//checks if there are any more possible moves
template <typename T>
int Puzzle<T>::isPossible(){
	int i,j,k,count,count2,p=1;
	
	for(k=0;k<numRows;k++){ //possible value position
	
		//check that it inputs[k] is possible in each row
		for(i=0;i<numRows;i++){ //row
			count=0; //reset count
			for(j=0;j<numRows;j++) //column
				if(possible[k][i].at(j)) count++;
			if(!count) p=0;
		} //end for(i=0;i<numRows;i++)
		
		//check that it inputs[k] is possible in each column
		for(j=0;j<numRows;j++){ //column
			count=0; //reset count
			count2=0; //reset count2
			for(i=0;i<numRows;i++) //row
				if(possible[k][i].at(j)==1) count++; //count possibilities
				else if(possible[k][i].at(j)==2) count2++; //count set numbers
			if((!count2 && !count) || count2>1) p=0;
		} //end for(i=0;i<numRows;i++)
			
	}//end for(k=0;k<numRows;k++)
	
	return p;
}

//overloaded operator() to take in row and column and output board value
template <typename T>
T Puzzle<T>::operator()(int r,int c){
 	T it=board[r-1].at(c-1);
	return it;
}

//overloaded operator[] to get inputs vector
template <typename T>
T Puzzle<T>::operator[](int i){
 	T it=inputs.at(i);
	return it;
}

//overloaded output operator
template <typename T>
ostream &operator<<(ostream &out,Puzzle<T> &p){
	int i,j,size=p.getSize(),box=sqrt(size);
	
	out << "  |";
	for(i=1;i<size;i++){
		if(i%box==0)
			out << i << "|"; //label columns
		else{
			out << i << " "; //label columns
			if(i<10) out << " ";
		} //end else
	} //end for(i=1;i<size;i++)
	out << i << "|" << endl;
	
	for(i=1;i<=size;i++){
		if((i-1)%box==0){
			for(j=0;j<=size;j++)
				out << "---"; //separate sudoku boxes
			out << endl; 
		} //end if(i%box==0)
		if(i<10) out << " ";
		out << i << "|"; //label rows
		for(j=1;j<=size;j++){ 
			out << p(i,j); //print value in  j-th column of i-th row
			if(j%box==0){
				if(j>=10) out << " ";
				out << "|"; //separate sudoku boxes
			}else out << "  "; //separate numbers
		} //end for(j=1;j<=size;j++)
		out << endl;
	} //end for(i=1;i<=size;i++)
	
	out << "\nInput values: ";
	for(i=0;i<size;i++) out << p[i] << " ";
	out << endl;
	
	return out;
}

#endif