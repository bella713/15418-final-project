#include <iostream>
#include <string.h>
#include <chrono>
#include "omp.h"
using namespace std;

#include "sudoku_helper.cpp"

// Number 0 in input board indicates an empty cell
#define EMPTY 0

double start;

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

int SolveSudoku(int N, int** grid, int level)
{
	int row, col;
	if (!findEmptyCell(N, grid, row, col))
		return true;

	for (int num = 1; num <= N; num++)
	{
		if (isSafe(N, grid, row, col, num))
		#pragma omp task firstprivate(grid, row, col, num, level) shared(start) final(level>1)
		{
			int** copy_grid;	
			copy_grid = new int*[N];
			for (int i = 0; i < N; i++)
				copy_grid[i] = new int[N];		

			for (int x=0 ; x < N; x++){
  				for (int y=0 ; y < N; y++){
					copy_grid[x][y] = grid[x][y];
				}
			}	
						
			// memcpy(copy_grid,grid,N * N * sizeof(int));		

			copy_grid[row][col] = num;  			
			// grid[row][col] = num;

			if (SolveSudoku(N, copy_grid, level+1)){
				// return true;
				cout << '\n' << "Solution board: " << '\n';
				printGrid(N,copy_grid);
                double end = omp_get_wtime();
                double time_spent = end - start;  				
				cout << "Computation Time: " << time_spent << '\n';
				exit(0);

			}

			for (int i = 0; i < N; i++)
				delete copy_grid[i];	
			free(copy_grid);

			// grid[row][col] = EMPTY;
		}
	}
	//backtracks
    #pragma omp taskwait
	return 0;
}

int main(int argc, char* argv[])
{
	// get command argument
	int N = stoi(argv[1]);
	string filename = argv[2];

	// generate the 2D array for input board
	int** grid;
	grid = new int*[N];
	for (int i = 0; i < N; i++)
		grid[i] = new int[N];
		
	getGrid(filename, grid);

	cout << "Input board:" << '\n';
	printGrid(N, grid);

	start = omp_get_wtime();

	#pragma omp parallel shared(grid) num_threads(4)
	#pragma omp single nowait	
	{
		SolveSudoku(N, grid, 1);
	}

	return 0;
}