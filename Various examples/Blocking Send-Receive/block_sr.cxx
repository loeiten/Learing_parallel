// Example of blocking send and receive

#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main (int argc, char **argv) {
    int myrank,i;
    double a[100],b[100];
    // Create an instance of MPI_Status
    MPI_Status status;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    // Get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if( myrank == 0 )        /* Send a message */
    {
        for (i=0;i<100;++i)
            a[i]=sqrt(i);
        // Send the array a
        // Has 100 elements
        // NOTE: Is of type MPI_DOUBLE (allthough sepcified as double)
        // Send the stuff to process 1
        // Tag it with the number 17
        // The communicator is 17
        MPI_Send( a, 100, MPI_DOUBLE, 1, 17, MPI_COMM_WORLD );
    }
    else if( myrank == 1 )
        // Receive a from 0, but store it as b...
        MPI_Recv( b, 100, MPI_DOUBLE, 0, 17, MPI_COMM_WORLD, &status );   
  
    // Terminate MPI
    MPI_Finalize();
    
    return 0;
}
