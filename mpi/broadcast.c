#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int broadcast_value = 42;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This program requires at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Broadcast from process 0 to all other processes
    MPI_Bcast(&broadcast_value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received broadcasted value: %d\n", rank, broadcast_value);

    MPI_Finalize();
    
    return 0;
}
