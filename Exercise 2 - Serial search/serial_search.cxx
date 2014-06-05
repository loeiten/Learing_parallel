// Name:    serial_search.cxx
// Purpose: Search for a target in an input array
// Version: 1.0
// Date:    2014.06.04
// Author:  Michael Loeiten
// Mail:    michael.l.magnussen@gmail.com

// TODO:
// Smarter way of reading number of lines and put it into an array

#include <stdlib.h>  // For the exit function
#include <iostream>  // Print to the terminal
#include <new>       // Allocate the memory
#include <fstream>   // File stream

// Declearing functions
void print_matrix_to_file (int**& matrix, int rows, int cols, int end_time);

// Printing: ASCII
int main (int argc, char* argv[])
{
    char* input_file; char* output_file; int target;
    //{{{ Abort if there are too few command-line arguments
    if (argc <= 2) {
        std::cout << "Usage: " << argv[0] << " input_file output_file target"
                  << std::endl;
        exit(1);
    } else {
        input_file = argv[1]; output_file = argv[2]; target = atoi(argv[3]);
    }
    //}}}

    //{{{ Loading the array into the memory
    // In order to create the array, we need to know how many entries our file
    // has.
    int number_of_lines = 0;
    std::string line;
    std::ifstream the_input_file(input_file);

    while (std::getline(the_input_file, line))
        ++number_of_lines;

    std::cout << input_file << " has " << number_of_lines << " lines" 
              << std::endl;

    // Dynamically allocate memory
    int* array = new int[number_of_lines];

    // We will close and open the file in order to mix getfile and thd the >>
    // operator
    the_input_file.close();
    the_input_file.open(input_file, std::fstream::in);

    // Put the stuff into the array
    for (int index = 0; index < number_of_lines; ++index){
        the_input_file >> array[index];
    }
    //}}}

    // Opening outputfile to print to
    std::ofstream the_output_file(output_file);
    // Specify the format on the output
//    the_output_file.setf()

    std::cout << "Searching..." << std::endl; 

    // Searching routine
    for (int index = 0; index < number_of_lines; ++index){
        if (array[index] == target){
            the_output_file << index << std::endl;
        }
    }

    std::cout << "...search complete.\n Written to " << output_file 
              << std::endl; 

    return 0;
}
