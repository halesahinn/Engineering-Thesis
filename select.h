/*This is the file to get the different individuals selected*/

population nselect(population oldpop);

population nselect(population oldpop)
{

    float rnd2,cub1,cub2,*f1,*f2;
    int c,i,l,rnd,rnd1,k,n,m1,m2,rank1,rank2;
	population newpop;
	

    for(n = 0,k = 0; n < popSize; n++,k++)
    {	
    	
		newpop.ind[k].hubList = (int *) malloc(numberOfNodes * sizeof(int));
		newpop.ind[k].assignList = (int *) malloc(numberOfNodes * sizeof(int));
        rnd2 = randomperc();
        rnd2 = popSize* rnd2;
        rnd = floor(rnd2);

        if(rnd == 0)
            rnd = popSize - k;

        if(rnd == popSize)
            rnd = (popSize-2)/2;

        m1 = rnd-1;

        rnd2 = randomperc();

        rnd2 = popSize * rnd2;

        m2 = floor(rnd2);

        if (m2 == 0)
            m2 = popSize - n;

        if(m2 == popSize)
            m2 = (popSize - 4)/2;
		m2=m2-1;

        /*------------------SELECTION PROCEDURE------------------------------------*/
		rank1 = oldpop.ind[m1].rank;
		rank2 = oldpop.ind[m2].rank;
		

		
		cub1 = oldpop.ind[m1].cub_len;
		cub2 = oldpop.ind[m2].cub_len;
        /*Comparing the fitnesses*/
        if(rank1 > rank2)
        {	
        	ptr=(char *)memcpy(newpop.ind[k].hubList,oldpop.ind[m2].hubList,sizeof(int)*numberOfNodes);
        	newpop.ind[k].hubIndexList = (int *) malloc(findHubNumber(newpop.ind[k].hubList) * sizeof(int));
        	ptr=(char *)memcpy(newpop.ind[k].hubIndexList,oldpop.ind[m2].hubIndexList,sizeof(int)*findHubNumber(newpop.ind[k].hubList));
        	ptr=(char *)memcpy(newpop.ind[k].assignList,oldpop.ind[m2].assignList,sizeof(int)*numberOfNodes);
        	for(l = 0 ; l < nfunc ;l++){
	    		newpop.ind[k].fitness[l] = oldpop.ind[m2].fitness[l];}
	    	newpop.ind[k].cub_len=oldpop.ind[m2].cub_len;
		    newpop.ind[k].rank=oldpop.ind[m2].rank;
        }
        else
        {
            if(rank1 < rank2)
            {
                ptr=(char *)memcpy(newpop.ind[k].hubList,oldpop.ind[m1].hubList,sizeof(int)*numberOfNodes);
                newpop.ind[k].hubIndexList = (int *) malloc(findHubNumber(newpop.ind[k].hubList) * sizeof(int));
        		ptr=(char *)memcpy(newpop.ind[k].hubIndexList,oldpop.ind[m1].hubIndexList,sizeof(int)*findHubNumber(newpop.ind[k].hubList));
        		ptr=(char *)memcpy(newpop.ind[k].assignList,oldpop.ind[m1].assignList,sizeof(int)*numberOfNodes);
        		for(l = 0 ; l < nfunc ;l++){
	    				newpop.ind[k].fitness[l] = oldpop.ind[m1].fitness[l];}
	    		newpop.ind[k].cub_len=oldpop.ind[m1].cub_len;
		    	newpop.ind[k].rank=oldpop.ind[m1].rank;
            }
            else
            {
                if(cub1 < cub2)
                {
					ptr=(char *)memcpy(newpop.ind[k].hubList,oldpop.ind[m2].hubList,sizeof(int)*numberOfNodes);
					newpop.ind[k].hubIndexList = (int *) malloc(findHubNumber(newpop.ind[k].hubList) * sizeof(int));
        			ptr=(char *)memcpy(newpop.ind[k].hubIndexList,oldpop.ind[m2].hubIndexList,sizeof(int)*findHubNumber(newpop.ind[k].hubList));
        			ptr=(char *)memcpy(newpop.ind[k].assignList,oldpop.ind[m2].assignList,sizeof(int)*numberOfNodes);
		        	for(l = 0 ; l < nfunc ;l++){
	    				newpop.ind[k].fitness[l] = oldpop.ind[m2].fitness[l];}
	    			newpop.ind[k].cub_len=oldpop.ind[m2].cub_len;
		    		newpop.ind[k].rank=oldpop.ind[m2].rank;
                }
                else
                {
	                ptr=(char *)memcpy(newpop.ind[k].hubList,oldpop.ind[m1].hubList,sizeof(int)*numberOfNodes);
	                newpop.ind[k].hubIndexList = (int *) malloc(findHubNumber(newpop.ind[k].hubList) * sizeof(int));
        			ptr=(char *)memcpy(newpop.ind[k].hubIndexList,oldpop.ind[m1].hubIndexList,sizeof(int)*findHubNumber(newpop.ind[k].hubList));
        			ptr=(char *)memcpy(newpop.ind[k].assignList,oldpop.ind[m1].assignList,sizeof(int)*numberOfNodes);
	        		for(l = 0 ; l < nfunc ;l++){
	    				newpop.ind[k].fitness[l] = oldpop.ind[m1].fitness[l];}
	    			newpop.ind[k].cub_len=oldpop.ind[m1].cub_len;
		    		newpop.ind[k].rank=oldpop.ind[m1].rank;
                }
            }
        }
    }

    return newpop;
}
