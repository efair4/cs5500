#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <mpi.h>

using namespace std;

struct City{
	int x;
	int y;
};

struct Genome{
	int c[100];
	long fitness;
};

void read100(City city[100]){
	ifstream infile;
	infile.open("tsinput.txt");
	if(!infile) {cout<<"Error opening file"<<endl;return;}
	for(int i=0;i<100;++i){
		infile>>city[i].x>>city[i].y;
  }
}

void setFitness(Genome &g, City city[100]){
  long ydist; long xdist;
  g.fitness=0;
  for(int j=0;j<99;++j){
	  ydist = city[g.c[j]].y - city[g.c[j+1]].y;
	  xdist = city[g.c[j]].x - city[g.c[j+1]].x;
	  g.fitness += sqrt(ydist*ydist+xdist*xdist);
	}
}

void setupGenome(Genome g[1000], City city[100]){
	for(int i=0;i<1000;++i){
	  for(int j=0;j<100;++j){
	    g[i].c[j]=j;
	  }
	  for(int j=0;j<1000;++j){
	    int a; int b; int t;
		  a=rand()%100; b=rand()%100;
		  t=g[i].c[a];
		  g[i].c[a]=g[i].c[b];
		  g[i].c[b]=t;
		}
		setFitness(g[i],city);
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

long bestFitness(Genome g[1000]){
  long bf;
  bf=g[0].fitness;
  for(int i=0;i<1000;++i){
    if(g[i].fitness<bf)bf=g[i].fitness;
  }
  return bf;															    
}

void mateAndSelect(Genome &g1, Genome &g2, City city[100]){
  Genome ng1,ng2;
  ng1 = g1;
  ng2 = g2;
  pmx(ng1,ng2);
  if(rand()%200==42) mutate(ng1);
  if(rand()%200==42) mutate(ng2);
  setFitness(ng1, city);
  setFitness(ng2, city);
  if(ng1.fitness<g1.fitness)g1=ng1;
  if(ng2.fitness<g2.fitness)g2=ng2;																			    
}

int main(){  
	City city[100];
	Genome g[1000];
	int mate;
	long oldbf=-1;
	long bf;
	
	read100(city);
	setupGenome(g,city);
	int count = 0;
	while(count < 48){
	  for(int i=0;i<1000;++i){
	    mate=rand()%1000;
	    mateAndSelect(g[i],g[mate],city);
	  }
		bf=bestFitness(g);
		if(oldbf==-1){
		  cout<<bf<<endl;
			oldbf = bf;
		}
		else{
		  if(bf<oldbf){
		    oldbf=bf;
		    cout<<bf<<endl;
		  }
		}
		count++;
	}
	return 0;
}
