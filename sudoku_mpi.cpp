#include <iostream>
#include "sudoku_helper.cpp"
#include <chrono>
#include "mpi.h"

// Number 0 in input board indicates an empty cell
#define EMPTY 0

bool findEmptyCell(int N, int* grid, int& row, int& col)
{
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++)
			if (grid[(row*N) + col] == EMPTY)
				return true;
	return false;
}

// void printGrids(int rank, int nproc, int** grid, int N)
// {
// 	for(int i = 0; i < nproc; i++)
// 	{
// 		if(i == rank)
// 		{
// 			printf("Grid %d\n", i);
// 			printGrid(N, grid);
// 			printf("\n\n");
// 		}

// 		MPI_Barrier(MPI_COMM_WORLD);
// 	}
// }

void getBounds(int rank, int nproc, int* startInd, int* endInd, int N)
{
	int sq = N*N;
    int spacing = (sq)/nproc;

	*startInd = rank*spacing;

	if(rank == nproc-1)
	{
		*endInd = sq;
	}
	else
	{
		*endInd = *startInd+spacing;
	}
}

void getRowCol(int index, int N, int* row, int* col)
{
	*row = index / N;
	*col = index % N;
}

bool isSafe(int N, int* grid, int row,
			int col, int num)
{
	int sqN = sqrt(N);

	if (grid[(row*N)+col] != EMPTY)
		return false;

	// check row
	for (int c = 0; c < N; c++)
		if (grid[(row*N)+c] == num)
			return false;

	// check col
	for (int r = 0; r < N; r++)
		if (grid[(r*N)+col] == num)
			return false;

	// check box
	int boxStartRow = row - (row % sqN);
	int boxStartCol = col - (col % sqN);

	// printf("%d,%d, %d,%d\n", row, col, boxStartRow, boxStartCol);
	for (int r = boxStartRow; r < boxStartRow+sqN; r++)
		for (int c = boxStartCol; c < boxStartCol+sqN; c++)
			if (grid[(r*N) + c] == num)
				return false;

	return true;

}

int checkSquare(int N, int* grid, int row, int col)
{
	int res = -1;
	int sqN = sqrt(N);

	int boxStartRow = row - (row % sqN);
	int boxStartCol = col - (col % sqN);

	for(int guess = 1; guess < N; guess++)
	{
		if(!isSafe(N, grid, row, col, guess))
		{
			continue;
		}
		
		//check row and col
		bool b1 = true;
		for(int i = 0; i < N; i++)
		{
			
			if(i != row && isSafe(N, grid, i, col, guess))
			{
				b1 = false;
				break;
			}

			if(i != col && isSafe(N, grid, row, i, guess))
			{
				b1 = false;
				break;
			}
		}

		if(b1 == false)
			continue;

		//check square
		for(int i = boxStartRow; b1 && (i < boxStartRow+sqN); i++)
		{
			for(int j = boxStartCol; b1 && (j < boxStartCol+sqN); j++)
			{
				if(i != row && j != col && isSafe(N, grid, i, j, guess))
				{
					b1 = false;
				}
			}
		}

		if(b1)
		{
			if(res != -1)
			{
				res = guess;
			}
			else
			{
				return -1;
			}
		}
	}

	return res;
}

bool constraintCheck(int N, int* grid, int startInd, int endInd)
{
	bool madeUpdate = false;
	int row;
	int col;
	for(int i = startInd; i < endInd; i++)
	{
		getRowCol(i, N, &row, &col);

		if(grid[i] != EMPTY)
			continue;

		int g = checkSquare(N, grid, row, col);

		if(g != -1)
		{
			grid[i] = g;
			madeUpdate = true;
		}
	}
	return madeUpdate;
}

bool SolveSudoku(int N, int* grid, int* grid2, int rank, int nproc, int startInd, int endInd, 
				int send_counts [], int recv_counts [], int send_displs [], int recv_displs [], int filled)
{
	int threshold = (N*N)*(0.80); 
	int row, col;

	bool local_bool;
	bool global_bool = true;
	while(global_bool && filled > threshold)	
	{
		local_bool = constraintCheck(N, grid, startInd, endInd);
			
		MPI_Alltoallv(&grid[0], send_counts, send_displs, MPI_INT, 
					  &grid2[0], recv_counts, recv_displs, MPI_INT, MPI_COMM_WORLD);


		MPI_Allreduce(&local_bool, &global_bool, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);
			
		memcpy(grid, grid2, sizeof(int)*N*N);
	}
	
	
	if (!findEmptyCell(N, grid, row, col))
		return true;
	
	for (int num = 1; num <= N; num++)
	{
		bool b = isSafe(N, grid, row, col, num);
		if (b)
		{
			grid[(row*N) + col] = num;
			if (SolveSudoku(N, grid, grid2, rank,nproc, startInd,endInd, send_counts, recv_counts, send_displs, recv_displs, filled+1))
				return true;
			
			grid[(row*N) + col] = EMPTY;			
		}
	}
	//backtracks
	return false;
}

int main(int argc, char* argv[])
{
    int pid;
    int nproc;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    // Get process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    // Get total number of processes specificed at start of run
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    
    int startInd;
	int endInd;

	int recv_counts [nproc];
	int recv_displs [nproc];

	int send_counts [nproc];
	int send_displs [nproc];

	// get command argument
	int N = stoi(argv[1]);
	string filename = argv[2];
	
	int dis = 0;
	for(int i = 0; i < nproc; i++)
	{
		getBounds(i, nproc, &startInd, &endInd, N);

		recv_counts[i] =  endInd - startInd; 
		recv_displs[i] = dis;
		dis += endInd - startInd;
	}

	getBounds(pid, nproc, &startInd, &endInd, N);

	for(int i = 0; i < nproc; i++)
	{
		send_displs[i] = startInd;
		send_counts[i] = endInd-startInd;
	}
	
	
	// generate the 2D array for input board
	int** grid;
	grid = new int*[N];
	for (int i = 0; i < N; i++)
		grid[i] = new int[N];


	int* g = new int [N*N];

	int* grid2;
	grid2 = new int [N*N];
		
	getGrid(filename, grid);

	int filled = 0;
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			g[(i*N)+j] = grid[i][j];
			grid2[(i*N)+j] = grid[i][j];

			if(grid[i][j] != EMPTY)
				filled += 1;
		}
	}

	if(pid== 0)
	{
		cout << "Input board:" << '\n';
		printGrid(N, grid);
	}
	
	bool result = false;
	
	MPI_Barrier(MPI_COMM_WORLD);
	auto start = chrono::system_clock::now();
	
	result = SolveSudoku(N, g, grid2, pid, nproc, startInd, endInd, send_counts, recv_counts, send_displs, recv_displs, filled);
	MPI_Barrier(MPI_COMM_WORLD);
	
	auto end = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end - start;
	
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			grid[i][j] = g[i*N+j];
		}
	}

	// print result
	if(pid == 0)
	{
		if (result){
			cout << '\n' << "Solution board: " << '\n';
			printGrid(N, grid);
			cout << "Computation Time: " << elapsed_seconds.count() << '\n';
		}
		else{
			cout << "No solution exists\n";
		}
	}

	MPI_Finalize();

	return 0;
}