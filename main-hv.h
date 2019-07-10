

#include "hv.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


double hypervolume( int itemnumber)
{

    double hyperv,hyper;
    int nobj = 3,i,j;
    double data[itemnumber][nobj];
    double *data2;
    FILE *outfile,* infile;
    double  * reference;
		reference=malloc(3*sizeof(float));
		reference[0]=40000000.0;
		reference[1]=25000000.0;
		reference[2]=700000.0;
	infile=fopen("pareto_fronts.out","r+");
	for(i=0;i<itemnumber;i++){
		for(j=0;j<nobj;j++){
			fscanf(infile,"%lf",&(data[i][j]));
		}
	}
	fclose(infile);
	data2 = &(data[0][0]);
	hyperv = fpli_hv (data2, nobj,itemnumber, reference);
	hyper=hyperv/334652279246905210000.000000;// for 10 node
	return hyper;
}
