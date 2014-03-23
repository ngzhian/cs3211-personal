#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int my_rank, num_procs;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  int recv_buf;

  if (my_rank == 0) {
    if (num_procs > 1) {
      int answer = 42;
      MPI_Send(&answer, 1, MPI_INT, 0, my_rank, MPI_COMM_WORLD);
    }
  } else {
    MPI_Status status;
    MPI_Recv(&recv_buf, 1, MPI_INT, my_rank - 1, my_rank - 1, MPI_COMM_WORLD, &status);
    printf("processor %d received %d\n", my_rank, recv_buf);
    if (my_rank != num_procs-1) {
      MPI_Send(&recv_buf, 1, MPI_INT, my_rank + 1, my_rank, MPI_COMM_WORLD);
    }
  }
  MPI_Finalize();
}
