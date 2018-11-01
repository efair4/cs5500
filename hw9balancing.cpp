#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<queue>
#define MCW MPI_COMM_WORLD
#define ANY MPI_ANY_SOURCE
#define IGNORE MPI_STATUS_IGNORE
#define BLACK 0
#define WHITE 1
#define JOB 1
#define TOKEN 2
#define CREATED 3
#define COMPLETED 4

using namespace std;

int main(int argc, char **argv) {
	int rank, size;
	int jobFlag, tokenFlag;
	int tasksCreated = 0;
	int tasksCompleted = 0;
	int taskLimit;
	int tokenColor = WHITE;
	int myColor = WHITE;
	bool hasToken = false;
	queue<int> myTasks;
	int newTask;
	MPI_Status status;
	MPI_Request request;

	MPI_Init(&argc, &argv);
	srand(time(NULL));
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	
	taskLimit = rand() % 1024 + 1024; 

	if(rank == 0) {
		hasToken = true;
		int job = rand() % 3 + 1;
		MPI_Send(&job, 1, MPI_INT, rand() % 4, JOB, MCW);
	}

	while(1) {
		MPI_Iprobe(ANY, JOB, MCW, &jobFlag, &status);
	//	if(!jobFlag) break;//not sure on this. Why is probe needed?
		MPI_Irecv(&newTask, 1, MPI_INT, ANY, JOB, MCW, &request);
		if(jobFlag && newTask == -1) {
			MPI_Send(&tasksCreated, 1, MPI_INT, 0, CREATED, MCW);
			MPI_Send(&tasksCompleted, 1, MPI_INT, 0, COMPLETED, MCW);
			break;
		}
		myTasks.push(newTask);
		if(myTasks.size() > 16) {
			for(int i = 0; i < 2; i++) {
				int task = myTasks.front();
				myTasks.pop();
				int dest = rand() % 4;
				while(dest == rank) {dest = rand() % 4;}
				if(dest < rank) {myColor = BLACK;}
				MPI_Send(&task, 1, MPI_INT, dest, JOB, MCW);
			}
		}
		else if(!myTasks.empty()) {
			int num = myTasks.front();
			myTasks.pop();
			cout<<"Rank "<<rank<<" is sleeping for "<<num/10<<" seconds "<<
			"and has "<<myTasks.size()<<" tasks in the queue"<<endl;
			sleep(num/10);
			tasksCompleted++;
		}
		else if(tasksCreated < taskLimit) {
			for(int i = 0; i < rand() % 3 + 1; i++) {
				myTasks.push(rand() % 3 + 1);
				tasksCreated++;
			}
		}
		else if(myTasks.empty()) {
			if(hasToken) {
				int dest = rank + 1 < size ? rank + 1 : 0;
				MPI_Send(&tokenColor, 1, MPI_INT, dest, TOKEN, MCW);
				myColor = WHITE;
			}
			else {
				int src = rank - 1 >= 0 ? rank - 1 : size - 1;
				MPI_Iprobe(src, TOKEN, MCW, &tokenFlag, &status);
				if(tokenFlag) {
					MPI_Irecv(&tokenColor, 1, MPI_INT, src, TOKEN, MCW, &request);
					if(rank == 0 && tokenColor == WHITE) {
						int globalStop = -1;
						for(int i = 1; i < size; i++) {
							MPI_Send(&globalStop, 1, MPI_INT, i, JOB, MCW);
						}
						break;
					}
					if(myColor == BLACK) {
						tokenColor = BLACK;
					}
				}
			}
		}
	}

	if(rank == 0) {
		int created, completed;
		for(int i = 1; i < size; i++) {
			MPI_Recv(&created, 1, MPI_INT, i, CREATED, MCW, IGNORE);
			MPI_Recv(&completed, 1, MPI_INT, i, COMPLETED, MCW, IGNORE);
			tasksCreated += created;
			tasksCompleted += completed;
		}
		cout<<"Tasks Created: "<<tasksCreated<<endl;
		cout<<"Tasks Completed: "<<tasksCompleted<<endl;
	}


	MPI_Finalize();
	return 0;
}
