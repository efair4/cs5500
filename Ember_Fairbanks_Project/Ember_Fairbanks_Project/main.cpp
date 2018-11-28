#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<random>
#include<ctime>
#include<numeric>

#define MCW MPI_COMM_WORLD
#define white 0
#define blue 1
#define red 2
#define green 3
#define orange 4
#define yellow 5
#define CUBESIZE 48
#define NUMALGOS 8

using namespace std;

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
void rotateSide(vector<vector<int>> &cube, int side);
void rotateSideCounter(vector<vector<int>> &cube, int side);

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

void printSol(int sol[NUMALGOS]) {
	for (int i = 0; i < NUMALGOS; i++) {
		switch (sol[i]) {
		case 0: cout << "r, "; break;
		case 1: cout << "l, "; break;
		case 2: cout << "u, "; break;
		case 3: cout << "d, "; break;
		case 4: cout << "f, "; break;
		case 5: cout << "b, "; break;
		case 6: cout << "ri, "; break;
		case 7: cout << "li, "; break;
		//case 2: cout << "l" << endl; break;
		//case 3: cout << "li" << endl; break;
		//case 4: cout << "u" << endl; break;
		//case 5: cout << "ui" << endl; break;
		//case 6: cout << "d" << endl; break;
		//case 7: cout << "di" << endl; break;
		//case 8: cout << "f" << endl; break;
		//case 9: cout << "fi" << endl; break;
		//case 10: cout << "b" << endl; break;
		//case 11: cout << "bi" << endl; break;
		}
	}
	cout << endl;
}

void doAlgo(vector<vector<int>> &cube, int algoNum) {
	switch (algoNum) {
	case 0: r(cube); break;
	case 1: l(cube); break;
	case 2: u(cube); break;
	case 3: d(cube); break;
	case 4: f(cube); break;
	case 5: b(cube); break;
	case 6: ri(cube); break;
	case 7: li(cube); break;
	//case 8: ui(cube); break;
	//case 9: di(cube); break;
	//case 10: fi(cube); break;
	//case 11: bi(cube); break;
	}
}

void setupCube(vector<vector<int>> &cube) {
	int currentColor = white;
	int startIndex = 0;
	while (currentColor <= yellow) {
		for (int i = startIndex; i < startIndex + 8; i++) {
			cube[currentColor].push_back(i);
		}
		startIndex = startIndex + 8;
		currentColor++;
	}
}

void mixCube(vector<vector<int>> &cube) {
	int numArray[NUMALGOS] = { 0,1,2,3,4,5,6,7 };// , 8, 9, 10, 11
	auto eng = default_random_engine(time(NULL));
	shuffle(begin(numArray), end(numArray), eng);
	printSol(numArray);
	for (int i = 0; i < NUMALGOS; i++) {
		doAlgo(cube, numArray[i]);
	}
	cout << endl;
}

int doPermutations(vector<vector<int>> startCube, vector<vector<int>> mixedCube, int startNum, vector<int> &list) {
	int cnt = 1;
	do {
		vector<vector<int>> copy = startCube;
		doAlgo(copy, startNum);
		for (int i = 0; i < NUMALGOS - 1; i++) {
			doAlgo(copy, list[i]);
		}
		if (copy == mixedCube) {
			cout << "Start Number = " << startNum << ", Permutation " << cnt << endl;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 8; j++) {
					cout << mixedCube[i][j] << ", ";
				}
			}
			cout << endl;
			list.insert(list.begin(), startNum);
			return 1;
		}
		cnt++;
	} while (next_permutation(list.begin(), list.end()));
	return 0;
}


