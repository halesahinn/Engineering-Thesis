#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define maxchrom  200   //max number of nodes
#define maxpop 1000
#define popSize 100
#define nfunc 3
#define maxfun 3
#define gener 1000

float pcross=0.9; //crossover probability
float pmutation=0.0025; //crossover probability
float W[maxchrom][maxchrom];
float fixedCost[maxchrom];
float O[maxchrom];
float D[maxchrom];

//int hubCosts = 10;
FILE *file;
char *ptr;
float coordinates[maxchrom][2];
float C[maxchrom][maxchrom];

//int i,j,k,l,r,m;
const int numberOfNodes = 200;
char inputFileName[20];
float collectionCost=1.000;
float transferCost=0.75;
float distributionCost=1.000;
int numberOfHubs;
float totalWeight[maxchrom];
float Pk[maxchrom];
float fixedfixedCost[maxchrom];
float Tk[maxchrom]; //the time hub k takes to process one unit of flow



//double W[numberOfNodes][1];

typedef struct       /*individual properties*/
{
    int *hubList, /*bianry chromosome*/
        *assignList,
        *hubIndexList,
        rank,              //Rank of the individual
        flag;             //Flag for ranking
    float fitness[3],//  double totalfixedCost,totalTime,maxTime; //our fitness values
          cub_len;             //crowding distance of the individual
} individual;       /*Structure defining individual*/


typedef struct
{
    int maxrank;            /*Maximum rank present in the population*/
    float rankrat[maxpop];  /*Rank Ratio*/
    int rankno[maxpop];     /*Individual at different ranks*/
    individual ind[popSize], /*Different Individuals*/
               *ind_ptr;
} population;            /*Population Structure*/


