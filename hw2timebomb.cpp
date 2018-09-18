#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv) {
  int rank, size;
  int timer=rand()%(100 + 1);

  MPI_Init(&argc, &argv);  
	MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size); 
		
	if(rank==0){
		MPI_Send(&timer,1,MPI_INT,rand()%size,0,MCW);
  }
	
	while(1) {
		MPI_Recv(&timer,1,MPI_INT,MPI_ANY_SOURCE,0,MCW,MPI_STATUS_IGNORE);
	
	if(!timer){
			cout << "BOOOM!! Process "<< rank << " blew up!"<<endl; 
			timer = -1;
			for(int i=0;i<size;++i){
				MPI_Send(&timer,1,MPI_INT,i,0,MCW);
		  }
		}
		else if(timer<0) {	
			break;
		} 
		else {
			cout<<"I am process "<<rank<<" and the timer is: "<<timer<<endl;
			timer--;
		}

		MPI_Send(&timer,1,MPI_INT,rand()%size,0,MCW);
	}

  MPI_Finalize();
	return 0;
}

