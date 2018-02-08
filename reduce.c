// /*==========================================================================
// |   Source code:  [reduce.c]
// |        Author:  [Cristian C Cepeda]
// |    Student ID:  [4959207]
// |    Assignment:  Program #[1] Problem #[3] [reduce.c]
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
// |                      - mpicc -O -o reduce reduce.c
// |
// |                  - This will create an executable file named
// |                    "reduce"
// |
// |                  RUN
// |                  - Enter this to Run a test version of it
// |
// |                  - mpirun -np 4 --hostfile my-hostfile reduce
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
// |  Description:  [ The main goal of this program is simulate the same
// |                  objective as we had to do for Problem Number 1. However we
// |                  were able to tackle this problem with a simple method that
// |                  MPI provides us with. This method is MPI_Reduce. The key
// |                  here to understand is that this method provides us with a
// |                  multitude of reduce operations such as the one we used or
// |                  MPI_MAX or MPI_MIN to name some others. It essentially
// |                  pairs the nodes up and performs the desired operation. And
// |                  the root node is the one that should receive the final
// |                  response after calling the MPI_Reduce() function. ]
// |
// |        Input:  [ The input is done through the command line. An example
// |                  looks like this:
// |
// |    -  make
// |    -  mpirun -np 4 --hostfile my-hostfile reduce
// |                    - Where 4 is the number of processes you want to run the
// |                      program with. Depending on the number you input you
// |                      will get a different value at the end. In this case
// |                      you are also creating these processes as nodes so as
// |                      you put more the number will be higher because we are
// |                      adding the results of their processID ]
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
// |                3. After this we create a variable with the negative form of
// |                their processID because there is no MPI_Subtract or reduction
// |                operation to subtract.
// |                4. We then call the MPI_Reduce twice one so that it can
// |                essentially go to the right adding all the numbers. and one
// |                so that it can go to the left adding all the negative numbers
// |                to get the same result as we did in problem number 1.
// |                5. Finally we create an if statement where only the
// |                parent or root has access to. It's here were it calls the
// |                variable where MPI_Reduce was adding all the numbers to, so
// |                that it can print the number out.
// |
// |   Required Features Not Included:  [ Everything was completed ]
// |
// |   Known Bugs:  [There are no Bugs]
// |  *===================================================================== */

/* NOTES
  - Method call for MPI_Reduce()
    - // MPI_Reduce(void* send_data, void* recv_data, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm communicator)
  - Good example that shows how to use MPI_Reduce when a process has more than
    one number.
    - https://github.com/wesleykendall/mpitutorial/blob/gh-pages/tutorials/mpi-reduce-and-allreduce/code/reduce_avg.c
*/

#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdio.h>

// *************************************************************************
// CONSTANTS
#define ROOT 0

//******************************************************************************
int main(int argc, char* argv[]) {
  int numberOfProcesses, processID;
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD, &processID);

  int valueUpdateADD, valueUpdateSUBTRACT;
  // Number each process is sending
  int positiveRank = processID;
  int negativeRank = processID*(-1);

  MPI_Reduce(&positiveRank, &valueUpdateADD, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
  MPI_Reduce(&negativeRank, &valueUpdateSUBTRACT, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
  if (processID == ROOT){
    printf("[Process: %d] [valueUpdateADD: %d] [valueUpdateSUBTRACT: %d]\n", processID, valueUpdateADD, valueUpdateSUBTRACT);
  }
  MPI_Finalize();
  return 0;
}
