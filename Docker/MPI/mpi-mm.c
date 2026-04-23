/**********************************************************************                                                                                      
 * MPI-based matrix multiplication AxB=C                                                                                                                     
 *********************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h"

MPI_Status status;

int main(int argc, char **argv)
{

  int numtasks,taskid,numworkers,source,dest,rows,offset,i,j,k;

  double start, stop;

  int N = atoi(argv[1]);
  double a[N][N], b[N][N], c[N][N];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  numworkers = numtasks-1;

  /*---------------------------- master ----------------------------*/
  if (taskid == 0) {
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
          a[i][j] = (rand() % 4) + 1;
          b[i][j] = (rand() % 4) + 1;
      }
    }

    start = MPI_Wtime();

    if (numworkers == 0) {
      /* Ejecución serial cuando solo hay 1 proceso */
      for (k=0; k<N; k++)
        for (i=0; i<N; i++) {
          c[i][k] = 0.0;
          for (j=0; j<N; j++)
            c[i][k] = c[i][k] + a[i][j] * b[j][k];
        }
    } else {
      /* send matrix data to the worker tasks */
      rows = N/numworkers;
      offset = 0;

      for (dest=1; dest<=numworkers; dest++)
      {
        MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
        MPI_Send(&b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
        offset = offset + rows;
      }

      /* wait for results from all worker tasks */
      for (i=1; i<=numworkers; i++)
      {
        source = i;
        MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
      }
    }

    stop = MPI_Wtime();

    double elapsed = stop - start;
    int minutes = (int)(elapsed / 60);
    double remaining = elapsed - (minutes * 60);
    
    printf("real    %dm%.6fs\n", minutes, remaining);
    printf("user    %dm%.6fs\n", minutes, remaining);
    printf("sys     %dm%.6fs\n", 0, 0.0);
  }

  /*---------------------------- worker----------------------------*/
  if (taskid > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    /* Matrix multiplication */
    for (k=0; k<N; k++)
      for (i=0; i<rows; i++) {
        c[i][k] = 0.0;
        for (j=0; j<N; j++)
          c[i][k] = c[i][k] + a[i][j] * b[j][k];
      }


    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
