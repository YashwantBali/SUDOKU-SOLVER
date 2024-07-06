#include<iostream>
#include<fstream>
#include <cstdlib>
//#include <Windows.h>
using namespace std;

class SudokuGrid{

	int grid[9][9]; 
	public:SudokuGrid(){
		menu();
	}

	private:
	void menu(){

		cout<<"\n======================\n";
		cout<<"    Sudoku Solver\n";
		cout<<"======================\n\n";

		cout<<"Welcome to Sudoku Solver!\n";
		cout<<"Before we start, you will have to input the puzzle into this program.\n\n";
		cout<<"You can either:-\n";
		cout<<"\t1. Input the puzzle by entering the values manually. (Enter 1)\n";
		cout<<"\t2. Input the puzzle by reading a file with values in it. (Enter 2)\n";
		cout<<"\t   The file must not have a name > 20 characters.\n";
		cout<<"\t   The file must be in the same directory as this C++ file.\n";
		cout<<"\t   The file must have all 81 values seperated with spaces.\n";
		cout<<"\t   Blank cells must be filled in as 0 (eg; 1 0 0  2 0 0 ...).\n";
		cout<<"\t   --> ";

		int choice;
		cin >> choice;

		if(choice==1) readFromConsole();
		else if(choice==2) readFromFile();
		else{
			while(true){
				cout<<"\nYou have entered an invalid option. Please Try again.\n";
				cout<<"\t   --> ";
				cin >> choice;
				if(choice==1) readFromConsole();
				else if(choice==2) readFromFile();
				else continue;
				break;
			}
		}
	}
	private:
	void readFromConsole(){
		cout<<"\nEnter the specified value when prompted.\n";
		cout<<"Enter 0 if cell is empty.\n\n";

		int row, col;

		for(row=0; row<9; row++){
			for(col=0; col<9; col++){
				int enteredValue;

				cout<<"Enter value for cell["<<row+1<<"]["<<col+1<<"] --> ";
				cin >> enteredValue;

				if(!(enteredValue >= 0 && enteredValue <= 9)){ 
					while(true){ 
						cout<<"Oops! You have entered a wrong value! Please Try again.\n";
						cout<<"Enter value for cell ["<<row+1<<"]["<<col+1<<"] --> ";
						cin>>enteredValue;

						if(enteredValue>=0 && enteredValue<=9) break;
					}
				}

				grid[row][col]=enteredValue;
			}
			cout<<"-------\n";
		}
		cout<<endl;
	}
	private:
	void readFromFile(){

		char filename[30]; //Getting filename.

		cout<<"\nEnter the name of the file that contains the Sudoku Puzzle.\n";
		cout<<"\t   --> ";
		cin>>filename;

		ifstream input_file; //Opening file for reading.
		input_file.open(filename,ios::in);

		int row, col;

		for(row=0; row<9; row++){ //Iterating over file values.
			for(col=0; col<9; col++){
				int readValue;

				input_file>>readValue;

				if(!(readValue>=0 && readValue<=9)){ //Checking bounds for input.
					cout<<"\nValue "<<((row*9)+col+1)<<" in "<<filename;
					cout<<" seems to be wrong! Correct the value and try again!\n";
					exit(EXIT_FAILURE);
				}

				grid[row][col]=readValue;

				if(input_file.eof()) break; //Breaking if EOF is reached.
			}
		}

		//Gotta have a line which lets us check the frame for any errors

		input_file.close();
		cout<<endl;
	}

	public:
	void setCellValue(int row, int col, int key){
		grid[row][col]=key;
	}
	public:
	int getCellValue(int row, int col){
		int cellValue=grid[row][col];
		return cellValue;
	}

	public:
	void displayGrid(){

		cout<<"=====================================";
		int row, col;
		for(row=0; row<9; row++){
			int cell=1;
			cout<<"\n||";
			for(col=0; col<9; col++){
				if(cell==3){
					cout<<" ";
					if(grid[row][col] != 0)
						cout<<grid[row][col]<<" ";
					else
						cout<<"  ";
					cout<<"||";
					cell=1;
				}
				else{
					cout<<" ";
					if(grid[row][col] != 0)
						cout<<grid[row][col]<<"  ";
					else
						cout<<"   ";
					cell++;
				}
			}
			if(row%3!=2) cout<<"\n++-----------++-----------++-----------++";
			else cout<<"\n++=====================================++";
		}
		cout<<" ";
	}

};

