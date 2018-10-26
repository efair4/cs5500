#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<fstream>
#define MPI MPI_COMM_WORLD
#define N 24
#define I 40

using namespace std;

int main(int argc, char **argv) {
	int rank, size;
	int world[N][N];
	ofstream outfile("world.txt");

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			world[i][j] = 0;
		}
	}
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

	for(int i = 0; i < I; i++) {
		int sum = 0;
		int tempWorld[N][N];
		for(int x = 0; x < N; x++) {
			for(int y = 0; y < N; y++) {
				if(x==0 && y==0) { //top left
					sum = world[x+1][y]+world[x+1][y+1]+world[x][y+1];
				}
				else if(x==0 && y==N-1) { //top right
					sum = world[x][y-1]+world[x+1][y-1]+world[x+1][y];
				}
				else if(x==N-1 && y ==0) { //bottom left
					sum = world[x-1][y]+world[x-1][y+1]+world[x][y+1];
				}
				else if(x==N-1 && y==N-1) { //bottom right
					sum = world[x][y-1]+world[x-1][y-1]+world[x-1][y];
				}
				else {
					if(y==0) { //left column
						sum = world[x-1][y]+world[x-1][y+1]+world[x][y+1]+world[x+1][y+1]+world[x+1][y];
					}
					else if(x==0) { //top row
						sum = world[x][y-1]+world[x+1][y-1]+world[x+1][y]+world[x+1][y+1]+world[x][y+1];
					}
					else if(y==N-1) { //right column
						sum = world[x-1][y]+world[x-1][y-1]+world[x][y-1]+world[x+1][y-1]+world[x+1][y];
					}
					else if(x==N-1) { //bottom row
						sum = world[x][y-1]+world[x-1][y-1]+world[x-1][y]+world[x-1][y+1]+world[x][y+1];
					}
					else { //any other space
						sum = world[x][y-1]+world[x-1][y-1]+world[x-1][y]+world[x-1][y+1]+world[x][y+1]
							+world[x+1][y+1]+world[x+1][y]+world[x+1][y-1];
					}
				}
				if(world[x][y]==1 && (sum==0 || sum==1 || sum>=4)) {tempWorld[x][y] = 0;}
				else if(world[x][y]==1 && (sum==2 || sum==3)) {tempWorld[x][y] = 1;}
				else if(world[x][y]==0 && sum==3) {tempWorld[x][y] = 1;} 
				else {tempWorld[x][y] = 0;}
			}
		}
		for(int row = 0; row < N; row++) {
			for(int col = 0; col < N; col++) {
				world[row][col] = tempWorld[row][col];
			}
		}
		outfile << "After "<<i<<" Iterations" << endl;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				if(world[i][j] == 0) {outfile << "o";}
				else {outfile << "X";}
			}
			outfile << endl;
		}
	}


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI, &rank);
	MPI_Comm_size(MPI, &size);

	

	MPI_Finalize();
	return 0;
}