int main(int argc, char **argv) {
	int rank, size;
	vector<vector<int>> startCube(6);
	setupCube(startCube);
	vector<vector<int>> mixedCube = startCube;
	mixCube(mixedCube);
	MPI_Status status;
	MPI_Request request;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	if (rank == 0) {
		cout << "Shuffled Cube" << endl;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 8; j++) {
				cout << mixedCube[i][j] << ", ";
			}
		}
		cout << endl;

		int currentAlgo = 0;
		for (int i = 1; i < size; i++) { //Send out initial start algorithm
			MPI_Send(&currentAlgo, 1, MPI_INT, i, 0, MCW);
			currentAlgo++;
		}
		int solFound = 0;
		int solution[NUMALGOS];
		while (!solFound) {
			MPI_Recv(&solFound, 1, MPI_INT, MPI_ANY_SOURCE, 1, MCW, &status);
			if (solFound) {
				MPI_Recv(&solution[0], NUMALGOS, MPI_INT, status.MPI_SOURCE, 2, MCW, MPI_STATUS_IGNORE);
				printSol(solution);
				//MPI_Bcast(&solFound, 1, MPI_INT, 0, MCW);
				for (int i = 1; i < size; i++) { //Send out stop signal
					int stopSignal = -1;
					MPI_Send(&stopSignal, 1, MPI_INT, i, 0, MCW);
				}
			}
			else if(currentAlgo < NUMALGOS) {
				MPI_Send(&currentAlgo, 1, MPI_INT, status.MPI_SOURCE, 0, MCW);
				currentAlgo++;
			}
		}
	}
	else {
		int solFound = 0;
		int startNum;
		while (1) {
			vector<int> numList(NUMALGOS);
			iota(numList.begin(), numList.end(), 0);
			MPI_Recv(&startNum, 1, MPI_INT, 0, 0, MCW, MPI_STATUS_IGNORE);
			if (startNum == -1) {
				break;
			}

			numList.erase(remove(numList.begin(), numList.end(), startNum), numList.end());
			solFound = doPermutations(startCube, mixedCube, startNum, numList);
			MPI_Send(&solFound, 1, MPI_INT, 0, 1, MCW);
			if (solFound) {
				cout << "Rank " << rank << " found a solution!" << endl;
				MPI_Send(&numList[0], NUMALGOS, MPI_INT, 0, 2, MCW);
			}
			else {
				cout << "Rank " << rank << " tried with start=" << startNum << endl;
			}
			//MPI_Ibcast(&solFound, 1, MPI_INT, 0, MCW, &request);
		}
	}

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

	rotateSide(tempCube, red);
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
	tempCube[yellow][2] = cube[green][5]; //yellow side
	tempCube[yellow][4] = cube[green][3];
	tempCube[yellow][7] = cube[green][0];

	rotateSideCounter(tempCube, red);
	cube = tempCube;
}

void l(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][0] = cube[blue][0]; //white side
	tempCube[white][3] = cube[blue][3];
	tempCube[white][5] = cube[blue][5];
	tempCube[blue][0] = cube[yellow][0]; //blue side
	tempCube[blue][3] = cube[yellow][3];
	tempCube[blue][5] = cube[yellow][5];
	tempCube[green][2] = cube[white][5]; //green side
	tempCube[green][4] = cube[white][3];
	tempCube[green][7] = cube[white][0];
	tempCube[yellow][0] = cube[green][7]; //yellow side
	tempCube[yellow][3] = cube[green][4];
	tempCube[yellow][5] = cube[green][2];

	rotateSide(tempCube, orange);
	cube = tempCube;
}

void li(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][0] = cube[green][7]; //white side
	tempCube[white][3] = cube[green][4];
	tempCube[white][5] = cube[green][2];
	tempCube[blue][0] = cube[white][0]; //blue side
	tempCube[blue][3] = cube[white][3];
	tempCube[blue][5] = cube[white][5];
	tempCube[green][2] = cube[yellow][5]; //green side
	tempCube[green][4] = cube[yellow][3];
	tempCube[green][7] = cube[yellow][0];
	tempCube[yellow][0] = cube[blue][0]; //yellow side
	tempCube[yellow][3] = cube[blue][3];
	tempCube[yellow][5] = cube[blue][5];

	rotateSideCounter(tempCube, orange);
	cube = tempCube;
}

void u(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[blue][0] = cube[red][0]; //blue side
	tempCube[blue][1] = cube[red][1];
	tempCube[blue][2] = cube[red][2];
	tempCube[red][0] = cube[green][0]; //red side
	tempCube[red][1] = cube[green][1];
	tempCube[red][2] = cube[green][2];
	tempCube[green][0] = cube[orange][0]; //green side
	tempCube[green][1] = cube[orange][1];
	tempCube[green][2] = cube[orange][2];
	tempCube[orange][0] = cube[blue][0]; //orange side
	tempCube[orange][1] = cube[blue][1];
	tempCube[orange][2] = cube[blue][2];

	rotateSide(tempCube, yellow);
	cube = tempCube;
}

void ui(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[blue][0] = cube[orange][0]; //blue side
	tempCube[blue][1] = cube[orange][1];
	tempCube[blue][2] = cube[orange][2];
	tempCube[red][0] = cube[blue][0]; //red side
	tempCube[red][1] = cube[blue][1];
	tempCube[red][2] = cube[blue][2];
	tempCube[green][0] = cube[red][0]; //green side
	tempCube[green][1] = cube[red][1];
	tempCube[green][2] = cube[red][2];
	tempCube[orange][0] = cube[green][0]; //orange side
	tempCube[orange][1] = cube[green][1];
	tempCube[orange][2] = cube[green][2];

	rotateSideCounter(tempCube, yellow);
	cube = tempCube;
}

void d(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[blue][5] = cube[orange][5]; //blue side
	tempCube[blue][6] = cube[orange][6];
	tempCube[blue][7] = cube[orange][7];
	tempCube[red][5] = cube[blue][5]; //red side
	tempCube[red][6] = cube[blue][6];
	tempCube[red][7] = cube[blue][7];
	tempCube[green][5] = cube[red][5]; //green side
	tempCube[green][6] = cube[red][6];
	tempCube[green][7] = cube[red][7];
	tempCube[orange][5] = cube[green][5]; //orange side
	tempCube[orange][6] = cube[green][6];
	tempCube[orange][7] = cube[green][7];

	rotateSide(tempCube, white);
	cube = tempCube;
}