void print_array(int array[], int n)
{
    int i;
    printf("\n");
    for(i=0 ; i<n ; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void print_population(population *pop_ptr)
{
    int t,hubNumber;

    for(t=0; t<popSize; t++)
    {
        hubNumber=findHubNumber(pop_ptr->ind[t].hubList,numberOfNodes);
        printf("Hub List %d : ",t);
        print_array(pop_ptr->ind[t].hubList,numberOfNodes);
        printf("Hub Index List %d : ",t);
        print_array(pop_ptr->ind[t].hubIndexList,hubNumber);
        printf("Assign List %d : ",t);
        print_array(pop_ptr->ind[t].assignList,numberOfNodes);
        printf("Total fixedCost Fitness: %lf\n",pop_ptr->ind[t].fitness[0]);
        printf("Total Time Fitness: %lf\n",pop_ptr->ind[t].fitness[1]);
        printf("Maximum Time Fitness: %lf\n",pop_ptr->ind[t].fitness[2]);
        printf("*************************\n\n");
    }
}

void print_pop(population pop)
{
    int t,hubNumber,*hubList;

    for(t=0; t<popSize; t++)
    {
    	hubList=pop.ind[t].hubList;
        hubNumber=findHubNumber(hubList,numberOfNodes);
        printf("Hub List %d : ",t);
        print_array(hubList,numberOfNodes);
        printf("Hub Index List %d : ",t);
        print_array(pop.ind[t].hubIndexList,hubNumber);
        printf("Assign List %d : ",t);
        print_array(pop.ind[t].assignList,numberOfNodes);
        printf("Total fixedCost Fitness: %lf\n",pop.ind[t].fitness[0]);
        printf("Total Time Fitness: %lf\n",pop.ind[t].fitness[1]);
        printf("Maximum Time Fitness: %lf\n",pop.ind[t].fitness[2]);
        printf("*************************\n\n");
    }
}

void readFromFile()
{
    int i, j, k, l, m;
    float capacity;
    float total=0.0;
    if ((file = fopen("Result200.txt", "r+")) != NULL)
    {
       ////read distances
         for (i=0; i<numberOfNodes; i++)
        {
         for (j=0; j<numberOfNodes; j++)
            {
            	fscanf(file,"%f",&C[i][j]);
            }
    	}
    	////read weights
         for (i=0; i<numberOfNodes; i++)
        {
         for (j=0; j<numberOfNodes; j++)
            {
            	fscanf(file,"%f",&W[i][j]);
            	total += W[i][j];
            }
    	}
		///read fixed costs

        for (i=0; i<numberOfNodes; i++)
        {
            fscanf(file,"%f",&fixedCost[i]);
        }

        fscanf(file,"%d",&i); //reads 0
		///read capacities
        for (i=0; i<numberOfNodes; i++)
        {
            fscanf(file,"%f",&capacity);
            Tk[i] = (int)((1/capacity)*28800); //per day (in 28 800 s) 8 saatlik calisma icin
            Pk[i] = (int)(sqrt(capacity)*13);
        }

        fclose(file);


//---------------------------------------------------------------- o d heasbý
        for(i=0; i<numberOfNodes; i++)
        {
            for(j=0; j<numberOfNodes; j++)
            {
                O[i]+=C[i][j];
                D[i]+=C[j][i];
            }
            // printf("%f %f \n",O[i],D[i]);
        }

        //--------------------------------------------i den tüm flowlarýn toplamý

        for(i=0; i<numberOfNodes; i++)
        {
            for(k=0; k<numberOfNodes; k++)
            {
                totalWeight[i]+=W[i][k];// burada spokelar� da ele aliyor,
                //spoketan spoke'a olan weighti de aliyor almamasi lazim.
            }

        }

    }
}

//finding hub number
int findHubNumber(int *hubList)
{
    int i;
    int count = 0;
    for(i=0; i<numberOfNodes ; i++)
    {
        if(hubList[i] == 1)
        {
            count++;
        }
    }

    return count;
}
#include "init.h"         /*Random Initialization of the population*/
#include "calculateFitness.h"
#include "random.h"
#include "ranking.h"
#include "select.h"
#include "crossover.h"
#include "keepaliven.h"
#include "mutation.h"
#include "repair.h"
#include "report.h"       /*Printing the report*/
#include "main-hv.h"


population oldpop,
           newpop,
           matepop,
           selectpop,
           paretopop,
           *old_pop_ptr,
           *new_pop_ptr,
           *mate_pop_ptr;
//population pop,*pop_ptr,newpop;

void main_code(FILE * outfile)
{
    int i,j,k,l,m,f,maxrank1,rank_new,rank_old,pareto_soln;
    float seed,tot,indicator;
    char buff[BUFSIZ];
    double hyper;
FILE
    *rep_ptr,
    *gen_ptr,
    *rep2_ptr,
    *end_ptr,
    *g_var,
	*pop_out_ptr;

  /*File Pointers*/

  rep_ptr = fopen("output.out","w");
  gen_ptr =fopen("all_fitness.out","w");
  rep2_ptr = fopen("ranks.out","w");
  end_ptr = fopen("final_fitness.out","w");
  g_var = fopen("final_var.out","w");
  pop_out_ptr = fopen("populations.out","w");

  /*Opening the files*/


  fprintf(rep_ptr,"Results in a file\n");
  fprintf(end_ptr,"# Last generation population (Feasible and non-dominated)\n");
  fprintf(end_ptr,"# Fitness_vector (first %d)  \n",nfunc);
  fprintf(g_var,"#Feasible Variable_vectors for non-dominated solutions at last generation\n");
  fprintf(g_var,"# Binary (next %d)\n",numberOfNodes);



    old_pop_ptr = &(oldpop);
    warmup_random(seed);

    init(old_pop_ptr);

    old_pop_ptr = &(oldpop);

    calculateFitness(old_pop_ptr);

    old_pop_ptr = &(oldpop);
    new_pop_ptr = &(newpop);
    mate_pop_ptr = &(matepop);


    for(k = 0; k < popSize; k++)   /*Initializing the Rank array having different individualst a particular  rank to zero*/
    {
        old_pop_ptr->rankno[k] = 0;
        new_pop_ptr->rankno[k] = 0;
        mate_pop_ptr->rankno[k] = 0;
        selectpop.rankno[k] = 0;
    }

    old_pop_ptr = &(oldpop);
    ranking(old_pop_ptr);
  fprintf(rep_ptr,"----------------------------------------------------\n");
  fprintf(rep_ptr,"Statistics at Generation 0 ->\n");
  fprintf(rep_ptr,"--------------------------------------------------\n");
    /*----------------------GENERATION STARTS HERE----------------------*/
    for (i = 0; i < gener; i++)
    {
    	old_pop_ptr = &(oldpop);

        fprintf(rep_ptr,"Population at generation no. -->%d\n",i+1);
      	fprintf(gen_ptr,"#Generation No. -->%d\n",i+1);
      	fprintf(gen_ptr,"#Hub_List	#Assign_List	#Fitness_Values \n");

        selectpop=nselect(oldpop);//---------SELECT----------------
        newpop=crossover(selectpop);//-------CROSSOVER----------------
        newpop=mutate(newpop);//-------MUTATION--------------------
        newpop=repair(newpop);//------REPAIR----------------------

        new_pop_ptr=&(newpop);

        calculateFitness(new_pop_ptr);//-------FITNESS CALCULATION------------

        new_pop_ptr = &(newpop);

        ranking(new_pop_ptr);//-------RANKING----------------

        new_pop_ptr = &(newpop);

        keepalive(old_pop_ptr ,new_pop_ptr ,mate_pop_ptr,i+1);//-------POPULATION POOL-----------

        mate_pop_ptr = &(matepop);


      report(i ,oldpop ,matepop ,rep_ptr ,gen_ptr );      //-------REPORT PRINTING------
      /*----------------Rank Ratio Calculation------------------------*/
      //Finding the greater maxrank among the two populations
      if(oldpop.maxrank > matepop.maxrank)
		maxrank1 = oldpop.maxrank;
      else
		maxrank1 = matepop.maxrank;

      fprintf(rep2_ptr,"--------RANK AT GENERATION %d--------------\n",i+1);
      fprintf(rep2_ptr,"Rank\told ranks\tnew ranks\t\trank ratio\n");
      for(j = 0;j < maxrank1 ; j++)
	{
		rank_old=oldpop.rankno[j];
		rank_new=matepop.rankno[j];
	  tot = (rank_old)+ (rank_new);
	  matepop.rankrat[j] = (float)(rank_new)/tot;//Finding the rank ratio for new population at this rank
	  //Printing this rank ratio to a file called ranks.dat
	   fprintf(rep2_ptr," %d\t\t\t%d\t\t\t%d\t\t\t%f\n",j+1,rank_old,rank_new,matepop.rankrat[j]);
	}
 	fprintf(rep2_ptr,"-----------------Rank Ratio-------------------\n");


        //////////////////////// COPYING MATE POP TO OLD POP ///////////////////////////////
        for(j=0;j<popSize;j++){
	        for(l = 0 ; l < nfunc ;l++){
		    	oldpop.ind[j].fitness[l] = matepop.ind[j].fitness[l];
			}
		    oldpop.ind[j].assignList=matepop.ind[j].assignList;
		    oldpop.ind[j].hubList=matepop.ind[j].hubList;
		    oldpop.ind[j].hubIndexList=matepop.ind[j].hubIndexList;
		    oldpop.ind[j].cub_len=matepop.ind[j].cub_len;
		    oldpop.ind[j].rank=matepop.ind[j].rank;
		    oldpop.ind[j].flag=matepop.ind[j].flag;
		}
		maxrank1 = matepop.maxrank ;
		for(l = 0;l < popSize;l++){
			oldpop.rankno[l] = matepop.rankno[l];}
		oldpop.maxrank = matepop.maxrank;



		pareto_soln=0;
       	FILE *pareto_ptr;
		pareto_ptr = fopen("pareto_fronts.out","w");
       	 for(f = 0;f < popSize ; f++) // for printing
	    {
		      if (matepop.ind[f].rank == 1)  // for all non-dominated solutions
			{

			for(l = 0;l < nfunc;l++){
			  	paretopop.ind[pareto_soln].fitness[l]=matepop.ind[f].fitness[l];
			    fprintf(pareto_ptr,"%f\t",matepop.ind[f].fitness[l]);
			}
			    pareto_soln++;
			    if(f!=popSize-1){
				fprintf(pareto_ptr,"\n");
				}
			}

		}  // feasibility check
		fclose(pareto_ptr);
		hyper=hypervolume(pareto_soln);
		fprintf(outfile,"%lf\n",hyper);
		//printf("%lf\n",hyper);
		remove("pareto_fronts.out");



}//end of generation loop



/*Closing the files*/
  fclose(rep_ptr);
  fclose(gen_ptr);
  fclose(rep2_ptr);
  fclose(end_ptr);
  fclose(g_var);
  fclose(outfile);

}

int main(){
	FILE *outfile;
	int i=0;
	readFromFile();
    srand(time(0));

	for(i=0;i<10;i++){
		char s1[100];
		char s2[100];
		char si[10];
		strcpy(s1,"hyper_out");
		itoa(i+1, si, 10);
		strcat(s1,si);
		strcpy(s2,".txt");
		strcat(s1,s2);
		printf("%s\n",&s1);
		outfile=fopen(s1,"a");
		main_code(outfile);
	}
}