class SudokuSolver{

	int recursiveCount; //Stats variable
	SudokuGrid grid; //The grid object

	public:SudokuSolver(){
		recursiveCount=0;

		displayGrid();
		cout<<"\n\nCalculating possibilities...\n";
		cout<<"Backtracking across puzzle....\n";
		cout<<"Validating cells and values...\n\n";

		solve();

		displayGrid();
		cout<<"\n\n";
	}

	private:
	bool cellValueValid(int row, int col, int keyValue){
		int rowIter, colIter;

		//Checking if value exists in same column
		for(rowIter=0; rowIter<9; rowIter++){
			if(rowIter == row)
				continue;

			if(grid.getCellValue(rowIter, col) == keyValue)
				return false;
		}

		//Checking if value exists in same row
		for(colIter=0; colIter<9; colIter++){
			if(colIter == col)
				continue;

			if(grid.getCellValue(row, colIter) == keyValue)
				return false;
		}

		//Checking if value exists in the same 3x3 square block
		if( !ThreeByThreeGridValid(row, col, keyValue) )
			return false;

		// It is a valid key
		return true;
	}


	private:bool ThreeByThreeGridValid(int row, int col, int keyValue){
		int rowI, colI;

		int s_start[3] = {0, 3, 6};
		int s_end[3] = {3, 6, 9};
		int s_r = int(row / 3);
		int s_c = int(col / 3);


		for(rowI = s_start[s_r]; rowI < s_end[s_r]; rowI++){
			for(colI = s_start[s_c]; colI < s_end[s_c]; colI++){
				if(rowI == row && colI == col){
					continue;
				}

				if(grid.getCellValue(rowI, colI) == keyValue){
					return false;
				}
			}
		}

		return true;
	}

	private:
	bool gridSolved(){

		int flag;

		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				if(grid.getCellValue(i, j) == 0)
					return false;

				flag = cellValueValid(i, j, grid.getCellValue(i, j));

				if(flag == 0){
					return false;
				}
			}
		}

		return true;
	}


	private:
	bool singleCellSolve(){

		statsIncrement(); //This is used to see how many times the func is called.

		// If the grid full and correct return true and break the recursive calls
		if(gridSolved())
			return true;

        //statsPrint();
        //displayGrid();
        //cin.get();

		int rowIter, colIter, keyValueIter;

		for(rowIter=0; rowIter<9; rowIter++){
			for(colIter=0; colIter<9; colIter++){

				// Check empty cells and do the rest only for them
				// If cell's value is 0, it is empty
				if(grid.getCellValue(rowIter, colIter) == 0){

					// Check all the possible numbers as keys
					for(keyValueIter=1; keyValueIter<10; keyValueIter++){

						if(cellValueValid(rowIter, colIter, keyValueIter)){

							// Assign the key value
							grid.setCellValue(rowIter, colIter, keyValueIter);

							// Calling recursive function
							if(singleCellSolve())
								return true;

							// If grid is not solved, the key is not correct one, delete it and continue
							grid.setCellValue(rowIter, colIter, 0);
						}
					}

					// If there is not any valid for that cell program should go to back stage, [backtracking]
					if(grid.getCellValue(rowIter, colIter) == 0)
						return false;
				}
			}
		}

		// Grid did not solved
		return false;

	}

	
	private:void solve(){
		bool success=singleCellSolve();

		statsPrint();
		if(success)
			cout<<"QED. Your puzzle has been solved!\n\n";		// quod erat demonstrandum [what was to be shown]
		else
			cout <<"There is not any correct answer for this puzzle :( \n";
	}

	
	private:
	void displayGrid(){
		grid.displayGrid();
	}

	private:
	void statsIncrement(){
		recursiveCount++;
	}

	public:
	void statsPrint(){
		cout<<"\nThe singleCellSolve() function was recursively called "<<recursiveCount<<" times.\n";
	}

};
int main(){
	SudokuSolver ss;
	return 0;
}
