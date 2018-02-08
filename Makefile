output: send-recv.o isend-irecv.o reduce.o sndrcv.o
	mpicc -O send-recv.o -o send-recv
	mpicc -O isend-irecv.o -o isend-irecv
	mpicc -O reduce.o -o reduce
	mpicc -O sndrcv.o -o sndrcv

send-recv.o: send-recv.c
	mpicc -c send-recv.c -o send-recv.o

isend-irecv.o: isend-irecv.c
	mpicc -c isend-irecv.c -o isend-irecv.o

reduce.o: reduce.c
	mpicc -c reduce.c -o reduce.o

sndrcv.o: sndrcv.c
	mpicc -c sndrcv.c -o sndrcv.o

clean:
	rm *.o *~ send-recv isend-irecv reduce sndrcv
