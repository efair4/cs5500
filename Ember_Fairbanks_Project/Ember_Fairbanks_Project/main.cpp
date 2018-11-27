#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<vector>
#include <algorithm>
#include <random>

#define MCW MPI_COMM_WORLD
#define white 0
#define blue 1
#define red 2
#define green 3
#define orange 4
#define yellow 5
#define CUBESIZE 48

using namespace std;

void printColor(int color) {
	switch (color) {
	case 0: cout << "white" << endl; break;
	case 1: cout << "blue" << endl; break;
	case 2: cout << "red" << endl; break;
	case 3: cout << "green" << endl; break;
	case 4: cout << "orange" << endl; break;
	case 5: cout << "yellow" << endl; break;
	}
}

void setupCube(vector<vector<int>> &cube) {
	int currentColor = white;
	int startIndex = 0;
	while (currentColor <= yellow) {
		cout << startIndex << endl;
		printColor(currentColor);
		for (int i = 0; i < 8; i++) {
			cube[currentColor].push_back(i);
		}
		startIndex = startIndex + 8;
		currentColor++;
	}
}

void r(vector<vector<int>> &cube);
void ri(vector<vector<int>> &cube);
void l(vector<vector<int>> &cube);
void li(vector<vector<int>> &cube);
void u(vector<vector<int>> &cube);
void ui(vector<vector<int>> &cube);
void d(vector<vector<int>> &cube);
void di(vector<vector<int>> &cube);
void f(vector<vector<int>> &cube);
void fi(vector<vector<int>> &cube);
void b(vector<vector<int>> &cube);
void bi(vector<vector<int>> &cube);

int main(int argc, char **argv) {
	int rank, size;
	vector<vector<int>> startCube(6);
	setupCube(startCube);
	vector<vector<int>> mixedCube = startCube;
	//auto rng = std::default_random_engine{};
	//shuffle(begin(mixedCube), end(mixedCube), rng);
	r(mixedCube);
	int count = 1;
	while (startCube != mixedCube) {
		r(mixedCube);
		count++;
	}
	cout << endl << count << endl;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);


	MPI_Finalize();
	return 0;
}


void r(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void ri(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[blue][2]; //white side
	tempCube[white][4] = cube[blue][4];
	tempCube[white][7] = cube[blue][7];
	tempCube[blue][2] = cube[yellow][2]; //blue side
	tempCube[blue][4] = cube[yellow][4];
	tempCube[blue][7] = cube[yellow][7];
	tempCube[green][0] = cube[white][7]; //green side
	tempCube[green][3] = cube[white][4];
	tempCube[green][5] = cube[white][2];
	tempCube[yellow][2] = cube[green][7]; //yellow side
	tempCube[yellow][4] = cube[green][4];
	tempCube[yellow][7] = cube[green][2];
	cube = tempCube;
}

void l(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void li(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void u(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void ui(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void d(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void di(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void f(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void fi(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void b(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}

void bi(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][2] = cube[green][5]; //white side
	tempCube[white][4] = cube[green][3];
	tempCube[white][7] = cube[green][0];
	tempCube[blue][2] = cube[white][2]; //blue side
	tempCube[blue][4] = cube[white][4];
	tempCube[blue][7] = cube[white][7];
	tempCube[green][0] = cube[yellow][7]; //green side
	tempCube[green][3] = cube[yellow][4];
	tempCube[green][5] = cube[yellow][2];
	tempCube[yellow][2] = cube[blue][2]; //yellow side
	tempCube[yellow][4] = cube[blue][4];
	tempCube[yellow][7] = cube[blue][7];
	cube = tempCube;
}
