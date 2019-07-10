/*This is a routine to keep the fronts alive (caring the end problem)*/

void keepalive(population *pop1_ptr,population *pop2_ptr,population *pop3_ptr,int gen);

typedef struct
{
  int maxrank,   /*Max rank of the global population*/
    rankar[2*maxpop][2*maxpop], /*record of array of individual numbers at
				  a particular rank */
    rankno[2*maxpop];           /*record of no. of individuals at a particular rank*/

  int hubList[2*maxpop][maxchrom],
  	assignList[2*maxpop][maxchrom] ,
	hubIndexList[2*maxpop][maxchrom],
    rank[2*maxpop],            /*rank of different individuals*/
    flag[2*maxpop];            /*Setting the flag */

  float fitness[2*maxpop][3], /*Fitness function values for the different
				     individuals*/
    cub_len[2*maxpop];              /*Dummyfitness*/

}globpop;

/*Population structure for the pool having both the old as well as new
  population*/

globpop globalpop,*global_pop_ptr;

void grank(int gen);
/*Ranking the global pool*/

int indcmp1(float *ptr1,float *ptr2);
/*Comparison of the variables*/

void gsort(int rnk,int sel);
/*Sorting for the function values in ascending order*/

void gshare(int rnk);
/*Sharing the fitness*/

void sort(int rnk);

int left,Lastrank;
float fpara1[2*maxpop][2];

void keepalive(population *pop1_ptr,population *pop2_ptr,population *pop3_ptr,int gen)
{
  int i,j,jj,k,m,a1,l,front_pop[maxpop],rec,*hil,*hil2,hubnum,hubnum2,hubnum3;

  int sum,st,str,pool,poolf,sel,r1;

  int *gene1_ptr, *gene2_ptr,leftsum,x;

   float rnd,a,*gene3_ptr,x3,*gene4_ptr;


  /*Forming the global mating pool*/

  for(i = 0;i < popSize;i++)
    {
      if(numberOfNodes > 0)
	{
	  /*Binary Coded GA hubList are copied*/
	    for(k = 0;k < numberOfNodes;k++)
	    {
	      	globalpop.hubList[i][k]=pop1_ptr->ind[i].hubList[k];
	      	globalpop.hubList[i+popSize][k] = pop2_ptr->ind[i].hubList[k];
	    	globalpop.assignList[i][k]=pop1_ptr->ind[i].assignList[k];
	      	globalpop.assignList[i+popSize][k] = pop2_ptr->ind[i].assignList[k];

	    }
	    hubnum=findHubNumber(pop1_ptr->ind[i].hubList);
	    hil=initialize_hubIndexList(pop1_ptr->ind[i].hubList,hubnum);
	    for(k=0;k<hubnum;k++){
			globalpop.hubIndexList[i][k]=hil[k];}

	    hubnum2=findHubNumber(pop2_ptr->ind[i].hubList);
	    hil2=initialize_hubIndexList(pop2_ptr->ind[i].hubList,hubnum2);
	    for(k=0;k<hubnum2;k++){
			globalpop.hubIndexList[i+popSize][k]=hil2[k];}
	 }
      /*Fitness is copied to the global pool */
       for(l = 0;l < nfunc;l++)
	 {

	   globalpop.fitness[i][l] = pop1_ptr->ind[i].fitness[l];
	   globalpop.fitness[i+popSize][l] = pop2_ptr->ind[i].fitness[l];
	 }

       /*Initial;ising the dummyfitness to zero */
       globalpop.cub_len[i] = 0;
       globalpop.cub_len[i+popSize] = 0;
    }


  global_pop_ptr = &(globalpop);
  grank(gen);

  m = globalpop.maxrank;

  /* Sharing the fitness to get the dummy fitness */
  for(i = 0;i < m;i++)
    {
      gshare(i+1);
    }

  poolf = popSize;
  pool = 0;


  /*Initializing the flags of population to zero */
  for(i = 0;i < 2*popSize;i++)
    {
      globalpop.flag[i] = 0;
    }
  // decide which all solutions belong to the pop3
  rec = 0;
  st = 0;
  for(i = 0 ;i < m ; i++)
    {
      /*    Elitism Applied Here     */
      st = pool;
      pool += globalpop.rankno[i];

      if(pool <= popSize)
	{
	  for(k = 0;k < 2*popSize ;k++)
	    {
	      if(globalpop.rank[k] == i+1)
		globalpop.flag[k] = 1;
	    }
	  pop3_ptr->rankno[i] = globalpop.rankno[i];
	}
      else
	{
	  sel = popSize - st;
	  Lastrank = i+1;
	  pop3_ptr->rankno[i] = sel;
	  gsort(i+1,sel);
	  break;
	}
    }

  k = 0;




  for(i = 0,k = 0;i < 2*popSize && k < popSize; i++)
    {
    	pop3_ptr->ind[k].hubList = (int *) malloc(numberOfNodes * sizeof(int));
		pop3_ptr->ind[k].assignList = (int *) malloc(numberOfNodes * sizeof(int));
    if(numberOfNodes > 0)
	{
    if(globalpop.flag[i] == 1)
	{
	   for(l = 0;l < numberOfNodes;l++)
	    {
	      	pop3_ptr->ind[k].hubList[l]=globalpop.hubList[i][l];
	    	pop3_ptr->ind[k].assignList[l]=globalpop.assignList[i][l];
	    }
	    hubnum3=findHubNumber(globalpop.hubList[i]);
	    	pop3_ptr->ind[k].hubIndexList = (int *) malloc(hubnum3 * sizeof(int));
	    for(l=0;l<hubnum3;l++){
			pop3_ptr->ind[k].hubIndexList[l]=globalpop.hubIndexList[i][l];}
	 }
	}

      if(globalpop.flag[i] == 1)
	{
	  for(j = 0;j < nfunc;j++){
	    pop3_ptr->ind[k].fitness[j] = globalpop.fitness[i][j];}
	  pop3_ptr->ind[k].cub_len = globalpop.cub_len[i];
	  pop3_ptr->ind[k].rank = globalpop.rank[i];
	  k++;  // increment the pop3 counter*/
	}
    }

  pop3_ptr->maxrank = Lastrank;

  return;
}

