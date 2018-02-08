// /*==========================================================================
// |   Source code:  [send-recv.c]
// |        Author:  [Cristian C Cepeda]
// |    Student ID:  [4959207]
// |    Assignment:  Program #[1] Problem #[1] [send-recv.c]
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
// |                      - mpicc -O -o send-recv send-recv.c
// |
// |                  - This will create an executable file named
// |                    "send-recv"
// |
// |                  RUN
// |                  - Enter this to Run a test version of it
// |
// |                  - mpirun -np 4 --hostfile my-hostfile send-recv
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
// |  Description:  [ The main goal of this program is to have the processes
// |                  communicate between each other. You can picture
// |                  this program to be a circular array. But we are starting
// |                  from an initial location the root and the last element can
// |                  communicate with the root. From the root we will send a
// |                  number to our neighbor and they will continue sending
// |                  that number to there next neighbor until it reaches the
// |                  root again. Once it reaches the root we will print these
// |                  two numbers out. These two numbers are our solution to the
// |                  program. When we move to the right in the array we are
// |                  adding the results. And when we move towards the left we
// |                  are subtracting the values. We should essentially get the
// |                  same number but one positive and one negative. To compile
// |                  and run the program look at that subsection. ]
// |
// |        Input:  [ The input is done through the command line. An example
// |                  looks like this:
// |
// |    -  make
// |    -  mpirun -np 4 --hostfile my-hostfile ./send-recv
// |                    - Where 4 is the number of processes you want to run the
// |                      program with. Depending on the number you input you
// |                      will get a different value at the end. ]
// |
// |
// |       Output:  [The output of the program depends on the number of process
// |                 you decide to create. If we put 4 then the answers will be
// |                 6 and -6. I would go ahead and insert different values to
// |                 see different outputs.
// |
// |      Process:  [The program’s steps are as follows:
// |                1. You have to do three essential steps so that we can
// |                create an MPI program. You call MPI_Init, MPI_Comm_size,
// |                MPI_Comm_rank.
// |                2. You call the last two MPI methods so that you can get the
// |                essential information so that you know who you need to
// |                communicate to.
// |                3. We then start to determine or say that everyone should be ready
// |                to receive a value. And you do this by calling MPI_Recv();
// |                4. This is as long as its not the first process since that's
// |                the process that needs to send the first number in both
// |                directions.
// |                5. Once everyone starts to receive their number they need to
// |                update their value which this value that is being passed
// |                around. So that it can eventually get back to zero. And the
// |                Zero process(The first process) can print out the number.
// |
// |   Required Features Not Included:  [ Everything was completed ]
// |
// |   Known Bugs:  [There are no Bugs]
// |  *===================================================================== */

/* NOTES
    - Basic point-to-point communication operations are SEND and RECEIVE
    - This is MPI's blocking sending and receiving functions
    - Method call for MPI_SEND()
      - MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
    - Method call for MPI_Recv()
     - MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
*/
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdio.h>

// *************************************************************************
// CONSTANTS
#define TAG_ID 0

int main(int argc, char* argv[])
{
  int numberOfProcesses, processID;
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &processID);

  int valueUpdateADD, valueUpdateSUBTRACT;

  // You want all process but the first process to be out in look out at first.
  // So create an MPI_Recv();
  if (processID != 0){
    MPI_Recv(&valueUpdateADD, 1, MPI_INT, (processID - 1), TAG_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&valueUpdateSUBTRACT, 1, MPI_INT, (processID - 1), TAG_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("The value of valueUpdateADD before being updated is: %d\n", valueUpdateADD);
    // printf("Process: %d  received value: %d from Process: %d\n", processID, valueUpdateADD, (processID-1) );
    valueUpdateADD += processID;
    valueUpdateSUBTRACT -= processID;
    // printf("After Process: %d updated the valueUpdateADD we have: %d\n", processID, valueUpdateADD);
  } else{
    valueUpdateADD = 0;
    valueUpdateSUBTRACT = 0;
  }
  /* We are sending it to (processID +1)%numberOfProcesses because the Last_Process
   when you modulo it should point back to the zero'th index. */
  MPI_Send(&valueUpdateADD, 1, MPI_INT, (processID+1)%numberOfProcesses, TAG_ID, MPI_COMM_WORLD);
  MPI_Send(&valueUpdateSUBTRACT, 1, MPI_INT, (processID+1)%numberOfProcesses, TAG_ID, MPI_COMM_WORLD);

  if(processID == 0){
    MPI_Recv(&valueUpdateADD, 1, MPI_INT, (numberOfProcesses - 1), TAG_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&valueUpdateSUBTRACT, 1, MPI_INT, (numberOfProcesses - 1), TAG_ID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("The value of valueUpdateADD before being updated is: %d\n", valueUpdateADD);
    // printf("Process: %d  received value: %d from Process: %d\n", processID, valueUpdateADD, (numberOfProcesses - 1));
    valueUpdateADD += processID;
    valueUpdateSUBTRACT -= processID;
    printf("[Process: %d] [valueUpdateADD: %d] [valueUpdateSUBTRACT: %d]\n", processID, valueUpdateADD, valueUpdateSUBTRACT);
  }

  MPI_Finalize();
  return 0;
}

/******************************************************************************/
/***************************** END OF PROGRAM *********************************/
/******************************************************************************/
