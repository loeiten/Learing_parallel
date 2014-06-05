// Name:    game_of_life.cxx
// Purpose: Simulate the game of life by John Conway
// Version: 1.0
// Date:    2014.06.03
// Author:  Michael Loeiten
// Mail:    michael.l.magnussen@gmail.com

// TODO:
// Make matrix and periodic BC into classes
// Inputs: grid_number_x, grid_number_y, repeat
// Or: Read given initial condition

#include <stdlib.h>  // For the exit function
#include <random>    // The random class
#include <iostream>  // Print to the terminal
#include <new>       // Allocate the memory
#include <fstream>   // File stream
#include <iomanip>   // setw and left

// Declearing functions
void print_matrix_to_file (int**& matrix, int rows, int cols, int end_time);

// Possible inputs
// Grid_number
// Number time steps

// Printing: ASCII
// Header with grid_number_x, grid_number_y and time_steps
int main (int argc, char* argv[])
{
    int grid_number; int time_steps;
    //{{{ Abort if there are too few command-line arguments
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " grid_number time_steps"
                  << std::endl;
        exit(1);
    } else {
        grid_number = atoi(argv[1]); time_steps = atoi(argv[2]);
    }
    //}}}

    int rows     = grid_number;
    int cols     = grid_number;
    int end_time = time_steps;

    // Dynamic allocation in C++ (NOT THE MOST EFFICIENT [use eventually the
    // matrix class written in GEO4070 for this])
    // Source
    // http://stackoverflow.com/questions/1403150/how-do-you-dynamically-allocate-a-matrix
    // The current matrix
    // +2 for the ghost cells (one on each side)

    // NOTE: The for loop runs as long as 'condition' is TRUE
    //       (which is evaluated before incrimentation)
    //       Thus, in 'condition' <= includes the number on the RHS,
    //       and      'condition'  < EXCLUDES the number on the RHS 
    //                               (for example counting indices from zero)
    int** matrix = new int*[rows + 2];
    for (int r = 0; r < rows + 2; ++r){
        matrix[r] = new int[cols + 2];
    }
    // The matrix containing all the time steps
    int** all_time_steps = new int*[rows*end_time];
    for (int r = 0; r < rows*end_time; ++r){
        all_time_steps[r] = new int[cols];
    }


    //{{{ Creating the initial configuration
    // Doing this by a random generator
    std::random_device rd;
    // Setting min and max
    int min = 0, max = 1;
    // C++ is ROW-MAJOR (transversing the ELEMENTS (columns) of one row is the
    // quickest) 
    std::cout << std::endl << "Getting initial configuration..." << std::endl;
    // Looping over the REAL matrix (excluding the ghost cells)
    // Start from 1 due to the ghost cells (there is additional 1 col/row in
    // the matrix available than cols/rows)
    for (int r = 1; r <= rows; ++r){
        for (int c = 1; c <= cols; ++c){
            matrix[r][c] = min + (rd() % (int)(max - min + 1));}
    }
    // Saving the initial configuration (done in separate loop due to indexing)
    for (int r = 0; r < rows; ++r){
        for (int c = 0; c < cols; ++c){
        // + 1 in the matrix due to the ghost cells
            all_time_steps[r][c] = matrix[r+1][c+1];}
    }
    std::cout << "...complete" << std::endl;
    //}}}

    //{{{ Playing the game
    int sum;

    if (end_time > 1){
        std::cout << std::endl << "Playing game of life (may the odds be EVER "
                  << "in your favour)..." << std::endl;}
    // We start the time at 1, because we've already saved the initial configuration
    for (int t = 1; t < end_time; ++t){
        // Fill in the gost cells (periodic boundary conditions)
        // Special treatment of the corners
        // Notice that the index we are copying really are rows, cols since the
        // index of the real matrix starts at on 1
        matrix[0]     [0]      = matrix[rows][cols];
        matrix[0]     [cols+1] = matrix[rows][1];
        matrix[rows+1][0]      = matrix[1]   [cols];
        matrix[rows+1][cols+1] = matrix[1]   [1];
        // Start at 1 so that we do not index the ghost points in the indexed
        // value to be copied
        for (int c = 1; c <= cols; ++c){
            // Top boundary    = real matrix bottom
            matrix[0]     [c] = matrix[rows][c];
            // Bottom boundary = real matrix top
            matrix[rows+1][c] = matrix[1][c];
        }
        for (int r = 1; r <= rows; ++r){
            // Left boundary   = real matrix right
            matrix[r][0]      = matrix[r][cols];
            // Right boundary  = real matrix left
            matrix[r][cols+1] = matrix[r][1];
        }

        // Start at 1 due to the ghost points
        for (int r = 1; r <= rows; ++r){
            for (int c = 1; c <= cols; ++c){

                // The sum of these cells determines the fate of the current
                // cell
                // The cell has eigth surrounding cells
                sum = matrix[r-1][c-1] +
                      matrix[r]  [c-1] + 
                      matrix[r+1][c-1] +
                      matrix[r-1][c+1] +
                      matrix[r]  [c+1] +
                      matrix[r+1][c+1] +
                      matrix[r-1][c]   +
                      matrix[r+1][c]   ;

                // Decision making

                // The result will be stored in the all_time_steps matrix
                // After looped through all rows and columns, then matrix will
                // be updated with results stored in the all_time_steps matrix

                // Minus one in the index of all_time_steps due to the fact
                // that we are starting to count r and c from one
                if (sum == 3){
                    all_time_steps[r-1 + t*rows][c-1] = 1;}
                else if (sum == 2){ 
                    all_time_steps[r-1 + t*rows][c-1] = matrix[r][c];
                    // Nothing happens
                    matrix[r][c] = matrix[r][c];}
                else{
                    all_time_steps[r-1 + t*rows][c-1] = 0;}

            }
        }
        // Updating the matrix with the just optained values which are stored
        // in all_time_steps
        for (int r = 0; r < rows; ++r){
            for (int c = 0; c < cols; ++c){
                // + 1 in the matrix due to the ghost cells
                matrix[r+1][c+1] = all_time_steps[r + t*rows][c];
            }
        }
    }
    if (end_time > 1){
        std::cout << "...complete" << std::endl;}
    //}}}

    // Print the matrix to the file (passing by reference)
    print_matrix_to_file(all_time_steps, rows*end_time, cols, end_time);

    return 0;
}


//// Inculsion of a dynamic multidimensional array
void print_matrix_to_file (int**& matrix, int rows, int cols, int end_time)
{
    std::cout << std::endl << "Writing the results to game_of_life.dat..." << std::endl;

    // Include a padding of 1
    int width = 2;

    // Opens matrix to print, the second arguments deletes previous input
    std::ofstream matrix_file ("game_of_life.dat", std::ios::trunc);
    // Check's if it was possible to open the file
    if (matrix_file.is_open())
    {
        // Print a simple header
        matrix_file << "# Rows=" << rows << " cols=" << cols << " t=" 
                    << end_time << std::endl;
        // Print the matrix
        for(int row = 0; row < rows; row++){
            for(int col = 0; col < cols; col++){
                // Left flushes to the left
                matrix_file << std::setw(width) << std::left
                            << matrix[row][col];}
            // New line
            matrix_file << "\n";}
        // Close the file
        matrix_file.close();
    }
    else std::cout << "Unable to open file!" << std::endl;

    std::cout << "...complete" << std::endl;
}
