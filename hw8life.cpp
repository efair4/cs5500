#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<fstream>
#include<ctime>
#define MCW MPI_COMM_WORLD
#define N 24
#define I 67

using namespace std;

int main(int argc, char **argv) {
	int rank, size, num;
	int world[N][N];
	ofstream outfile("world.txt");
//	srand(time(NULL));
//	for(int i = 0; i < N; i++) {
//		for(int j = 0; j < N; j++) {
//			num = rand() % 5;
//			if(num == 1) {world[i][j] = 1;}
//			else {world[i][j] = 0;}
//		}
//	}
	world[0][0] = 1; 
	world[0][2] = 1;
	world[1][1] = 1;
	world[1][2] = 1;
	world[2][1] = 1; 
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(world[i][j] == 0) {outfile << "o";}
			else {outfile << "x";}
		}
		outfile << endl;
	}
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	int partSize = N/size;
	int myWorld[partSize][N];
	if(rank == 0) {
		for(int dest = 0; dest < size; dest++) {
			for(int row = 0; row < partSize; row++) {
				for(int col = 0; col < N; col++) {
					myWorld[row][col] = world[row+(dest*partSize)][col];				}
			}
			MPI_Send(&myWorld, N*partSize, MPI_INT, dest, 0, MCW);
		}
	}
	
	MPI_Recv(&myWorld, N*partSize, MPI_INT, 0, 0, MCW, MPI_STATUS_IGNORE);
	int sendBelow[N];
	int fromAbove[N];
	int sendAbove[N];
	int fromBelow[N];

	for(int i = 0; i < I; i++) {
		if(rank < size-1) {
			for(int col = 0; col < N; col++) {
				sendBelow[col] = myWorld[partSize-1][col];
			}
			MPI_Send(&sendBelow, N, MPI_INT, rank+1, 1, MCW);
		}
		if(rank > 0) {
			MPI_Recv(&fromAbove, N, MPI_INT, rank-1, 1, MCW, MPI_STATUS_IGNORE);
		}
		else {
			for(int col = 0; col < N; col++) {
				fromAbove[col] = 0;
			}
		}
		if(rank > 0) {
			for(int col = 0; col < N; col++) {
				sendAbove[col] = myWorld[0][col];
			}
			MPI_Send(&sendAbove, N, MPI_INT, rank-1, 1, MCW);
		}
		if(rank < size-1) {
			MPI_Recv(&fromBelow, N, MPI_INT, rank+1, 1, MCW, MPI_STATUS_IGNORE);
		}
		else {
			for(int col = 0; col < N; col++) {
				fromBelow[col] = 0;
			}
		}
		int sum = 0;
		int tempWorld[partSize][N];
		for(int x = 0; x < partSize; x++) {
			for(int y = 0; y < N; y++) {
				if(x==0 && y==0) { //top left
					sum = myWorld[x+1][y]+myWorld[x+1][y+1]+myWorld[x][y+1]+fromAbove[0]+fromAbove[1];
				}
				else if(x==0 && y==N-1) { //top right
					sum = myWorld[x][y-1]+myWorld[x+1][y-1]+myWorld[x+1][y]+fromAbove[N-1]+fromAbove[N-2];;
				}
				else if(x==N-1 && y ==0) { //bottom left
					sum = myWorld[x-1][y]+myWorld[x-1][y+1]+myWorld[x][y+1]+fromBelow[0]+fromBelow[1];
				}
				else if(x==N-1 && y==N-1) { //bottom right
					sum = myWorld[x][y-1]+myWorld[x-1][y-1]+myWorld[x-1][y]+fromBelow[N-1]+fromBelow[N-2];
				}
				else {
					if(y==0) { //left column
						sum = myWorld[x-1][y]+myWorld[x-1][y+1]+myWorld[x][y+1]+myWorld[x+1][y+1]+myWorld[x+1][y];
					}
					else if(x==0) { //top row
						sum = myWorld[x][y-1]+myWorld[x+1][y-1]+myWorld[x+1][y]+myWorld[x+1][y+1]+myWorld[x][y+1]
							+fromAbove[y-1]+fromAbove[y]+fromAbove[y+1];
					}
					else if(y==N-1) { //right column
						sum = myWorld[x-1][y]+myWorld[x-1][y-1]+myWorld[x][y-1]+myWorld[x+1][y-1]+myWorld[x+1][y];
					}
					else if(x==N-1) { //bottom row
						sum = myWorld[x][y-1]+myWorld[x-1][y-1]+myWorld[x-1][y]+myWorld[x-1][y+1]+myWorld[x][y+1]
							+fromBelow[y-1]+fromBelow[y]+fromBelow[y+1];
					}
					else { //any other space
						sum = myWorld[x][y-1]+myWorld[x-1][y-1]+myWorld[x-1][y]+myWorld[x-1][y+1]+myWorld[x][y+1]
							+myWorld[x+1][y+1]+myWorld[x+1][y]+myWorld[x+1][y-1];
					}
				}
				if(myWorld[x][y]==1 && (sum==0 || sum==1 || sum>=4)) {tempWorld[x][y] = 0;}
				else if(myWorld[x][y]==1 && (sum==2 || sum==3)) {tempWorld[x][y] = 1;}
				else if(myWorld[x][y]==0 && sum==3) {tempWorld[x][y] = 1;} 
				else {tempWorld[x][y] = 0;}
			}
		}
		for(int row = 0; row < partSize; row++) {
			for(int col = 0; col < N; col++) {
				myWorld[row][col] = tempWorld[row][col];
			}
		}
		if(rank == 0) {
			int oneBoard[partSize][N];
			outfile << "After "<<i+1<<" Iterations" << endl;
			for(int row = 0; row < partSize; row++) {
				for(int col = 0; col < N; col++) {
					if(myWorld[row][col] == 0) {outfile << "o";}
					else {outfile << "X";}
				}
				outfile << endl;
			}
			for(int src = 1; src < size; src++) {
				MPI_Recv(&oneBoard, N*partSize, MPI_INT, src, 2, MCW, MPI_STATUS_IGNORE);
				for(int row = 0; row < partSize; row++) {
					for(int col = 0; col < N; col++) {
						if(oneBoard[row][col] == 0) {outfile << "o";}
						else {outfile << "X";}
					}
					outfile << endl;
				}
			}
		}
		else {
			MPI_Send(&myWorld, N*partSize, MPI_INT, 0, 2, MCW);
		}
	}
	
	outfile.close();
	MPI_Finalize();
	return 0;
}
