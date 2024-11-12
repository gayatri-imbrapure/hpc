#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_value = 42, recv_value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This program requires at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        MPI_Send(&send_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process %d sent value: %d\n", rank, send_value);
    } else if (rank == 1) {
        MPI_Recv(&recv_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received value: %d\n", rank, recv_value);
    }

    MPI_Finalize();
    
    return 0;
}
