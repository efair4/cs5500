#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <mpi.h>
#include <vector>
#define MCW MPI_COMM_WORLD
#define NUMGENOMES 250

using namespace std;

struct City{
	int x;
	int y;
};

struct Genome{
	int c[100];
	long fitness;
};

void read100(City cities[100]){
	ifstream infile;
	infile.open("tsinput.txt");
	if(!infile) {cout<<"Error opening file"<<endl;return;}
	for(int i=0;i<100;++i){
		infile>>cities[i].x>>cities[i].y;
  }
}

void setFitness(Genome &g, City cities[100]){
  long ydist; long xdist;
  g.fitness=0;
  for(int j=0;j<99;++j){
	  ydist = cities[g.c[j]].y - cities[g.c[j+1]].y;
	  xdist = cities[g.c[j]].x - cities[g.c[j+1]].x;
		long root = sqrt(ydist*ydist+xdist*xdist);
	  g.fitness += root; 
	}
}

void setupGenome(Genome g[NUMGENOMES], City cities[100]){
	for(int i=0;i<NUMGENOMES;++i){
	  for(int j=0;j<100;++j){
	    g[i].c[j]=j;
	  }
	  for(int j=0;j<NUMGENOMES;++j){
	    int a; int b; int t;
		  a=rand()%100; b=rand()%100;
		  t=g[i].c[a];
		  g[i].c[a]=g[i].c[b];
		  g[i].c[b]=t;
		}	
		setFitness(g[i],cities);
	}
}

void pmx(Genome &g1, Genome &g2){
  int start = rand()%100;
  int end = rand()%100;
  int t;
  int j,k;
  if(start>end){ t=start; start=end; end=t; }
  for(int i=start;i<end;++i){
    for(j=0;j<100;++j) if(g1.c[j]==g2.c[i])break;
    for(k=0;k<100;++k) if(g2.c[k]==g1.c[i])break;
  	t=g1.c[i]; g1.c[i]=g1.c[j]; g1.c[j]=t;
		t=g2.c[i]; g2.c[i]=g2.c[k]; g2.c[k]=t;
  }																														    
}

void mutate(Genome &g1){
  int start = rand()%100;
  int end = rand()%100;
  int t;
  if(start>end){ t=start; start=end; end=t; }
  for(int i=start;i<(end-start)/2+start;++i){
    t=g1.c[i]; g1.c[i]=g1.c[end-i]; g1.c[end-i]=t;
  }																	    
}

long bestFitness(Genome g[NUMGENOMES]){
  long bf;
  bf=g[0].fitness;
  for(int i=0;i<NUMGENOMES;++i){
    if(g[i].fitness<bf)bf=g[i].fitness;
  }
  return bf;															    
}

void mateAndSelect(Genome &g1, Genome &g2, City cities[100]){
  Genome ng1,ng2;
  ng1 = g1;
  ng2 = g2;
  pmx(ng1,ng2);
  if(rand()%200==42) mutate(ng1);
  if(rand()%200==42) mutate(ng2);
  setFitness(ng1, cities);
  setFitness(ng2, cities);
  if(ng1.fitness<g1.fitness)g1=ng1;
  if(ng2.fitness<g2.fitness)g2=ng2;																			    
}

int main(int argc, char **argv){  
	int rank, size;
	int flag;
	vector<int> fitnessVec;
	City cities[100];
	Genome g[NUMGENOMES];
	int mate;
	long oldbf=-1;
	long bf;
	MPI_Status status;
	
	read100(cities);

	MPI_Init(&argc, &argv);

	const int nitems=2;
  int blocklengths[2] = {100,1};
	MPI_Datatype types[2] = {MPI_INT, MPI_INT};
	MPI_Datatype mpi_genome_type;
	MPI_Aint offsets[2];

	offsets[0] = offsetof(Genome, c);
	offsets[1] = offsetof(Genome, fitness);
  MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_genome_type);
  MPI_Type_commit(&mpi_genome_type);

	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	srand(time(NULL)+rank);
	setupGenome(g,cities);
	int count = 0;
	while(count < 24){
		MPI_Iprobe(MPI_ANY_SOURCE, 3, MCW, &flag, &status);
		if(flag) {
			Genome newGenomes[10];
			MPI_Recv(&newGenomes[0], 10, mpi_genome_type, status.MPI_SOURCE, 3, MCW, MPI_STATUS_IGNORE);
			for(int i = 0; i < 10; i++) {
				g[i] = newGenomes[i];
			}
		}
	  for(int i=0;i<NUMGENOMES;++i){
	    mate=rand()%NUMGENOMES;
	    mateAndSelect(g[i],g[mate],cities);
	  }
		bf=bestFitness(g);
		if(oldbf==-1){
			oldbf = bf;
			fitnessVec.push_back(oldbf);
		}
		else{
		  if(bf<oldbf){
				oldbf=bf;
				fitnessVec.push_back(bf);
		  }
		}

		if(count%14 == 0) {
			MPI_Send(&g[0], 10, mpi_genome_type, rand()%size, 3, MCW);
		}

		count++;
	}
	if(rank == 0) {
		for(int i = 0; i < fitnessVec.size(); i++) {
			cout<<"Rank "<<rank<<" Best fitness: "<<fitnessVec[i]<<endl;
		}
		cout<<endl;
		int vecSize;
		for(int i = 1; i < size; i++) {
			MPI_Recv(&vecSize, 1, MPI_INT, i, 0, MCW, MPI_STATUS_IGNORE);
			int incomingArr[vecSize];
			MPI_Recv(&incomingArr[0], vecSize, MPI_INT, i, 1, MCW, MPI_STATUS_IGNORE);
			for(int j = 0; j < vecSize; j++) {
				cout<<"Rank "<<i<<" Best fitness: "<<incomingArr[j]<<endl;
			}
			cout<<endl;
		}
	}
	else {
		cout<<"here"<<endl;
		int myVecSize = fitnessVec.size();
		MPI_Send(&myVecSize, 1, MPI_INT, 0, 0, MCW);
		MPI_Send(&fitnessVec[0], myVecSize, MPI_INT, 0, 1, MCW);
	}

	MPI_Finalize();
	return 0;
}
