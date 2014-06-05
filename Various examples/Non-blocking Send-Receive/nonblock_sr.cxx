// Nonblocking send and receive (avoiding deadlock)
// If we would use blocking, the program would go into deadlock

#include <stdio.h>
#include <mpi.h>
#include <iostream> 

int main (int argc, char **argv) {
    int myrank;
    MPI_Request request; // Make an instance of MPI_Request
    MPI_Status status;
    double a[100], b[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if( myrank == 0 ) {
        // Post a receive, send a message, then wait
        MPI_Irecv( b, 100, MPI_DOUBLE, 1, 19, MPI_COMM_WORLD, &request );
        MPI_Send( a, 100, MPI_DOUBLE, 1, 17, MPI_COMM_WORLD );

        // Wait is blocking the processor until the receive is finished
        int wait_error = MPI_Wait( &request, &status );
        std::cout << "wait_error returned " << wait_error << std::endl;
    }
    else if( myrank == 1 ) {
        // Post a receive, send a message, then wait
        MPI_Irecv( b, 100, MPI_DOUBLE, 0, 17, MPI_COMM_WORLD, &request );   
        MPI_Send( a, 100, MPI_DOUBLE, 0, 19, MPI_COMM_WORLD );

        // Test will not block (can also return an integer for the error)
        int flag = 0;
        while(flag == 0){
            MPI_Test( &request, &flag, &status );
            if (flag == 0){
                std::cout << "The flag is " << flag << "...better wait"
                          << std::endl;
            }
            else if (flag==1 ){ 
                std::cout << "The flag is " << flag << "...hurray :D"
                          << std::endl;
            }
        }
    }

    MPI_Finalize(); 

    return 0;
}
