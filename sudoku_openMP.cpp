#include <iostream>
#include <omp.h>
using namespace std;

#include "sudoku_helper.cpp"

// Number 0 in input board indicates an empty cell
#define EMPTY 0

bool findEmptyCell(int N, int** grid, int& row, int& col)
{
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++)
			if (grid[row][col] == EMPTY)
				return true;
	return false;
}


bool isSafe(int N, int** grid, int row,
			int col, int num)
{
	int sqN = sqrt(N);

	if (grid[row][col] != EMPTY)
		return false;

	// check row
	for (int col = 0; col < N; col++)
		if (grid[row][col] == num)
			return false;

	// check col
	for (int row = 0; row < N; row++)
		if (grid[row][col] == num)
			return false;

	// check box
	int boxStartRow = row - row % sqN;
	int boxStartCol = col - col % sqN;
	for (int row = 0; row < sqN; row++)
		for (int col = 0; col < sqN; col++)
			if (grid[row + boxStartRow][col + boxStartCol] == num)
				return false;

	return true;
}

bool SolveSudoku(int N, int** grid)
{
	int row, col;
	if (!findEmptyCell(N, grid, row, col))
		return true;

	for (int num = 1; num <= N; num++)
	{
		if (isSafe(N, grid, row, col, num))
		{
			grid[row][col] = num;
			if (SolveSudoku(N, grid))
				return true;

			grid[row][col] = EMPTY;
		}
	}
	//backtracks
	return false;
}

int main(int argc, char* argv[])
{
	// get command argument
	int N = stoi(argv[1]);
	string filename = argv[2];

	// int size = 16;

	// generate the 2D array for input board

	// int grid[N][N];
	int** grid;
	grid = new int*[N];
	for (int i = 0; i < N; i++)
		grid[i] = new int[N];
		
	getGrid(filename, grid);

	cout << "Input board:" << '\n';
	printGrid(N, grid);

	auto start = chrono::system_clock::now();
	bool result = SolveSudoku(N, grid);
	auto end = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end - start;

	// print result
	if (result){
		cout << '\n' << "Solution board: " << '\n';
		printGrid(N, grid);
		cout << "Computation Time: " << elapsed_seconds.count() << '\n';
	}
	else{
		cout << "No solution exists";
	}

	return 0;
}