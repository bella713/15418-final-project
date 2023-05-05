Parallelization in Sudoku Solving Algorithm
==================================

The project aims to implement a Sudoku solver using backtracking algorithm, and optimize it with parallelization using OpenMP and MPI.


## Website and Reports

The website to this project:
[Project website](https://bella713.github.io/15418-final-project/)

Project proposal, milestone report and final report are linked here:

[Project proposal](./project_proposal.pdf)

Project milestone report

Project final report

## Instructions

To compile the file: <br>
**Serial Algorithm** g++ -o serial sudoku_serial.cpp <br>
**OpenMP Algorithm** g++ -o openmp sudoku_openmp.cpp -fopenmp <br>
**MPI Algorithm** mpic++ -o mpi sudoku_mpi.cpp <br>

To run serial file: <br>
./serial <board_size> <test file> <br>
./serial 9 test_files/board_9x9_1.txt <br>
./serial 16 test_files/board_16x16_1.txt <br>
./serial 25 test_files/board_25x25_1.txt <br>

To run OpenMP file:
./openmp <board_size> <test file> <br>
./openmp 9 test_files/board_9x9_1.txt <br>
./openmp 16 test_files/board_16x16_1.txt <br>
./openmp 25 test_files/board_25x25_1.txt <br>

To run MPI file: <br>
mpirun -n <number_processes> mpi <board_size> <test_file> <br>
mpirun -n 2 mpi 9 test_files/board_9x9_1.txt <br>
mpirun -n 4 mpi 9 test_files/board_9x9_1.txt <br>
mpirun -n 4 mpi 16 test_files/board_16x16_1.txt <br>
