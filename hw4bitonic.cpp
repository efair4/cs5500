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

void compare(int &num, int rank, int partner, int order) {
	int recvNum;	
	MPI_Send(&num, 1, MPI_INT, partner, 0, MCW);
	MPI_Recv(&recvNum, 1, MPI_INT, partner, 0, MCW, MPI_STATUS_IGNORE);
	
  if(num > recvNum && order) {
		if(rank < partner)
			num = recvNum;
	}
	else if(num < recvNum && order) {
		if(rank > partner) 
			num = recvNum;
	}
	else if(num < recvNum && !order) {
		if(rank > partner)
			num = recvNum;
	}
	else if(num > recvNum && !order) {
		if(rank < partner)
			num = recvNum;
	}
}

int main(int argc, char **argv) {
	int rank, size;
	int num;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	
	srand(rank + 2);
	num = rand()%50 + 1;	

	print(num);	
	for(int i = 0; i < log2(size); i++) {
		for(int j = i; j >= 0; j--) {
			if(rank == 0) cout<<"Cube"<<j<<endl;
			int partnerRank = rank ^ (1 << j);
			if ((( rank >> (i + 1)) & 1  ) == (  (rank >> j) & 1) )
				compare(num, rank,  partnerRank, 1); //1 is ascending
			else 
				compare(num, rank, partnerRank, 0); //0 is descending
			print(num);	
		}
	}

	MPI_Finalize();

	return 0;
}
