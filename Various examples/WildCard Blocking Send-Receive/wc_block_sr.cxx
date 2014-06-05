#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main (int argc, char **argv) {
    int myrank,i,count;
    // Create an instance of MPI_Status
    MPI_Status status;
    double a[100],b[300];

    // Initialize MPI
    MPI_Init(&argc, &argv);
    // Get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if( myrank == 0 ) {
        for (i=0;i<100;++i)
            a[i]=sqrt(i);
        // 0 is sending the message
        MPI_Send( a, 100, MPI_DOUBLE, 1, 17, MPI_COMM_WORLD );
    }
    else if( myrank == 1 ){
        // Receive message with WILDCARD, notice that it is allowed to take
        // more than what was sent
        MPI_Recv( b, 300, MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,
                  MPI_COMM_WORLD, &status );   
        MPI_Get_count(&status,MPI_DOUBLE,&count);
        printf("P:%d message came from rank %d\n",myrank,status.MPI_SOURCE);
        printf("P:%d message had tag %d\n",myrank,status.MPI_TAG);
        printf("P:%d message size was %d\n",myrank,count);
    }

    // Terminate
    MPI_Finalize();
    return 0;
}
