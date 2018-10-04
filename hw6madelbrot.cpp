#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <cmath>
#include <vector>
#define MCW MPI_COMM_WORLD
#define PIXELS 512

using namespace std;

void getColor(int i, vector<int> &rgbVals){
	int r,g,b;

	if(i == 0) {i = 1;}
	r=(log(i)/log(1024))*255;
	g=(log(i)/log(1024))*55;
	b=(log(i)/log(1024))*255;
	if (i==1024) {r=0; g=0; b=0;}
	//cout << r << " ";
	//cout << g << " ";
	//cout << b << " ";															    
	rgbVals.push_back(r);
	rgbVals.push_back(g);
	rgbVals.push_back(b);
}

struct Complex{
  double r;
  double i;
};

Complex operator * (Complex a, Complex b){
  Complex c;
  c.r = a.r*b.r-a.i*b.i;
	c.i = a.r*b.i+a.i*b.r;
	return c;
}

Complex operator + (Complex a, Complex b){
  Complex c;
  c.r = a.r+b.r;
  c.i = a.i+b.i;
  return c;
}

Complex operator - (Complex a, Complex b){
  Complex c;
  c.r = a.r-b.r;
	c.i = a.i-b.i;
	return c;
}

int mbrot_iters(Complex c){
  int i=0;
  Complex z = c;
  while(z.r*z.r+z.i*z.i<2.0*2.0 && i<1024){
    z = z*z+c;
    i++;
  }
  return i;
}


int main(int argc, char **argv){
  Complex c1,c2,cx,cdiff;
  double rinc;
  double iinc;
  int iters;
	int rank, size;
	int currentRow;
	vector<int> rgbVect;
  c1.r = -2.0;
  c2.r = 0.0;
  c1.i = 1.0;
  c2.i = -1.0;
  cdiff = c2-c1;
	rinc = cdiff.r/PIXELS;
	iinc = cdiff.i/PIXELS;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	
	if(rank == 0) {
	  cout << "P3" <<endl;
		cout << PIXELS << " " << PIXELS << endl;
	  cout << "255" <<endl;
		int currentProcess = 1;
//		currentRow = 0;
//		while(currentRow < PIXELS) {
//			cout<<"%%%%%%%%%%%%%%  "<<currentRow<<"  %%%%%%%%%%%%%%"<<endl<<endl;
//			for(int i=1;i<size;++i){
//				MPI_Send(&currentRow, 1, MPI_INT, i, 0, MCW); 
//				currentRow++;
//			}
//			currentRow-=size-1;
//			for(int j=1;j<size;++j) {
//				cout<<"current row = "<<currentRow<<" rank: "<<j<<endl;
//				rgbVect.resize(PIXELS*3);
//				MPI_Recv(&rgbVect[0], (PIXELS * 3), MPI_INT, j, currentRow, MCW, MPI_STATUS_IGNORE);
//				currentRow++;
//				int index = 0;
//				for(int j = 0; j < PIXELS; ++j) {
//					for(int k = 0; k < 3; k++) {
//						cout<<rgbVect[index]<<" ";
//						index++;
//					}
//				}
//				cout<<endl;
//			}
//		}
			currentProcess = 1;
			for(int i=0;i<PIXELS;++i){
				currentRow = i;
			cout<<"current row in for loop: "<<currentRow<<" Process: "<<currentProcess<<endl;	
				MPI_Send(&currentRow, 1, MPI_INT, currentProcess, 0, MCW); 
				currentProcess++;
				if(currentProcess == size) {currentProcess = 1;}
			}
			currentProcess = 1;
			for(int j=0;j<PIXELS;++j) {
				rgbVect.resize(PIXELS*3);
				MPI_Recv(&rgbVect[0], (PIXELS * 3), MPI_INT, currentProcess, j, MCW, MPI_STATUS_IGNORE);
				currentProcess++;
				if(currentProcess == size) {currentProcess = 1;}
				int index = 0;
				for(int k = 0; k < PIXELS; ++k) {
					for(int l = 0; l < 3; l++) {
						cout<<rgbVect[index]<<" ";
						index++;
					}
				}
				cout<<endl;
			}
	}
	else {
		MPI_Recv(&currentRow, 1, MPI_INT, 0, 0, MCW, MPI_STATUS_IGNORE);
		rgbVect.clear();
		for(int j=0;j<PIXELS;++j){
		    cx.i = c1.i + j * iinc;
		    cx.r = c1.r + currentRow * rinc;
		    iters = mbrot_iters(cx);        
		    getColor(iters, rgbVect);
	  } 
		cout<<"current row before send: "<<currentRow<<" Rank: "<<rank<<endl;;	
		MPI_Send(&rgbVect[0], (PIXELS*3), MPI_INT, 0, currentRow, MCW);
	}

		
	MPI_Finalize();

	return 0;
}

