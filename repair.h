#include <math.h>
population repair(population newpop);

population repair(population newpop)
{
	int i,j,hubnum,hubnum2,hubnum3,open,close,max,assigned,rndhub,*hublist,*hublist2;
	float rnd,rnd2,rnd3;
	i=0;
	max=(int)(numberOfNodes/4);
	//repair hub number
	while(i<popSize){
		hublist=newpop.ind[i].hubList;
		hubnum=findHubNumber(hublist);
		//printf("\nind num: %d hub num : %d, min 2 max : %d\n",i,hubnum,max);
		//hub  number less than 2
		if(hubnum<2){
			do{
			rnd = randomperc();
			open = floor(rnd*(numberOfNodes+10));
            if(open >= numberOfNodes){
                open = open/2;}
			}while(hublist[open]==1);//find a nonhub not 1
			//printf("\nhub açma point : %d\n",open);
		newpop.ind[i].hubList[open]=1;//turn that point a hub to 1
		
		}else if(hubnum>max){
			do{
			rnd2 = randomperc();
			close = floor(rnd2*(numberOfNodes+10));
            if(close >= numberOfNodes){
                close = close/2;}
			}while(hublist[close]==0);//find a hub not 0
		newpop.ind[i].hubList[close]=0;	//turn that to nonhub to 0
		//printf("\nhub kapama point : %d\n",close);
		}else{
			hubnum2=findHubNumber(hublist);
			newpop.ind[i].hubIndexList=initialize_hubIndexList(hublist,hubnum2);
			i++;
		}
	}//end pop loop for hub number repair
	
	
	//repair assign list
	for(i=0;i<popSize;i++){
		
		for(j=0;j<numberOfNodes;j++){
			hublist2=newpop.ind[i].hubList;
			assigned=newpop.ind[i].assignList[j];
			if(hublist2[assigned]!=1){
				hubnum3=findHubNumber(hublist2);
				rnd3 = randomperc();
				rndhub=(int)(rnd3*hubnum3);
				newpop.ind[i].assignList[j]=newpop.ind[i].hubIndexList[rndhub];
			}
		}
	}//end pop loop for assign list repair	
	 
	 return newpop;

}

