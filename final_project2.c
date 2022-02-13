/*CITS3402 Project2 
Student name: Dat D Nguyen
Student number: 21909182
mpicc -std=c99 -Wall -pedantic -o  pro2 proj2.c
*/
// C code to implement Priority Queue 
// using Linked List 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h> 
//#include <mpi.h>
#include <math.h>
/*GLOBAL VARIABLE HERE*/
int c;
int *adj_graph;//the graph represent adjadency relation of each node
int num_node;//number of vertices
size_t graph_size;
int *node_handle;//store number of vertex need to be handled by each process
int **node;
int mpisize;
int *rev_count;
int *rev_dis;
int *part_dist;
int *final_dist;
//int index=1;
int rank;
/*END of GLOBAL VARIABLE*/


int readfile(const char *filename)
{
    FILE *fp = fopen ( filename, "rb" );
	if ( fp == NULL )
   	{
     		printf ( "Could not open filename: %s\n",filename ) ;
     		return 0;//can't open file???Wrong name???
   	}
    int num;

    fread(&num,sizeof(int),1,fp);
    
    num_node=num;
   
    adj_graph= (int *)malloc(sizeof(int)*(num_node*num_node));
    if(adj_graph==NULL){printf("could not mallocate memory\n");}
    
    fread(adj_graph,sizeof(int),num_node*num_node,fp);
    graph_size=num_node*num_node;
 
  
    fclose(fp);
    return 1;
}

  
typedef struct node { 
    int data; 
  
    // Lower values indicate higher priority 
    int priority; 
  
    struct node* next; 
    
} Node; 
  
// Function to Create A New Node 
Node* newNode(int d, int p) 
{ 
    Node* temp = (Node*)malloc(sizeof(Node)); //should be freed when called
    temp->data = d; 
    temp->priority = p; 
    temp->next = NULL; 
    //we must ise -> here because temp is a pointer of type struct Node
    return temp; 
} 
  
// Return the value at head 
Node peek(Node** head) 
{ 
    return (**head); 
} 
  
// Removes the element with the 
// highest priority form the list 
void pop(Node** head) 
{ 
    Node* temp = *head; 
    (*head) = (*head)->next; 
    free(temp); 
} 
  
// Function to push according to priority 
void push(Node** head, int d, int p) 
{ 
    Node* start = (*head); 
  
    // Create new Node 
    Node* temp = newNode(d, p); 
  
    // Special Case: The head of list has lesser 
    // priority than new node. So insert new 
    // node before head node and change head node. 
    if ((*head)->priority > p) { //smaller value of priority mean higher priority
  
        // Insert New Node before head 
        temp->next = *head; 
        
        (*head) = temp; 
       
    } 
    else { 
  
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && start->next->priority < p) { 
           
            start = start->next; 
        } 
  
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 

        start->next = temp; 
      
    } 
} 
  
// Function to check is list is empty 
int isEmpty(Node** head) 
{ 
    return (*head) == NULL; 
}
  
