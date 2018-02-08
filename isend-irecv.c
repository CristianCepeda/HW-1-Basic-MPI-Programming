// /*==========================================================================
// |   Source code:  [isend-irecv.c]
// |        Author:  [Cristian C Cepeda]
// |    Student ID:  [4959207]
// |    Assignment:  Program #[1] Problem #[2] [isend-irecv.c]
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
// |                      - mpicc -O -o isend-irecv isend-irecv.c
// |
// |                  - This will create an executable file named
// |                    "isend-irecv"
// |
// |                  RUN
// |                  - Enter this to Run a test version of it
// |
// |                  - mpirun -np 4 --hostfile my-hostfile isend-irecv
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
// |                  root again. Once it reaches the root we will print this
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
// |    -  mpirun -np 4 --hostfile my-hostfile ./isend-irecv
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
// |                3. We then determine if you're the first process. This is
// |                because the first process needs to send its initial value to
// |                both its neighbors. Before anyone else decides to send a
// |                value. The first process also starts to receive but it waits
// |                until it gets its value before adding it to the corresponding
// |                counter.
// |                4. As for the rest of the processes they will all have to
// |                be out in the look out or receiving a value before they
// |                decide to send a value to their neighbor. When they call the
// |                receive function they also have to wait. Once they get
// |                the value from there neighbor they perform the addition or
// |                subtraction and then they send that value to their next
// |                neighbor.
// |                5. Its now that everyone has received and sent a number that
// |                the first process needs to print out the total value.
// |
// |   Required Features Not Included:  [ Everything was completed ]
// |
// |   Known Bugs:  [There are no Bugs]
// |  *===================================================================== */

/* NOTES
    - Basic point-to-point communication operations are SEND and RECEIVE
    - This is MPI's Nonblocking sending and receiving function
    - Method call for MPI_SEND()
      - MPI_Isend(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator, MPI_Request *request)
    - Method call for MPI_Recv()
     - MPI_Irecv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Request *request)
    - Method call for MPI_Wait()
     - MPI_Wait(MPI_Request *request, MPI_Status *status)
     - MPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses)

    - MPI_WAIT returns when the operation identified by request is complete.
    - Message-Passing model implies that communication is initiated by the
    sender. The communication will generally have a lower overhead if a receive
    is already posted when the sender initiates the communication (data can be
    moved directly to the receive buffer, and there is no need to queue a pending
    send request). However, a receive operation can complete only after the
    matching send has occurred. The use of nonblocking receives allows one to
    achieve lower communication overheads without blocking the receiver while it
    waits for the send.

    - Good Websites on MPI
          - http://mpitutorial.com/tutorials/mpi-send-and-receive/
          - http://mpi-forum.org/docs/mpi-1.1/mpi-11-html/node28.html#Node28
          - Read about buffered/synchronous send
            - http://mpi-forum.org/docs/mpi-1.1/mpi-11-html/node40.html#Node40
          - // http://www.bu.edu/tech/support/research/training-consulting/online-tutorials/mpi/example1-2/example1_3/
          - // https://www.yumpu.com/en/document/view/36471665/dr-david-cronk-innovative-computing-lab-university-of-it-works/63
*/

#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdio.h>

// *************************************************************************
// CONSTANTS
#define TAG_ID 0

int main(int argc, char* argv[]) {

  int numberOfProcesses, processID;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &processID);

  MPI_Request request;
  MPI_Status status;


  const int First_Process = 0;
  const int Last_Process = numberOfProcesses - 1;
  int leftNeighbor = (processID == First_Process)? Last_Process: processID - 1;
  int rightNeighbor = (processID == Last_Process)? First_Process : processID + 1;

  int sendingDataPositive = processID;
  int sendingDataNegative = processID;
  int receivingDataPositive;
  int receivingDataNegative;

  int valueUpdateADD = 0;
  int valueUpdateSUBTRACT = 0;

  if (processID == First_Process) {
    // TAKING CARE OF SENDING RIGHT(FOWARD)
    MPI_Isend(&sendingDataPositive, 1, MPI_INT, rightNeighbor, TAG_ID, MPI_COMM_WORLD, &request);
    // TAKING CARE OF SENDING LEFT(BACKWARDS)
    MPI_Isend(&sendingDataNegative, 1, MPI_INT, leftNeighbor, TAG_ID, MPI_COMM_WORLD, &request);

    // RECEIVING FROM LEFT NEIGHBOR SO POSITIVE VALUE
    MPI_Irecv(&receivingDataPositive, 1, MPI_INT, leftNeighbor, TAG_ID, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    // RECEIVING FROM RIGHT NEIGHBOR SO NEGATIVE VALUE
    MPI_Irecv(&receivingDataNegative, 1, MPI_INT, rightNeighbor, TAG_ID, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    valueUpdateADD += receivingDataPositive;
    valueUpdateSUBTRACT += receivingDataNegative;

  } else {
    // WE ARE SENDING TO THE RIGHT BUT WE NEED TO RECEIVE FROM OUR LEFT NEIGHBOR
    MPI_Irecv(&receivingDataPositive, 1, MPI_INT, leftNeighbor, TAG_ID, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    valueUpdateADD += receivingDataPositive + processID;
    MPI_Isend(&valueUpdateADD, 1, MPI_INT, rightNeighbor, TAG_ID, MPI_COMM_WORLD, &request);

    // WE ARE SENDING TO THE LEFT BUT WE NEED TO RECEIVE FROM OUR RIGHT NEIGHBOR
    MPI_Irecv(&receivingDataNegative, 1, MPI_INT, rightNeighbor, TAG_ID, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    valueUpdateSUBTRACT += receivingDataNegative - processID;
    MPI_Isend(&valueUpdateSUBTRACT, 1, MPI_INT, leftNeighbor, TAG_ID, MPI_COMM_WORLD, &request);
  }

  if (processID == First_Process) {
    printf("[Process: %d] [valueUpdateADD: %d] [valueUpdateSUBTRACT: %d]\n", processID, valueUpdateADD, valueUpdateSUBTRACT);
  }


  MPI_Finalize();
  return 0;
}
