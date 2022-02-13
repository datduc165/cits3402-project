/*CITS3402 Project2 
Student name: Dat D Nguyen
Student number: 21909182
gcc -std=c99 -Wall -pedantic -o  pro2 proj2.c
*/
// C code to implement Priority Queue 
// using Linked List 
#include <time.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h> 

/*GLOBAL VARIABLE HERE*/
int c;
int *adj_graph;//the graph represent adjadency relation of each node
int num_node;//number of vertices
size_t graph_size;
int *node_handle;//store number of vertex need to be handled by each process
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
    printf("%d \n",num_node);
    
    adj_graph= (int *)malloc(sizeof(int)*(num_node*num_node));
    if(adj_graph==NULL){printf("could not mallocate memory\n");}
    
    //printf( "hope everything ok\n");

    fread(adj_graph,sizeof(int),num_node*num_node,fp);
    graph_size=num_node*num_node;
 /* for (int i=0; i<graph_size;i++){
      printf( "hope everything ok adj_graph[%d]: %d\n",i,adj_graph[i]);
  }*/
    //free(adj_graph);
    //adj_graph=NULL;//stop  adj_graph
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
        //printf("head.data: %d\n",(*head)->data);
        (*head) = temp; 
        //printf("---head.data: %d\n",(*head)->data);
        //printf("---head.next.data: %d\n",(*head)->next->data);
    } 
    else { 
  
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && start->next->priority < p) { 
           // printf("start: %d\n",start->data);
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
  
// A utility function to print the constructed distance array 
void printSolution(int *dist) 
{ 
    printf("Vertex \t\t Distance from Source\n"); 
    for (int i = 0; i < num_node; i++) 
        {printf("%d \t\t %d\n", i, dist[i]); }

} 
  
// Function that implements Dijkstra's single source shortest path algorithm 
// for a graph represented using adjacency matrix representation 
void dijkstra(int *graph, int src) 
{ 
    int *dist; // The output array.  dist[i] will hold the shortest 
    dist = (int *)malloc(sizeof(int)*(num_node));
    // distance from src to i 
    //need to malloc in size of num_node and then free
    if(dist==NULL){printf("can't allocate memory for dist\n");}
    int *seen; // sptSet[i] will be true if vertex i is included in shortest 
    seen=(int *)malloc(sizeof(int)*(num_node));
    if(seen==NULL){printf("can't allocate memory for seen\n");}
    // path tree or shortest distance from src to i is finalized 
    //need to malloc in size of num_node and then free
    // Initialize all distances as INFINITE and stpSet[] as false 
    for (int i = 0; i < num_node; i++) 
        {
            //dist[i] = INT_MAX
            seen[i] = -1; 
        }
    // Distance of source vertex from itself is always 0 
    dist[src] = 0; 

    Node* pq = newNode(src, 0); 
    push(&pq,1,INT_MAX);
    // Find shortest path for all vertices 
    int count1=0;
    while (!isEmpty(&pq)) { 
        //printf("doing djkstra now.....\n");
        //printf("number of time of checking %d\n",count1);
        //count1++;
        // Pick the minimum distance vertex from the set of vertices not 
        // yet processed. u is always equal to src in the first iteration. 
        Node curr = peek(&pq);
        pop(&pq); 
        // Mark the picked vertex as processed,seen
        if(seen[curr.data]==-1){
            seen[curr.data] = 1;
           // printf("???curr.data %i\n",curr.priority);
            //printf("???curr.priority %i\n",curr.priority);
            dist[curr.data]=curr.priority;
        }
        // Update dist value of the adjacent vertices of the picked vertex. 
        //printf("the number of vertex is: %d\n",num_node);
        for (int v = 0; v < num_node; v++) {
            // printf("doing checking adj vertex.....\n");
             
            // Update dist[v] only if is not in sptSet, there is an edge from 
            // u to v, and total weight of path from src to  v through u is 
            // smaller than current value of dist[v] 
            if (seen[v]==-1 && graph[curr.data*num_node+v]>0){
                //printf("found an unseen vertex\n");
                int cost=graph[curr.data*num_node+v];
                //printf("the cost is: %d\n",cost);
                if(!isEmpty(&pq)){
                    //printf("pq is not empty so push new node\n");
                    push(&pq,v,cost+curr.priority);
                }
               // printf("should pushed new node\n");
                //dist[v] = cost+curr.priority; 
            }
        }
    } 
  
    // print the constructed distance array 
    printSolution(dist); 
    free(pq);
    if(dist){free(dist); printf("free dist...");}
    if(seen){free(seen);printf("free seen...");}
} 

int main ( int argc, char *argv[] )
{
	
	if(argc==1){printf("there is no operation command\n");}
    if(argc==2){

        int haha=readfile(argv[1]);
        printf("%d the value of readfile return\n",haha);
        clock_t t; 
    t = clock(); 
        dijkstra(adj_graph, 0);
        dijkstra(adj_graph, 1);
        dijkstra(adj_graph, 2);
        dijkstra(adj_graph, 3);
          t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("fun() took %f seconds to execute \n", time_taken); 
        if(adj_graph){free(adj_graph);printf("free adj_graph\n");}
    }
   
    return 0;

}