void grank(int gen)
{
  int i,j,k,rnk,val,nondom,popsize1,gflg[2*maxpop],q;
  float *ptr1,*ptr2;
  FILE *gr;
  gr = fopen("g_rank_record.out","a");
  fprintf(gr,"Generation no. = %d\n",gen);
  /*----------------------------* RANKING *---------------------------------*/
  rnk = 0;
  nondom = 0;
  popsize1 = 2*popSize;

  for(i = 0;i < popsize1;i++)
    {
      gflg[i] = 2;
    }

  for(k = 0;k < popsize1;k++)
    {
      q =  0;
      for(j = 0;j < popsize1;j++)
	{
	  if (gflg[j] != 1) break;
	}
      if(j == popsize1) break;
      rnk = rnk +1;
      for( j = 0 ;j < popsize1; j++)
	{
	  if(gflg[j] == 0) gflg[j] = 2;
	}
      for(i = 0;i < popsize1 ; i++)
	{
	  if(gflg[i] != 1 && gflg[i] != 0)
	    {
	      ptr1 = &(global_pop_ptr->fitness[i][0]);
	      for(j = 0;j < popsize1 ; j++)
		{
		  if( i!= j)
		    {
		      if(gflg[j] != 1)
			{
			  ptr2 = &(global_pop_ptr->fitness[j][0]);
			  val = indcmp1(ptr1,ptr2);
			  if( val == 2)
			    {
			      gflg[i] = 0;/* individual 1 is dominated */
			      break;
			    }
			  if(val == 1)
			    {
			      gflg[j] = 0;/* individual 2 is dominated */
			    }
			  if(val == 3)
			    {
			      nondom++;/* individual 1 & 2 are non dominated */
			      if(gflg[j] != 0)gflg[j] = 3;
			    }
			}
		    }
		}
	      if( j == popsize1)
		{
		  global_pop_ptr->rank[i] = rnk;
		  gflg[i] = 1;
		  global_pop_ptr->rankar[rnk-1][q] =  i;
		  q++;
		}
	    }
	}
      global_pop_ptr->rankno[rnk-1] = q;
    }
  global_pop_ptr->maxrank = rnk;

  fprintf(gr,"   RANK     No Of Individuals\n");
  for(i = 0;i < rnk;i++)
    fprintf(gr,"\t%d\t%d\n",i+1,globalpop.rankno[i]);

  fclose(gr);
  return;
}


