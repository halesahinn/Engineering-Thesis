/* This is the module used to formulate the mutation routine*/

population mutate(population newpop);             

population mutate(population newpop)
{
  int i,j,r,hubnum,*hubL,rnd4;
  float rand1,rand2,rnd3;

  rand1=randomperc();
  
  for(j = 0;j < popSize;j++)
    {   
       /*hub list mutation */
      for (i = 0;i < numberOfNodes;i++)
	{
	  rand1 = randomperc();
	    if(rand1 <= pmutation){/*Check whether to do mutation or not*/
	    //printf("\n\nYES, IND %d, POINT %d MUTATED\n\n",j,i);
	      if(newpop.ind[j].hubList[i] == 0)
		newpop.ind[j].hubList[i]=1;
	      else
		newpop.ind[j].hubList[i]=0;
	      //nmut++;
	    }
	}	
	
	
	       //assign list mutation 
      for (i = 0;i < numberOfNodes;i++)
	{
	  rand2 = randomperc();
	    if(rand2 <= pmutation){//Check whether to do mutation or not

		rnd3 = randomperc();
		rnd4=(int)(rnd3*numberOfNodes);
		newpop.ind[j].assignList[i]=rnd4;

	    }
	}	
	
	
	/*	hubL=newpop.ind[j].hubList;
		hubnum=findHubNumber(hubL);
		newpop.ind[j].hubIndexList=initialize_hubIndexList(hubL,hubnum);*/
    }
    	
  return newpop;
}
