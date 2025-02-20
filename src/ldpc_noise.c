/* 
  FILE...: ldpc_enc.c
  AUTHOR.: Don Reid
  CREATED: Aug 2018

  Add noise to ldpc sd samples for testing.
  
  First pass only adds AGWN with variance = 1

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <errno.h>

/*
int opt_exists(char *argv[], int argc, char opt[]) {
    int i;
    for (i=0; i<argc; i++) {
        if (strcmp(argv[i], opt) == 0) {
            return i;
        }
    }
    return 0;
}
*/

int main(int argc, char *argv[]) {
    FILE        *fin, *fout;
    double	datain, dataout;

    if (argc < 3) {
        fprintf(stderr, "\n");
        fprintf(stderr, "usage: %s InputFile OutputFile NodB\n", argv[0]);
        fprintf(stderr, "\n");
        exit(1);
    }

    if (strcmp(argv[1], "-")  == 0) fin = stdin;
    else if ( (fin = fopen(argv[1],"rb")) == NULL ) {
        fprintf(stderr, "Error opening input bit file: %s: %s.\n",
                argv[1], strerror(errno));
        exit(1);
    }
        
    if (strcmp(argv[2], "-") == 0) fout = stdout;
    else if ( (fout = fopen(argv[2],"wb")) == NULL ) {
        fprintf(stderr, "Error opening output bit file: %s: %s.\n",
                argv[2], strerror(errno));
        exit(1);
    }

    double NodB = atof(argv[3]);
    double No = pow(10.0, NodB/10.0);
    double sum_xx = 0; double sum_x = 0.0; long n = 0;
    
    fprintf(stderr, "single sided NodB = %f, No = %f\n", NodB, No);
    
    while (fread(&datain, sizeof(double), 1, fin) == 1) {

	// Gaussian from uniform:
	double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        double z = sqrt(-2 * log(x)) * cos(2 * M_PI * y);

	double noise = sqrt(No/2) * z;
	dataout = datain + noise;

        fwrite(&dataout, sizeof(double), 1, fout);        

        // keep running stats to calculate actual noise variance (power)
        
        sum_xx += noise*noise;
        sum_x  += noise;
        n++;
    }

    fclose(fin);  
    fclose(fout); 

    double noise_var = (n * sum_xx - sum_x * sum_x) / (n * (n - 1));
    fprintf(stderr, "measured double sided (real) noise power: %f\n", noise_var);
 
    return 0;
}
