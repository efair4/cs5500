#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>
#define MCW MPI_COMM_WORLD

using namespace std;

vector<int>  merge(vector<int> mainArray,vector<int> numArray, int begin, int numVals) {
	vector<int> tempArray;
	int index1 = 0;
	int index2 = begin;
	cout<<"In merge func"<<endl;

	while(tempArray.size() < (mainArray.size() + numArray.size())) {
		if(index1 < mainArray.size() && index2 < begin + numVals) {
			if(mainArray[index1] <= numArray[index2]) {
				tempArray.push_back(mainArray[index1]);
				index1++;
			}
			else {
				tempArray.push_back(numArray[index2]);
				index2++;
			}
		}
		else if(index1 < mainArray.size()) {
			for(int i = index1; i < mainArray.size(); i++) {
				tempArray.push_back(mainArray[i]);
			}
		}
		else {
			for(int i = index2; i < numVals; i++) {
				tempArray.push_back(numArray[i]);
			}
		}
	}
	cout<<"Exiting merge func"<<endl;
	return tempArray;
}

int main(int argc, char **argv) {
	const int NUM_VALS = 20;
	vector<int> mainArray;
	vector<int> numArray;
	int rank, size, numSlaves, begin, tag1, tag2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	numSlaves = size - 1;
	tag1 = 1;
	tag2 = 2;

	if(rank == 0) {
		for(int i = 0; i < (NUM_VALS * numSlaves); i++) {
			numArray.push_back(rand()%100 + 1);
		}
		begin = 0;
		for(int i = 1; i < size; i++) {
			MPI_Send(&begin, 1, MPI_INT, i, tag1, MCW);
			MPI_Send(&numArray[begin], NUM_VALS, MPI_INT, i, tag2, MCW);
			cout<<"After sending array"<<endl;
			begin += NUM_VALS;
		}
		for(int j = 1; j <= numSlaves; j++) {
			MPI_Recv(&begin, 1, MPI_INT, j, tag1, MCW, MPI_STATUS_IGNORE); 
			MPI_Recv(&numArray[begin], NUM_VALS, MPI_INT, j, tag2, MCW, MPI_STATUS_IGNORE);
			mainArray = merge(mainArray, numArray, begin, NUM_VALS);
		}
		for(int i = 0; i < mainArray.size(); i++) {
			cout<<mainArray[i]<<endl;
		}
	} 
	else {
		MPI_Recv(&begin, 1, MPI_INT, 0, tag1, MCW, MPI_STATUS_IGNORE);
		MPI_Recv(&numArray[begin], NUM_VALS, MPI_INT, 0, tag2, MCW, MPI_STATUS_IGNORE);
		cout<<"Received unsorted!"<<endl;
		for(int i = begin; i < begin + NUM_VALS; i++) {
			cout<<numArray[i]<<endl;
		}
		sort(numArray.begin() + begin, numArray.begin() + begin + NUM_VALS);
		cout<<"Sending sorted array"<<endl;
		MPI_Send(&begin, 1, MPI_INT, 0, tag1, MCW);
		MPI_Send(&numArray[begin], NUM_VALS, MPI_INT, 0, tag2, MCW);
		cout<<"Sent sorted array"<<endl;
	}

	MPI_Finalize();

	return 0;
}
