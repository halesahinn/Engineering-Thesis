/*This is the file for formulating the crossover process*/

population crossover(population matepop) ;

population crossover(population matepop)
{
    int i,j,k,l,r,m,n,y,p1,p2,c1,c2,jCross,hubnum,hubnum2,a,b,c,*c1_hubList,*c2_hubList;
    float rnd;
    population newpop;
    rnd=randomperc();

    for (i = 0,y = 0,n = 0; i < popSize/2;i++)
    {
		p1=y;
		p2=y+1;
		c1=n;
		c2=n+1;
        rnd = randomperc();
	        newpop.ind[c1].assignList=matepop.ind[p1].assignList;
	        newpop.ind[c1].hubList=matepop.ind[p1].hubList;
	        newpop.ind[c2].assignList=matepop.ind[p2].assignList;
	        newpop.ind[c2].hubList=matepop.ind[p2].hubList;
        if (rnd < pcross)
        {
            rnd = randomperc();
			c = floor(rnd*(numberOfNodes+10));
            jCross=c;
            if(jCross >= numberOfNodes){
                jCross = jCross/2;
            }
	        //printf("\n\nCross edilen %d %d, %d %d\nCross Point : %d\n\n",p1,p2,c1,c2,jCross);			
	        for (j=jCross; j<numberOfNodes; j++){
	        	a=matepop.ind[p1].hubList[j];
	        	newpop.ind[c1].hubList[j] = matepop.ind[p2].hubList[j];
				newpop.ind[c2].hubList[j] = a;
				b=matepop.ind[p1].assignList[j];
				newpop.ind[c1].assignList[j] = matepop.ind[p2].assignList[j];
				newpop.ind[c2].assignList[j] = b;	
			}
		}
			c1_hubList=newpop.ind[c1].hubList;
	        hubnum=findHubNumber(c1_hubList);
			newpop.ind[c1].hubIndexList=initialize_hubIndexList(c1_hubList,hubnum);
			c2_hubList=newpop.ind[c2].hubList;
	        hubnum2=findHubNumber(c2_hubList);
			newpop.ind[c2].hubIndexList=initialize_hubIndexList(c2_hubList,hubnum2); 
			for(l = 0 ; l < nfunc ;l++){
	    		newpop.ind[c1].fitness[l] = 0.0;
				newpop.ind[c2].fitness[l] = 0.0;}   
	    y=y+2;
		n=n+2;
		hubnum=0;
		hubnum2=0;
    }
    
    //print_pop(newpop);
    return newpop;
}

