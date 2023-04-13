#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

void printGrid(int N, int** grid)
{
	int sqN = sqrt(N);
	bool big_board = (N > 9);
	string segment;

	if (N == 9)
		segment = "------";
	else if (N == 16)
		segment = "------------";
	else if (N == 25)
		segment = "---------------";

	for (int row = 0; row < N; row++)
	{
		if (row != 0 && row % sqN == 0){
			cout << segment << "+" << segment << "-+";
			if (N == 16)
				cout << segment << "-+";
			if (N == 25)
				cout << segment << "-+" << segment << "-+";			
			cout << segment << '\n';
		}

		for (int col = 0; col < N; col++){
			if (col != 0 && col % sqN == 0)
				printf("| ");
			if (big_board && grid[row][col] < 10)
				cout << " ";
			cout << grid[row][col] << " ";
		}
		cout << endl;
	}
}

void getGrid(string filename, int** grid){

    fstream test_file;
    
    test_file.open(filename, ios::in); 
    if (test_file.is_open()) { 
        string cur_line;

        // Read data from the file object and put it into a string.
		int row = 0;
        while (getline(test_file, cur_line)) { 

			stringstream cur_line_ss;
			string temp;
			int cur_num;
			int col = 0;

			cur_line_ss << cur_line;
			while (!cur_line_ss.eof()){
				cur_line_ss >> temp;
				if (stringstream(temp) >> cur_num){
					grid[row][col] = cur_num;
				}
				temp = " ";
				col++;
			}
			row++;
        }
        
        test_file.close(); 
    }
}