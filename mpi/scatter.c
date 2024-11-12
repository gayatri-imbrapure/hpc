#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This program requires at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        MPI_Scatter(send_data, 1, MPI_INT, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatter(NULL, 0, MPI_INT, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    printf("Process %d received value: %d\n", rank, recv_data);

    MPI_Finalize();
    
    return 0;
}
