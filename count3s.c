#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

double start_time, stop_time;
int globalCount,myCount;
int *myArray;
int length_per_process;
int level;

int main(int argc, char **argv)
{
  int* data;
  int  m, n, myID, p, j, i, numProcs;
  int RootProcess=0;
  //Added by Alibek-
  length_per_process=0;

  //-----------------
  MPI_Status status;

  if (argc != 2) {
    printf("usage: %s <num_items>\n", argv[0]);
    return 1;
  }

  // Number of items to be sorted
  n = atoi(argv[1]);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myID);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  length_per_process = n/numProcs;
  myArray = (int *) malloc(length_per_process*sizeof(int));
  printf("Terminated here");

  /*Read the data, distribute it among the various processes*/
  if(myID==RootProcess)
  {
    // data generation
    srandom(0);
    // Make sure that n is a multiple of p
    for(p=1; p<numProcs; p++)
    {
      for (i=0;i<length_per_process;i++)
      {
        myArray[i] = random() % 100 + 1;
        ++j;
      }
      MPI_Send(myArray, length_per_process, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
    for (i=0;i<length_per_process;i++)
    {
      myArray[i] = myArray[i] = random() % 100 + 1;
      ++j;
    }
  }
  else
  {
    MPI_Recv(myArray, length_per_process, MPI_INT, RootProcess, 0, MPI_COMM_WORLD, &status);
  }

  start_time = clock();
  //Do the actual work
  for (i=0;i<length_per_process;i++)
  {
    if (myArray[i] == 3)
    {
      myCount++;
    }
  }

  printf("Number of 3s %d, MyID %d\n", myCount,myID);

  //Costom MPI-reduce()
  level = 1;
  while( level < numProcs )
  {
    if(myID % (2*level) == 0)
    {
      if( myID+level < numProcs )
      {
        MPI_Recv(&m, 1 , MPI_INT,myID+level,0,MPI_COMM_WORLD,&status);
        myCount += m;
      }
    }
    else
    {
      int neighborhood =  myID-level;
      MPI_Send(&myCount,1,MPI_INT,neighborhood,0,MPI_COMM_WORLD);
      break;
    }
    level = level*2;
  }

  //MPI_Reduce(&myCount, &globalCount, 1, MPI_INT, MPI_SUM, RootProcess, MPI_COMM_WORLD);

  stop_time = clock();

  if (myID == RootProcess)
  {
    printf("RootProcess myCount %d\n", myCount);
    printf("%d procs, %d items, %f seconds\n", numProcs, n, (stop_time-start_time)/CLOCKS_PER_SEC);
    printf("Success!\n");
  }

  free(myArray);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
