#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data, recv_data[8];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 8) {
        printf("This program works for up to 8 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    send_data = rank + 1;  // Each process sends its rank + 1 as data

    MPI_Gather(&send_data, 1, MPI_INT, recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Root process gathered data: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", recv_data[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    
    return 0;
}
