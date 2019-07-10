/*This program subroutine is used to print the report*/

void report(int t,population pop1, population pop2,FILE *rep_ptr,FILE *gen_ptr);

void report(int t,population pop1,population pop2,FILE *rep_ptr,FILE *gen_ptr )
{
  int i,j,*be,*rptr,*rptr1,*ptr1_b, *ptr2_b,*ptr1_a, *ptr2_a; 

  float *ptr1,*ptr,*fptr,*fptr1;

  float *ptr2,*cons_ptr1,*cons_ptr2, *err2;
  
  int *gene_ptr1 ,*gene_ptr2 ;

  fprintf(rep_ptr,"\n\n---------------------------------------------------\n");
  fprintf(rep_ptr,"Generation No.     ->%d\n",t+1);
  fprintf(rep_ptr,"------------------------------------------------------\n");
  fprintf(rep_ptr," variables (Hub List %d)	(Assign List %d)  (Fitness %d)  (rank) (cublen) || variables (Hub List %d)	(Assign List %d)  (Fitness %d)  (rank) (cublen)\n",numberOfNodes,numberOfNodes,nfunc,numberOfNodes,numberOfNodes,nfunc);


  for(i = 0;i < popSize;i++)
    {
      fprintf(rep_ptr,"\n------------------------------------------------\n"); 

      for(j = 0;j < numberOfNodes;j++)
		fprintf(rep_ptr,"%d ",pop1.ind[i].hubList[j]);
		fprintf(rep_ptr,"\t ");
      for(j = 0;j < numberOfNodes; j++)
		fprintf(rep_ptr,"%d ",pop1.ind[i].assignList[j]);

      
      for(j = 0;j < nfunc;j++)
	fprintf(rep_ptr,"  %.4f",pop1.ind[i].fitness[j]);
      
      fprintf(rep_ptr," %d ",pop1.ind[i].rank);
      
      fprintf(rep_ptr,"%f ",pop1.ind[i].cub_len);
      fprintf(rep_ptr,"|**|");

      for(j = 0;j < numberOfNodes;j++)
	{
	  fprintf(rep_ptr," %d ",pop2.ind[i].hubList[j]);
	  fprintf(gen_ptr," %d ",pop2.ind[i].hubList[j]);
	}
	fprintf(rep_ptr,"\t ");
	fprintf(gen_ptr,"\t ");
      for(j = 0;j < numberOfNodes; j++)
	{
	  fprintf(rep_ptr,"%d ",pop2.ind[i].assignList[j]); 
	  fprintf(gen_ptr,"%d ",pop2.ind[i].assignList[j]);
	}
      for(j = 0;j < nfunc;j++)
	{	
	  fprintf(rep_ptr,"  %f",pop2.ind[i].fitness[j]);
	  fprintf(gen_ptr,"  %f",pop2.ind[i].fitness[j]);
	}
      fprintf(rep_ptr," %d ",pop2.ind[i].rank);
      
      
      fprintf(rep_ptr," %f ",pop2.ind[i].cub_len);
      
      
      fprintf(gen_ptr,"\n");
    }
  
  fprintf(rep_ptr,"\n--------------------------------------------------\n\n"); 
  fprintf(rep_ptr,"-------------------------------------------------------\n");
  fprintf(gen_ptr,"\n--------------------------------------------------\n\n");
  return;
}