// Function that implements Dijkstra's single source shortest path algorithm 
// for a graph represented using adjacency matrix representation 
void dijkstra(int *graph,int num, int src) 
{ 
   
	
    int *seen; // seen will be 1 if vertex i is included in shortest 
    seen=(int *)malloc(sizeof(int)*(num));
    if(seen==NULL){printf("can't allocate memory for seen\n");}
   
    for (int i = 0; i < num; i++) 
        {
            
	    part_dist[i]=-1;
            seen[i] = -1; 
        }
    // Distance of source vertex from itself is always 0 
    
    part_dist[src]=0;

    Node* pq = newNode(src, 0); 
    push(&pq,1,INT_MAX);
    //push(&pq,1,INT_MAX);//the implementation of priority queue does not handle to push a node into an empty priority queue.
    // Find shortest path for all vertices 
    int count1=0;
    while (!isEmpty(&pq)) { 
      
        count1++;
        
        Node curr = peek(&pq);
        pop(&pq); 
        // Mark the picked vertex as processed,seen
        if(seen[curr.data]==-1){
            printf("seen at [%i] is: %i\n",curr.data,seen[curr.data]);
            seen[curr.data] = 1;
             printf("seen after [%i] is: %i\n",curr.data,seen[curr.data]);
           printf("^^^^%i^^^^^ %d\n",curr.data,curr.priority);
	    part_dist[curr.data]=curr.priority;
	    	
        }
        // Update dist value of the adjacent vertices of the picked vertex. 
        
        for (int v = 0; v < num; v++) {
            
            if (seen[v]==-1 && graph[curr.data*num+v]>0){
               //printf(" graph at curr.data*num+v[%i] :%i\n",curr.data*num+v,graph[curr.data*num+v]);
                int cost=graph[curr.data*num_node+v];
           // printf("the fuck %d\n",cost);
                push(&pq,v,cost+curr.priority);
               
            }
        }
    } 
 
    free(pq);
    
    if(seen)
    {
        free(seen);//printf("free seen...\n");
    }
	
} 
void handle(int num){
	node_handle = (int*)calloc(mpisize,sizeof(int));
	for(int i=0; i<num; i++)	{
		node_handle[i%mpisize]++;
	}

	node = (int **)malloc(sizeof(int *)*num);

	rev_count = (int*)calloc(mpisize,sizeof(int));
	rev_dis = (int*)calloc(mpisize,sizeof(int));

	size_t c =0;
	for(int i=0; i<mpisize;i++){
		
		rev_count[i]=node_handle[i]*num;
	
		node[i] = (int *)malloc(sizeof(int)*node_handle[i]);
		for(int j=0; j<node_handle[i]; j++){
			node[i][j]=c;
		
			c++;
		}
	}
	rev_dis[0]=0;
	for(int i=1; i<mpisize;i++){
		rev_dis[i]=rev_dis[i-1]+rev_count[i-1];		
	}
	
}
void printSolution(int *dist) 
{ 
    printf("Vertex \t\t Distance from Source\n"); 
    for (int i = 0; i < num_node; i++) 
        {printf("%d \t\t %d\n", i, dist[i]); }

} 
int main ( int argc, char *argv[] )
{
	
	if(argc==1){printf("there is no operation command\n");}
	if(argc==2){

      
	/*MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
	rank=0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);*/
    //the purpose of using bcast is to run readfile on root processor only and then pass 1-d array adj_graph to all processors so that it avoid duplicating readfile function
	//if(rank==0){
	  int haha=readfile(argv[1]);
          printf("%d the value of readfile return\n",haha);
	//}
	//printf("[%d] the num_node before bcast: %d\n",rank,num_node);
	//MPI_Bcast(&num_node,1, MPI_INT,0,MPI_COMM_WORLD);
	//MPI_Bcast(adj_graph,graph_size, MPI_INT,0,MPI_COMM_WORLD);
	//printf("[%d] the num_node AFTER bcast: %d\n",rank,num_node);
	//MPI_Barrier(MPI_COMM_WORLD);

	//handle(num_node);

	//part_dist=(int*)calloc(rev_count[rank],sizeof(int));
    part_dist=(int*)calloc(num_node,sizeof(int));
    //parrallel here, each processor run dijkstra on different starting vertex
	//double start = MPI_Wtime();
	//for(index=0; index<node_handle[rank]; index++){
        	dijkstra(adj_graph, num_node,0);
            printSolution(part_dist);
	//}
	//double end = MPI_Wtime();

	//final_dist=(int*)calloc(num_node*num_node,sizeof(int));
	//MPI_Gatherv(part_dist,rev_count[rank],MPI_INT,final_dist,rev_count,rev_dis,MPI_INT,0,MPI_COMM_WORLD);
	/*if(rank==0){
		printf("%d ",num_node);
		for(int i=0; i<num_node*num_node;i++){

			printf("%i ",final_dist[i]);

		}
	}*/

	//free(final_dist);//printf("free final_dist\n");
	free(part_dist); printf("free part_dist\n");
	/*for(int i=0; i<mpisize;i++){
		//printf("free node[%i]\n",i);
		free(node[i]);
	}*/
	//free(node); //printf("free node\n");
	//free(rev_count); //printf("free rev_count\n");
	//free(rev_dis); //printf("free rev_dis\n");
	//printf("+++++++++the rank is: %d +++++++++\n",rank);
	
	//MPI_Finalize();
	
	//printf("the duration is: %f\n",end-start);
	if(adj_graph){
	free(adj_graph);//printf("free adj_graph\n");
	}
	
	if(node_handle!=NULL) {
	free(node_handle);//printf("free node_handle\n");
	}        
   }
    
    return 0;

}
