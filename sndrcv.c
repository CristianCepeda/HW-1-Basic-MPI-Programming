// /*==========================================================================
// |   Source code:  [sndrcv.c]
// |        Author:  [Cristian C Cepeda]
// |    Student ID:  [4959207]
// |    Assignment:  Program #[1] Problem #[4] [sndrcv.c]
// |
// |        Course:  [COP 4520 (Introduction to Parallel Computing)]
// |       Section:  [U01]
// |    Instructor:  Jason Liu
// |      Due Date:  [06 February 2018], at the beginning of class
// |
// |	I hereby certify that this collective work is my own
// |	and none of it is the work of any other person or entity.
// |
// |	__Cristian C Cepeda____________________________________ [Signature]
// |
// |        Language:  [Program Language: C]
// |     Compile/Run:
// |
// |                  COMPILE
// |                  Once you are in the correct directory where all the
// |                  files are found enter
// |
// |                  make
// |
// |                  - This will compile all the necessary files needed to
// |                    run the program. Incase you just want to compile this
// |                    file then you can run this command to compile
// |
// |                      - mpicc -O -o sndrcv sndrcv.c
// |
// |                  - This will create an executable file named
// |                    "sndrcv"
// |
// |                  RUN
// |                  - Enter this to Run a test version of it
// |
// |                  - mpirun -np 4 --hostfile my-hostfile sndrcv
// |
// |                  - Where 4 is an arbitrary number and you can insert any
// |                    number to create that set of processes run the program.
// |
// |                  - The "--hostfile my-hostfile" was necessary in my case so
// |                  that I could run the programs on my computer. The max
// |                  number of process that my computer would let me run was
// |                  2. This is why I had to create this file and add this
// |                  extension. As it is now you can run up 10 processes.
// |                  However if you intend to run more than 10 processes then
// |                  you will have to go into the file named "my-hostfile" and
// |                  change the number 10 to the number of desired processes.
// |                  This might just be system dependent so it might just be
// |                  that I'm the only one that needs to run it like this.
// |
// |  +------------------------------------------------------------------------
// |
// |  Description:  [ The main goal of this program is receive two numbers, one
// |                  from each neighbor and then once you have received them
// |                  you add them up to get a local total including your
// |                  number. ]
// |
// |        Input:  [ The input is done through the command line. An example
// |                  looks like this:
// |
// |    -  make
// |    -  mpirun -np 4 --hostfile my-hostfile sndrcv
// |                    - Where 4 is the number of processes you want to run the
// |                      program with. Depending on the number you input you
// |                      will get a different value at the end.]
// |
// |
// |       Output:  [The output of the program depends on the number of process
// |                 you decide to create. But to get an example of an how the
// |                 output should look like if you enter
// |
// |                  - mpirun -np 4 --hostfile my-hostfile sndrcv
// |
// |                                      0:4
// |                                      1:3
// |                                      2:6
// |                                      3:5
// |                 This output will vary depending on the number of process
// |                 you enter in the command line. ]
// |
// |      Process:  [The program’s steps are as follows:
// |                1. You have to do three essential steps so that we can
// |                create an MPI program. You call MPI_Init, MPI_Comm_size,
// |                MPI_Comm_rank.
// |                2. You call the last two MPI methods so that you can get the
// |                essential information so that you know who you need to
// |                communicate to.
// |                3. After this we create if() else if() statements so that we can
// |                see if we are the first or the last process. These two are
// |                usually the most important ones for control purposes in our
// |                case.
// |                4. For each node you have to call the MPI_Sendrecv()
// |                function twice. This is so you can receive both neighbor
// |                processID number.
// |                5. Finally we have a print statement for each of the three
// |                situations where either you are the last process , first
// |                process or the process in between. This print statement is
// |                so that each process can print out the result.
// |
// |   Required Features Not Included:  [ Everything was completed ]
// |
// |   Known Bugs:  [There are no Bugs]
// |  *===================================================================== */

/* NOTES
  - Method call for MPI_Sendrecv()
    - MPI_Sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag,
                   void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status)
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

// *****************************************************************************
// CONSTANTS
#define TAG_ID 0
#define INITIAL_RANK 0

/* NOTES AND ANALYZIS OF PROGRAM
1. Asuming size is greater than 1
    - have the first node alway communicate with the first Node and last node
2. the second node and after only comunicates with the node infront
3. The last node shouldnt do anything.
*/

/*
 A -> B
 ^    B -> C
 |         C -> D
 |______________|

*/

int main(int argc, char* argv[]){
  int processID, numberOfProcesses;
  int data, localdata;
  int rightNeighborDest, leftNeighborDest;
  MPI_Status status;

  int leftNeighbor = -100;
  int rightNeighbor = -100;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &processID);

  const int First_Process = 0;
  const int Last_Process = numberOfProcesses - 1;

  data = processID;

  if (processID == Last_Process){
    // Left Neighbor
    leftNeighborDest = processID - 1;
    MPI_Sendrecv(&data, 1, MPI_INT, leftNeighborDest, 0, &leftNeighbor, 1, MPI_INT, leftNeighborDest, 0, MPI_COMM_WORLD, &status );

    // Right Neighbor
    rightNeighborDest = 0;
    MPI_Sendrecv(&data, 1, MPI_INT, rightNeighborDest, 0, &rightNeighbor, 1, MPI_INT, rightNeighborDest, 0, MPI_COMM_WORLD, &status);
    printf("%d:%d\n", processID, (leftNeighbor + processID + rightNeighbor));

  } else if (processID == First_Process) {
    // Left Neighbor
    leftNeighborDest = numberOfProcesses - 1;
    MPI_Sendrecv(&data, 1, MPI_INT,leftNeighborDest, 0, &leftNeighbor, 1, MPI_INT, leftNeighborDest, 0, MPI_COMM_WORLD, &status );

    // Right Neighbor
    rightNeighborDest = processID + 1;
    MPI_Sendrecv(&data, 1, MPI_INT, rightNeighborDest, 0, &rightNeighbor, 1, MPI_INT, rightNeighborDest, 0, MPI_COMM_WORLD, &status );
    printf("%d:%d\n", processID, (leftNeighbor + processID + rightNeighbor));

  } else{
    // Right Neighbor
    rightNeighborDest = processID + 1;
    MPI_Sendrecv(&data, 1, MPI_INT, rightNeighborDest, 0, &rightNeighbor, 1, MPI_INT, rightNeighborDest, 0, MPI_COMM_WORLD, &status);

    // Left Neighbor
    leftNeighborDest = processID - 1;
    MPI_Sendrecv(&data, 1, MPI_INT, leftNeighborDest, 0, &leftNeighbor, 1, MPI_INT, leftNeighborDest, 0, MPI_COMM_WORLD, &status );
    printf("%d:%d\n", processID, (leftNeighbor + processID + rightNeighbor));
  }
  MPI_Finalize();
  return 0;
}
