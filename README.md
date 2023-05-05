Parallelization in Sudoku Solving Algorithm
==================================

The project implements Sudoku solver using backtracking algorithm, and optimize it with parallelization using OpenMP and MPI.

## Introduction

Sudoku is a classical logic-based combinatorial puzzle game originated from Japan. In classic Sudoku, the player is given a partially filled 9 x 9 grid. The objective is to fill the grid with digits such that each row, column, and 3 x 3 sub-grid contain all the digits from 1-9. In this project, different N * N of Sudoku boards will be tested, such as 16 x 16 and 25 x 25, which will further assess the performance of the project.
 
Sudoku solving algorithms include testing for different possible answers for each empty cells. The inherent nature of this problem allows for parallelization to optimize it.

The project implementes Sudoku solver in three ways: serial backtracking, openMP parallelized backtracking, and MPI backtracking. 

## Website and Reports

The website to this project: <br>
[Project website](https://bella713.github.io/15418-final-project/)

Project documents are linked here:

[Project proposal](./documents/project_proposal.pdf) <br>
[Project milestone report](./documents/project_milestone_report.pdf) <br>
[Project final report](./documents/project_final_report.pdf) <br>

## Instructions

To compile the file: <br>
**Serial Algorithm** <br>
	g++ -o serial sudoku_serial.cpp <br>
**OpenMP Algorithm** <br>
	g++ -o openmp sudoku_openmp.cpp -fopenmp <br>
**MPI Algorithm** <br>
	mpic++ -o mpi sudoku_mpi.cpp <br>

To run serial file: <br>
	./serial <board_size> <test_file> <br>
	./serial 9 test_files/board_9x9_1.txt <br>
	./serial 16 test_files/board_16x16_1.txt <br>
	./serial 25 test_files/board_25x25_1.txt <br>

To run OpenMP file:<br>
	./openmp <board_size> <test_file> <br>
	./openmp 9 test_files/board_9x9_1.txt <br>
	./openmp 16 test_files/board_16x16_1.txt <br>
	./openmp 25 test_files/board_25x25_1.txt <br>

To run MPI file: <br>
	mpirun -n <number_processes> mpi <board_size> <test_file> <br>
	mpirun -n 2 mpi 9 test_files/board_9x9_1.txt <br>
	mpirun -n 4 mpi 9 test_files/board_9x9_1.txt <br>
	mpirun -n 4 mpi 16 test_files/board_16x16_1.txt <br>