void di(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[blue][5] = cube[red][5]; //blue side
	tempCube[blue][6] = cube[red][6];
	tempCube[blue][7] = cube[red][7];
	tempCube[red][5] = cube[green][5]; //red side
	tempCube[red][6] = cube[green][6];
	tempCube[red][7] = cube[green][7];
	tempCube[green][5] = cube[orange][5]; //green side
	tempCube[green][6] = cube[orange][6];
	tempCube[green][7] = cube[orange][7];
	tempCube[orange][5] = cube[blue][5]; //orange side
	tempCube[orange][6] = cube[blue][6];
	tempCube[orange][7] = cube[blue][7];

	rotateSideCounter(tempCube, white);
	cube = tempCube;
}

void f(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][0] = cube[red][5]; //white side
	tempCube[white][1] = cube[red][3];
	tempCube[white][2] = cube[red][0];
	tempCube[red][0] = cube[yellow][5]; //red side
	tempCube[red][3] = cube[yellow][6];
	tempCube[red][5] = cube[yellow][7];
	tempCube[orange][2] = cube[white][0]; //orange side
	tempCube[orange][4] = cube[white][1];
	tempCube[orange][7] = cube[white][2];
	tempCube[yellow][5] = cube[orange][7]; //yellow side
	tempCube[yellow][6] = cube[orange][4];
	tempCube[yellow][7] = cube[orange][2];

	rotateSide(tempCube, blue);
	cube = tempCube;
}

void fi(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][0] = cube[orange][2]; //white side
	tempCube[white][1] = cube[orange][4];
	tempCube[white][2] = cube[orange][7];
	tempCube[red][0] = cube[white][2]; //red side
	tempCube[red][3] = cube[white][1];
	tempCube[red][5] = cube[white][0];
	tempCube[orange][2] = cube[yellow][7]; //orange side
	tempCube[orange][4] = cube[yellow][6];
	tempCube[orange][7] = cube[yellow][5];
	tempCube[yellow][5] = cube[red][0]; //yellow side
	tempCube[yellow][6] = cube[red][3];
	tempCube[yellow][7] = cube[red][5];

	rotateSideCounter(tempCube, blue);
	cube = tempCube;
}

void b(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][5] = cube[orange][0]; //white side
	tempCube[white][6] = cube[orange][3];
	tempCube[white][7] = cube[orange][5];
	tempCube[red][2] = cube[white][7]; //red side
	tempCube[red][4] = cube[white][6];
	tempCube[red][7] = cube[white][5];
	tempCube[orange][0] = cube[yellow][2]; //orange side
	tempCube[orange][3] = cube[yellow][1];
	tempCube[orange][5] = cube[yellow][0];
	tempCube[yellow][0] = cube[red][2]; //yellow side
	tempCube[yellow][1] = cube[red][4];
	tempCube[yellow][2] = cube[red][7];

	rotateSide(tempCube, green);
	cube = tempCube;
}

void bi(vector<vector<int>> &cube) {
	vector<vector<int>> tempCube = cube;

	tempCube[white][5] = cube[red][7]; //white side
	tempCube[white][6] = cube[red][4];
	tempCube[white][7] = cube[red][2];
	tempCube[red][2] = cube[yellow][0]; //red side
	tempCube[red][4] = cube[yellow][1];
	tempCube[red][7] = cube[yellow][2];
	tempCube[orange][0] = cube[white][5]; //orange side
	tempCube[orange][3] = cube[white][6];
	tempCube[orange][5] = cube[white][7];
	tempCube[yellow][0] = cube[orange][5]; //yellow side
	tempCube[yellow][1] = cube[orange][3];
	tempCube[yellow][2] = cube[orange][0];

	rotateSideCounter(tempCube, green);
	cube = tempCube;
}

void rotateSide(vector<vector<int>> &cube, int side) {
	vector<vector<int>> tempCube = cube;
	tempCube[side][0] = cube[side][5];
	tempCube[side][1] = cube[side][3];
	tempCube[side][2] = cube[side][0];
	tempCube[side][3] = cube[side][6];
	tempCube[side][4] = cube[side][1];
	tempCube[side][5] = cube[side][7];
	tempCube[side][6] = cube[side][4];
	tempCube[side][7] = cube[side][2];
	cube = tempCube;
}

void rotateSideCounter(vector<vector<int>> &cube, int side) {
	vector<vector<int>> tempCube = cube;
	tempCube[side][0] = cube[side][2];
	tempCube[side][1] = cube[side][4];
	tempCube[side][2] = cube[side][7];
	tempCube[side][3] = cube[side][1];
	tempCube[side][4] = cube[side][6];
	tempCube[side][5] = cube[side][0];
	tempCube[side][6] = cube[side][3];
	tempCube[side][7] = cube[side][5];
	cube = tempCube;
}
