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
	int mainArray[PIXELS][PIXELS * 3]; //2d array with PIXELS rows and PIXELS*3 columns.
	//PIXELS*3 columns are needed to hold an R, G, and B value for each pixel.
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
	
	if(rank == 0) { //I am the master
	  cout << "P3" <<endl; //Get PPM file ready
		cout << PIXELS << " " << PIXELS << endl;
	  cout << "255" <<endl;

		currentRow = 0;
		for(int i=1;i<size;++i){ //Send out initial rows to each process
			MPI_Send(&currentRow, 1, MPI_INT, i, 0, MCW); 
			currentRow++;
		}

		int received = 0;
		while(received < PIXELS - 1) { //Continue while fewer than PIXELS rows have been received
			MPI_Status status;
			rgbVect.resize(PIXELS*3);
			//Receive a vector of RGB values
			t
			t
			t
			t
			t
			t
			t
			MPI_Recv(&rgbVect[0], (PIXELS * 3), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &status);

			for(int k = 0; k < (PIXELS * 3); k++) { //Copy vector RGB values over to the main array
				mainArray[status.MPI_TAG][k] = rgbVect[k];
			}

			received++;
			if(currentRow < PIXELS - 1) { 
				//If not all rows have been sent out, send the next one to the process that just finished
				currentRow++;
				MPI_Send(&currentRow, 1, MPI_INT, status.MPI_SOURCE, 0, MCW);
			}
		}
		
		for(int i = 1; i < size; i++) { //Send -1 to each process to signify that the work is done
			currentRow = -1;
			MPI_Send(&currentRow, 1, MPI_INT, i, 0, MCW);
		}

		for(int i = 0; i < PIXELS; i++) { //Print out all values to the PPM file
			for(int j = 0; j < PIXELS * 3; ++j) {
				cout<<mainArray[i][j]<<" ";
			}
			cout<<endl;
		}	
	}
	else { //If I am not the master
		currentRow = 0;
		while(currentRow != -1) {
			MPI_Recv(&currentRow, 1, MPI_INT, 0, 0, MCW, MPI_STATUS_IGNORE); //Receive a row to work on
			
			rgbVect.clear();
			if(currentRow != -1) { //If the currentRow is not the stop signal, continue
				for(int j=0;j<PIXELS;++j){
						cx.i = c1.i + j * iinc;
						cx.r = c1.r + currentRow * rinc;
						iters = mbrot_iters(cx);        
						getColor(iters, rgbVect); //Fill the vector with RGB values
				} 
				MPI_Send(&rgbVect[0], (PIXELS*3), MPI_INT, 0, currentRow, MCW); //Send my result back to master
			}
		}
	}
	
	MPI_Finalize();

	return 0;
}

