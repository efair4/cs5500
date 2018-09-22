#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<algorithm>
#include<iterator>
#include<vector>
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv) {
	const int NUM_VALS = 20;
	int rank, size, numSlaves, begin, tag1, tag2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	tag1 = 1;
	tag2 = 2;
	int  numArray[NUM_VALS * size];

	if(rank == 0) {
		for(int i = 0; i < (NUM_VALS * size); i++) {
			numArray[i] = rand()%100 + 1;
		}
	}

	int subArray[NUM_VALS];
	MPI_Scatter(numArray, NUM_VALS, MPI_INT, subArray, NUM_VALS, MPI_INT, 0, MCW);
	vector<int> temp;
	for(int i = 0; i < NUM_VALS; i++) {
		temp.push_back(subArray[i]);
	}
	sort(temp.begin(), temp.end());
	for(int i = 0; i < NUM_VALS; i++) {
		subArray[i] = temp[i];
	}
	int mainArray[NUM_VALS * size];
	MPI_Gather(&subArray, NUM_VALS, MPI_INT, mainArray, NUM_VALS, MPI_INT, 0, MCW);

	if(rank == 0) {
		vector<int> tempMain;
		for(int i = 0; i < NUM_VALS * size; i++) {
			tempMain.push_back(mainArray[i]);
		}
		inplace_merge(tempMain.begin(), tempMain.begin() + NUM_VALS, tempMain.begin() + (2*NUM_VALS));
		inplace_merge(tempMain.begin() + NUM_VALS * 2, tempMain.begin() + NUM_VALS*3, tempMain.end());
		inplace_merge(tempMain.begin(), tempMain.begin() + NUM_VALS * 2, tempMain.end());
		for(int i = 0; i < NUM_VALS * size; i++) {
			cout<<i<<" : "<<tempMain[i]<<endl;
		}
	}

	MPI_Finalize();

	return 0;
}
