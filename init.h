/*This is the file which initializes the population*/
void init(population *pop_ptr);

int *initialize_assignList(int* hubList,int* hubIndexList)
{

    int *assignList;
    int hubNumber = 0; //for counting hub number
    int x,i;

    assignList = (int*) malloc(numberOfNodes*sizeof(int));
    if(assignList == NULL)
    {
        printf("Error. The array couldn't be allocated.\n");
        exit(1);
    }

    //assignin 0 default
    for(i=0 ; i<numberOfNodes ; i++)
    {
        assignList[i] = -1;
    }

    //hub olanlarÄ± assign dizisinde kendini atama
    for(i=0; i<numberOfNodes ; i++)
    {
        if(hubList[i] == 1)
        {
            assignList[i] = i;
            hubNumber++;
        }
    }
    for(i=0 ; i<numberOfNodes ; i++)
    {
        if (assignList[i] == -1)
        {
            x = (rand() % hubNumber);
            assignList[i] = hubIndexList[x];
        }
    }
    return assignList;

}

int *initialize_hubList(int numberOfNodes)
{
    int *hubList;
    int numOfHubs;
    int index;
    int k=0,i,j;

    //the hubList array is allocated dynamically
    hubList = (int*) malloc(numberOfNodes*sizeof(int));
    if(hubList == NULL)
    {
        printf("Error. The array couldn't be allocated.\n");
        exit(1);
    }

    for(i=0 ; i<numberOfNodes ; i++)
    {
        hubList[i]=0;
    }

    do                               
    {
        numOfHubs =(int) ceil( (int)(numberOfNodes/4) * (double) rand() / (double) RAND_MAX);
    }
    while(numOfHubs<2); //hub number cant be less than 2
    //numOfHubs=1;
    for(j=0; j<numOfHubs; j++)
    {
        do
        {
            index =(int) floor((numberOfNodes-1) * (double) rand() / (double) RAND_MAX);
            //printf("index:  %d\n",index);
        }
        while(hubList[index] == 1);
        hubList[index] = 1;
    }

    return hubList;

}


int *initialize_hubIndexList(int* hubList,  int hubNumber)
{

    int *hubIndexList;
    int n = 0,i;

    //allocating array
    hubIndexList = (int*) malloc(hubNumber*sizeof(int));
    if(hubIndexList == NULL)
    {
        printf("Error. The array couldn't be allocated.\n");
        exit(1);
    }

    for(i=0; i<numberOfNodes ; i++)
    {
        if(hubList[i] == 1)
        {
            hubIndexList[n] = i;
            n++;
        }
    }

    return hubIndexList;
}

void init(population *pop_ptr)
{
    int *hubList;
    int *assignList;
    int *hubIndexList;
    int hubNumber,i;
    pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

    /*Loop Over the population size*/
    for (i = 0 ; i < popSize ; i++)
    {

        hubList=initialize_hubList(numberOfNodes);
        hubNumber=findHubNumber(hubList);
        hubIndexList = initialize_hubIndexList(hubList, hubNumber);
        assignList=initialize_assignList(hubList,hubIndexList);

        pop_ptr->ind_ptr->hubList = hubList;
        pop_ptr->ind_ptr->hubIndexList = hubIndexList;
        pop_ptr->ind_ptr->assignList = assignList;

        pop_ptr->ind_ptr = &(pop_ptr->ind[i+1]);

    }
}
