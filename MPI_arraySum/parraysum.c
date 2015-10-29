#include "mpi.h"
#include <stdio.h>
#include <math.h>

void readArray(char * fileName, double ** a, int * n);
double sumArray(double * a, int low, int high) ;

int main(int argc, char * argv[])
{
  int  howMany;
  double sum;
  double * a;
  int myid, numprocs, x, low, high;

  if (argc != 2) {
    fprintf(stderr, "\n*** Usage: arraySum <inputFile>\n\n");
    exit(1);
  }
  readArray(argv[1], &a, &howMany);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  
  /* broadcast data */
  MPI_Bcast(data, howMany, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  /* add portion of data */
  x = howMany/numprocs; /* must be an integer */
  low = myid * x;
  high = low + x;

  sum = sumArray(a, low, high);
  printf("I got %lf from %d\n", myresult, myid);

  /* compute global sum */
  MPI_Reduce(&myresult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if(0 == myid) {
    printf("The sum is %lf.\n", result);
  }

  MPI_Finalize();
  
  return 0;
}

void readArray(char * fileName, double ** a, int * n) {
  int count, howMany;
  double * tempA;
  FILE * fin;

  fin = fopen(fileName, "r");
  if (fin == NULL) {
    fprintf(stderr, "\n*** Unable to open input file '%s'\n\n",
      fileName);
    exit(1);
  }

  fscanf(fin, "%d", &howMany);
  tempA = malloc(howMany * sizeof(double));
  if (tempA == NULL) {
    fprintf(stderr, "\n*** Unable to allocate %d-length array",
      howMany);
    exit(1);
  }

  for (count = 0; count < howMany; count++) {
    fscanf(fin, "%lf", &tempA[count]);
  }

  fclose(fin);

  *n = howMany;
  *a = tempA;
}

double sumArray(double * a, int low, int high) {

  int i;
  double myresult=0.0, result=0.0;

  for(i=low; i<high; i++) {
    myresult += *a;
    a++;
  }
  
  return result;
}