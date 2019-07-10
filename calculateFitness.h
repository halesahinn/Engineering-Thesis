void calculateFitness(population *pop_ptr);

void calculateFitness(population *pop_ptr)
{
    int pc=0,hubLength,i,k,m,hub;
    int *hubList,*hubIndexList,*assignList;

    float fitness1, fitness2, fitness3,maxService;
    for(pc=0; pc<popSize; pc++) //pc stands for population counter
    {
		fitness1=0.0;
        fitness2=0.0;
        fitness3=0.0;
        maxService=0.0;
        pop_ptr->ind[pc].fitness[0]=0.0;//total fixedCost
        pop_ptr->ind[pc].fitness[1]=0.0;//total time
        pop_ptr->ind[pc].fitness[2]=0.0;//max time
        
        hubLength=findHubNumber(pop_ptr->ind[pc].hubList);
        assignList = pop_ptr->ind[pc].assignList;
        hubList=pop_ptr->ind[pc].hubList;
        hubIndexList=pop_ptr->ind[pc].hubIndexList;

//------------------------------- TOTAL fixedCost CALCULATIONS ARE STARTING -------------------------------------------------
        int j;
        float sum2, tempsum;
        fitness1=0.0;
        for (i=0; i<numberOfNodes; i++)
        {
            tempsum=0.0;
            for (k=0; k<numberOfNodes; k++)
            {
                if (assignList[i]==k)
                {
                    tempsum += C[i][k];
                    break;
                }
            }
            fitness1+= tempsum*(O[i]+D[i]);
        }
        sum2=0.0;
        for (j=0; j<numberOfNodes; j++)
        {
            for (m=0; m<numberOfNodes; m++)
            {
                if (assignList[j]==m)
                {
                    for (i=0; i<numberOfNodes; i++)
                    {
                        tempsum=0.0;
                        for (k=0; k<numberOfNodes; k++)
                        {
                            if (assignList[i]==k)
                            {
                                tempsum+=C[k][m];
                                break;
                            }
                        }
                        sum2+=tempsum*W[i][j];
                    }
                    break;
                }
            }
        }
        fitness1+=sum2;
        for (i=0; i<numberOfNodes; i++)
        {
            if (hubList[i]==1)
                fitness1+=fixedCost[i];
        }

        //printf("total fixedCost toplam 3 = %lf\n", fitness1);


//------------------------------- TOTAL TIME CALCULATIONS ARE STARTING -------------------------------------------------

        for(i=0; i<numberOfNodes; i++)    // time'ın ilk toplamı
        {
            for(k=0; k<numberOfNodes; k++)
            {
                if(assignList[i]==k)
                {
                    fitness2+=O[i]*Tk[k];
                }
            }
            if(hubList[i]==1)  //is i a Hub?
            {
                fitness2+=Pk[i];
            }
        }


//------------------------------- MAX SERVICE TIME CALCULATIONS ARE STARTING -------------------------------------------------

        maxService=0.0;
        for(k=0; k<numberOfNodes; k++)
        {
            for(i=0; i<numberOfNodes; i++)
            {
                if(assignList[i]==k)
                {
                    fitness3+=O[i]*Tk[k];
                }
                if(hubList[k]==1) //is k a hub?
                {
                    fitness3+=Pk[k];
                }
                if(fitness3 > maxService)
                {
                    maxService=fitness3;
                }
                fitness3=0.0;
                //printf("fitness3 %lf for pop %d\n",fitness3,pc);
            }


        }
        pop_ptr->ind[pc].fitness[0]=fitness1;//total fixedCost
        pop_ptr->ind[pc].fitness[1]=fitness2;//total time
        pop_ptr->ind[pc].fitness[2]=maxService;//max time

        

    }

}
