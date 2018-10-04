#include <iostream>
#include <fstream>
#include <complex>
#include <mpi.h>
#include <vector>
#define MCW MPI_COMM_WORLD
using namespace std;

float width = 512;
float height = 512; 

int value (int x, int y)  {
	complex<float> point((float)x/width-1.5, (float)y/height-0.5);
	complex<float> z(0, 0);
	complex<float> w(.8, .8);

	int nb_iter = 0;
	while (abs (z) < 3 && nb_iter <= 20) {
		z = z * z + point / w;
		nb_iter++;
	}
	if (nb_iter < 20)
	  return (255*nb_iter)/20;
	else 
	  return 0;
}

int getVal(double real0, double real1, double imag0) {
	complex<float> imag1(0, 0);
	int max = 512;
	int iter = 0;
	while(abs(imag1) < 2 && iter <= max) {
		imag1 = imag0 + (real1 - real0);
		iter++;
	}
	return(255 * iter)/max;
}

vector<int> getColor(int iterations) {
	int r, g, b;
  // colour gradient:      Red -> Blue -> Green -> Red -> Black
  // corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
  if (iterations < 16) {
	  r = 16 * (16 - iterations);
	  g = 0;
	  b = 16 * iterations - 1;
	} else if (iterations < 32) {
	  r = 0;
	  g = 16 * (iterations - 16);
	  b = 16 * (32 - iterations) - 1;
	} else if (iterations < 64) {
	  r = 8 * (iterations - 32);
	  g = 8 * (64 - iterations) - 1;
	  b = 0;
	} else { // range is 64 - 127
	  r = 255 - (iterations - 64) * 4;
	  g = 0;
	  b = 0;
	}
//	vector<int> vals;
	int valArray[] = {r, g, b};
	vector<int> vals(valArray, valArray + 3);
	return vals;
}

int main (int argc, char **argv)  {
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	if(rank == 0) {
		double start = MPI_Wtime();
		ofstream my_Image ("mandelbrot.ppm");

		if (my_Image.is_open ()) {
			my_Image << "P3\n" << width << " " << height << " 255\n";
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++)  {
					int val = getVal(i, j, 0.75); //value(i, j);
					my_Image << val << ' ' << 0 << ' ' << val << "\n";
			  }
	   }
			my_Image.close();
		}
		else
			cout << "Could not open the file";
		double end = MPI_Wtime();
		cout<<"Elapsed Time: "<<end - start<<endl;
	}
	MPI_Finalize();
  return 0;
}
