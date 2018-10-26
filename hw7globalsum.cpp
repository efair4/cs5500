#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#define MCW MPI_COMM_WORLD

using namespace std;

void print(int data){
	int rank;
	int size;
	MPI_Comm_rank(MCW, &rank); 
	MPI_Comm_size(MCW, &size); 
	int *dArray = new int [size];
	MPI_Gather(&data,1,MPI_INT,dArray,1,MPI_INT,0,MCW);
	if(rank==0){
		for(int i=0;i<size;++i){
	    cout<<dArray[i]<<"---";
	  }
	  cout<<endl;
	}

	return;
}

void cubeSum(int &num, int rank, int partner) {
	int recvNum;	
	MPI_Send(&num, 1, MPI_INT, partner, 0, MCW);
	MPI_Recv(&recvNum, 1, MPI_INT, partner, 0, MCW, MPI_STATUS_IGNORE);
	num+=recvNum;	
}

int main(int argc, char **argv) {
	int rank, size;
	int num, recvNum;
	bool changed = false;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	
	srand(rank + 2);
	num = rand()%50 + 1;	

	print(num);	
//	if(rank == 0) {cout<<"Cube Method"<<endl;}
//	for(int i = 0; i < log2(size); i++) {
//		int partnerRank = rank ^ (1 << i);
//		cubeSum(num, rank,  partnerRank);	
//	}


	if(rank == 0) {cout<<"Ring Method"<<endl;}
		if(rank == 0) {
		MPI_Send(&num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
		changed = true;
	}
	else {
		while(!changed) {
			MPI_Recv(&recvNum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			num+=recvNum;
			changed = true;
			if (rank < size - 1) {
				MPI_Send(&num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
	  	}
		}
	}

	MPI_Barrier(MCW);

	changed = false;
	if (rank == size - 1) {
		MPI_Send(&num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
		changed = true;
	}
	else {
		while(!changed) {
			MPI_Recv(&num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			changed = true;
			if (rank > 0) {
				MPI_Send(&num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
			}
		}
	}
	
	cout<<"Rank: "<<rank<<" -- My num: "<<num<<endl;
	MPI_Finalize();

	return 0;
}