int indcmp1(float *ptr1,float *ptr2)
{
  float fit1[nfunc],fit2[nfunc];
  int i,value,m,n;
  for(i = 0;i < nfunc ;i++)
    {
      fit1[i] = *ptr1++;
      fit2[i] = *ptr2++;
    }
  m = 0;n=0;
  while(m < nfunc && fit1[m] <= fit2[m])
    {
      if((fit2[m] - fit1[m]) < 1e-7) n++;
      m++;
    }
  if(m == nfunc)
    {
      if(n == nfunc) value = 3;
      else value = 1;                    /*value = 1 for dominating*/
    }
  else
    {
      m = 0;n = 0;
      while(m < nfunc && fit1[m] >= fit2[m])
	{
	  if((fit1[m] - fit2[m]) < 1e-7) n++;
	  m++;
	}
      if(m == nfunc)
	{
	  if(n != nfunc)
	    value = 2;                       /*value =  2 for dominated */
	  else value = 3;
	}
      else value = 3;                   /*value = 3 for incomparable*/
    }
  return value;
}


/* This is the file used to sort the dummyfitness arrays */
void gsort(int rnk,int sel)
{
  int i,j,a,q;
  float array[2*maxpop][2],temp,temp1;

  q = globalpop.rankno[rnk-1];

  for(i = 0 ;i < q ;i++)
    {
      array[i][0] = globalpop.rankar[rnk-1][i];
      a = globalpop.rankar[rnk-1][i];
      array[i][1] = globalpop.cub_len[a];
    }
  for(i = 0;i < q ;i++)
    {
      for(j = i+1;j < q;j++)
	{
	  if(array[i][1] < array[j][1])
	    {
	      temp = array[i][1];
	      temp1 = array[i][0];
	      array[i][1] = array[j][1];
	      array[i][0] = array[j][0];

	      array[j][1] = temp;
	      array[j][0] = temp1;
	    }
	}
    }

  for(i = 0;i < sel;i++)
    {
      a = array[i][0];
      globalpop.flag[a] = 1;
    }
  return;
}

/*========================================================================*/

void gshare(int rnk)
{
  float length[2*maxpop][2],max;
  int i,j,m1,a ;
  float min, Diff;  // Added 18.08.2003

  m1 = globalpop.rankno[rnk-1];

  for(j = 0;j < nfunc;j++)
    {
      for(i = 0;i < m1;i++)
	{
	  fpara1[i][0] = 0;
	  fpara1[i][1] = 0;
	}

      for(i = 0;i < m1;i++)
	{
	  a = globalpop.rankar[rnk-1][i];
	  fpara1[i][0] = (float)a ;
	  fpara1[i][1] = globalpop.fitness[a][j];
	}

      sort(m1); /*Sort the arrays in ascending order of the fitness*/

      max = fpara1[m1-1][1];
      min = fpara1[0][1];  // Added 18.08.2003
      Diff = max-min;      // Added 18.08.2003 and 5 subsequent lines
      if (Diff < 0.0)
	{
	  printf("Something wrong in keepaliven.h\n");
	  exit(1);
	}
      for(i = 0;i < m1;i++)
	{
	  if(i == 0 ||i == (m1-1))
	    {
	      length[i][0] = fpara1[i][0];
	      length[i][1] = 100*max;
	    }
	  else
	    {
	      length[i][0] = fpara1[i][0];
	      length[i][1] = fabs(fpara1[i+1][1]- fpara1[i-1][1])/Diff; // crowding distances are normalized 18.08.2003
	    }
	}
      for(i = 0;i < m1;i++)
	{
	  a = length[i][0];
	  globalpop.cub_len[a] += length[i][1];
	}
    }

  return;
}


void sort(int m1)
{
  float temp,temp1;
  int i1,j1,k1;
  for(k1 = 0;k1 < m1-1;k1++)
    {
      for(i1 = k1+1;i1 < m1;i1++)
	{
	  if(fpara1[k1][1] > fpara1[i1][1])
	    {
	      temp = fpara1[k1][1];
	      temp1 = fpara1[k1][0];
	      fpara1[k1][1] = fpara1[i1][1];
	      fpara1[k1][0] = fpara1[i1][0];
	      fpara1[i1][1] = temp;
	      fpara1[i1][0] = temp1;
	    }
	}
    }
  return;
}
