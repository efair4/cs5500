#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<algorithm>
#define MCW MPI_COMM_WORLD
using namespace std;

int main(int argc, char **argv) {
	const int NUM_VALS = 20;
	int rank, size;
	int mainArray[];
	int partialArray[];
	int numSlaves;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	numSlaves = size - 1;
	int numArray[NUM_VALS * size);

	if(rank == 0) {
		for(int i = 0; i<numArray.size(); i++) {
			numArray[i] = rand()%100 + 1;
		}
		int begin = 0;
		for(int i = 1; i < size; i++) {
			MPI_Send(&numArray[begin], NUM_VALUES, MPI_INT, i, 0, MCW);
			begin += NUM_VALUES;
		}
		while(numSlaves > 0) {
			MPI_Recv(&partialArray, NUM_VALUES, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
			numSlaves--;
		}
	} 
	else {
		MPI_Recv(&numArray, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
		partialArray = sort(numArray.begin(), numArray.end());
		MPI_Send(&partialArray, NUM_VALS, MPI_INT, 0, 0, 0);
	}



	MPI_Finalize();

	return 0;
}
