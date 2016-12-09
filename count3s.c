#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

double start_time, stop_time;

int main(int argc, char **argv)
{
  int myCount;
  int* data;
  int m, n, myID, p, j, i, numProcs, globalCount;
  int RootProcess=0;
  //Added by Alibek-
  int length_per_process=0;
  int *sub_data;
  int *myArray;
  int level;
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

  j=0;
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
  /*
  level = 1;
  while( level < p )
  {
    if(id % (2*level) == 0)
    {
      if( id+level < p )
      {
        MPI_Recv(&m, 1 , MPI_INT,id+level,0,MPI_COMM_WORLD,&status);
        sub_data1 = (int *)malloc(m*sizeof(int));
        MPI_Recv(sub_data1,m,MPI_INT,id+level,0,MPI_COMM_WORLD,&status);
        sub_data = merge(sub_data,sub_data1, length_per_process,m);
        length_per_process = length_per_process + m;
      }
    }
    else
    {
      int neighborhood = id-level;
      MPI_Send(&length_per_process,1,MPI_INT,neighborhood,0,MPI_COMM_WORLD);
      MPI_Send(sub_data,length_per_process,MPI_INT,neighborhood,0,MPI_COMM_WORLD);
      break;
    }
    level = level*2;
  }
  */
  myCount=0;
  globalCount=0;
  //Do the actual work
  for (i=0;i<length_per_process;i++)
  {
    if (myArray[i] == 3)
    {
      myCount++;
    }
  }
  MPI_Reduce(&myCount, &globalCount, 1, MPI_INT, MPI_SUM, RootProcess, MPI_COMM_WORLD);

  stop_time = clock();

  if (myID == RootProcess)
  {
    printf("%d procs, %d items, %f seconds\n", numProcs, n, (stop_time-start_time)/CLOCKS_PER_SEC);

    else
    {
        printf("Success!\20132n");
    }
  }
  MPI_Finalize();
}
