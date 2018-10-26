#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<fstream>
#define MPI MPI_COMM_WORLD
#define N 24
#define I 20

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
		for(int x = 0; x < N; x++) {
			for(int y = 0; y < N; y++) {
				if(x == 0 && y == 0) { //top left
					sum = world[x+1][y]+world[x+1][y+1]+world[x][y+1];
				}
				else if(x == 0 && y == N-1) { //top right
					sum = world[x][y-1]+world[x+1][y-1]+world[x+1][y];
				}
				else if(x == N-1 && y == 0) { //bottom left
					sum = world[x-1][y]+world[x-1][y+1]+world[x][y+1];
				}
				else if(x == N-1 && y == N-1) { //bottom right
					sum = world[x][y-1]+world[x-1][y-1]+world[x-1][y];
				}
				else {
					if(y == 0) {
						sum = world[x-1][y]+world[x-1][y+1]+world[x][y+1]+world[x+1][y+1]+world[x+1][y];
					}
					else if(x == 0) {
					

	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI, &rank);
	MPI_Comm_size(MPI, &size);

	

	MPI_Finalize();
	return 0;
}
