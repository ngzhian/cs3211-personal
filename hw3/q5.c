#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int my_rank, num_procs;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  
  int maxn = 12;
  int num_rows = 12/num_procs;

  int **my_rows = malloc(num_rows * maxn * sizeof(int));
  int *ghost_row_top = malloc(maxn * sizeof(int));
  int *ghost_row_bottom = malloc(maxn * sizeof(int));
  int offset_to_bottom = (num_rows-1) * maxn;
  int start_row = my_rank * num_rows;
  int end_row = start_row + num_rows - 1;

  if (num_procs < 2) {
    MPI_Finalize();
    exit(0);
  }
  if (my_rank == 0) {
    MPI_Request send_request;
    MPI_Request recv_request;
    MPI_Isend(my_rows + offset_to_bottom, maxn, MPI_INT, my_rank + 1, my_rank, MPI_COMM_WORLD, &send_request);
    MPI_Irecv(ghost_row_bottom, maxn, MPI_INT, my_rank + 1, my_rank + 1, MPI_COMM_WORLD, &recv_request);
  } else if (my_rank == num_procs - 1) {
    MPI_Request send_request;
    MPI_Request recv_request;
    MPI_Isend(my_rows, maxn, MPI_INT, my_rank - 1, my_rank, MPI_COMM_WORLD, &send_request);
    MPI_Irecv(ghost_row_top, maxn, MPI_INT, my_rank - 1, my_rank - 1, MPI_COMM_WORLD, &recv_request);
  } else {
    MPI_Request send_request;
    MPI_Request send_request_2;
    MPI_Request recv_request;
    MPI_Request recv_request_2;

    MPI_Isend(my_rows, maxn, MPI_INT, my_rank - 1, my_rank, MPI_COMM_WORLD, &send_request);
    MPI_Isend(my_rows + offset_to_bottom, maxn, MPI_INT, my_rank + 1, my_rank, MPI_COMM_WORLD, &send_request_2);

    MPI_Irecv(ghost_row_top, maxn, MPI_INT, my_rank - 1, my_rank - 1, MPI_COMM_WORLD, &recv_request);
    MPI_Irecv(ghost_row_bottom, maxn, MPI_INT, my_rank + 1, my_rank + 1
        , MPI_COMM_WORLD, &recv_request);
  }

  MPI_Finalize();
}
