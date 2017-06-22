// TSALESIS EVANGELOS
// AM: 1779

// It searches for the shortest path between two points S and G1 or G2 (whichever is closer)
// in a random NxN maze (p is the probability of an obstacle in a node), where only horizontal 
// (cost=0.5) and vertical (cost=1.0) movement is allowed. 
// In a second stage, it searches for the shortest path between G1 and G2 (or the opposite).

// The A* algorithm is used.

// Open List is a sorted linked list of nodes. 
// Head of the list is always the node with the smallest f value. 
// (f = distance from the start + heuristic (manhattan) distance from the end)

// Closed list is a NxN 2d array where the positions of closed nodes are stored.

// Heuristic values for every node are calculated at the beginning of the algorithm 
// and are stored in the 'heuristic' 2d array.



#include <stdio.h>  
#include <stdlib.h>
#include <time.h>


// NODE STRUCT
typedef struct node {
	int pos[2];
	float d;
	float f;
	struct node* parent;
	struct node* next;
} node;

// DEBUG
//void waitFor (unsigned int secs) {
//   unsigned int retTime = time(0) + secs;   // Get finishing time.
//    while (time(0) < retTime);               // Loop until it arrives.
//}


// PRINT MAZE
void printMaze(float** heuristic, int N, int* S, int* G1, int* G2, node* head, char** closedList){
	int i;
   	int j;
   	int k;

   	node* current = head;

	printf("     ");
    for (i=0; i<N; i++){
 		printf("%3d", i);
 	}
 	printf("\n");
    printf("     ┌");
 	for (i=0; i<N-1; i++){
 		printf("──┬");
 	}
 	printf("──┐");
 	printf("\n");
    for (i=0; i<N; i++){
    	printf("%5d", i);
    	printf("│");
    	for (j=0; j<N; j++){
    		if (heuristic[i][j] == -1){
    			printf("██");
    		}
    		else if (G1[0] == i && G1[1] == j){
    			printf("G1");
    		}
    		else if (G2[0] == i && G2[1] == j){
    			printf("G2");
    		}
    		else if (S[0] == i && S[1] == j){
    			printf("S ");
    		}
    		else{
    			if (closedList == NULL) {
	    			while (current != NULL){
	    				if (current->pos[0] == i && current->pos[1] == j){
	    					printf(" *");
	    					current = head;
	    					break;
	    				}
	    				current = current->parent;
	    			}
	    			if (current == NULL){
	    				printf("  ");
	    			}
	    			current = head;
	    		}
	    		else {
	    			if (closedList[i][j] == 1){
	    				printf(" *");
	    			}
	    			else {
	    				printf("  ");
	    			}
	    		}
    		}
    		printf("│");
    	}
    	printf("\n");

    	if (i != N-1){
    		printf("     ├");
    		for (k=0; k<N-1; k++){
    			printf("──┼");
    		}
    		printf("──┤");
    		printf("\n");
    	}
    }
    printf("     └");
    for (k=0; k<N-1; k++){
    	printf("──┴");
    }
    printf("──┘");
    printf("\n");    
}


// INSERT A NODE INTO OPEN LIST
node* insertOpenList(node* head, node* new){
	node* current;
	// list is empty
	if (head == NULL){
		head = new;
		return head;
	}
	// insert new node first in the list
	if (head->f >= new->f){
		new->next = head;
		head = new;
		return head;
	}
	current = head;
	// list has many elements, insert the new node somewhere in the middle (sorted)
	while (current->next != NULL){
		if (current->next->f >= new->f && current->f <= new->f){
			new->next = current->next;
			current->next = new; 
			return head;
		}
		current = current->next;
	}
	// insert the new node last in the list
	current->next = new;
	return head;
}


// MAIN ROUTINE
int main()  
{  
	char c; // used to flush input in stdin
	int N;
   	float p;
   	
   	float** heuristic; //2d array with heuristic values (-1: obstacle, value: min(hG1,hG2))
   	float hG1;
   	float hG2;

   	node* head;		  // head of open linked list (sorted by f)
   	node* current;
   	node* child;
   	node* previous;
   	
 	int** closedList;		// 2d array with positions of closed nodes (0: open, 1: closed)
	
	int closedListCounter = 1;
	float pathCost = 0;

	int flag = 0;		// used for avoiding duplicate nodes when an existing node's d value is updated in openList

	node* temp;		// reset algorithm
	int tempG[2] = {-1, -1};

   	int G1[2] = {-1, -1};
   	int G2[2] = {-1, -1};
   	int S[2] = {-1, -1};

   	int i;
   	int j;
   	int k;
   	int n;

   	//node* current1; 	//debug

    // INPUT PROMPTS (N,p)
   	while (1) {
    	printf("Input N (>2): ");
		scanf("%d", &N);
		if (N>1 && N<= 10000) {
			break;
		}
		printf("Invalid input, try again.\n");
		while ( (c = getchar()) != '\n' && c != EOF ) { }	//flush stdin when invalid input character
	} 
	while (1) {
    	printf("Input p (0-1): ");
		scanf("%f", &p);
		if (p>=0 && p<1) {
			break;
		}
		printf("Invalid input, try again.\n");
		while ( (c = getchar()) != '\n' && c != EOF ) { }
	}
	while ( (c = getchar()) != '\n' && c != EOF ) { }

	
	// INITIATE CLOSED LIST
	closedList = (int**) malloc(N*sizeof(int*));
    for (i=0; i<N; i++){
    	closedList[i] = (int*) calloc(N, sizeof(int));
    }

	
	// GENERATE OBSTACLES (-1: obstacle), p: probability
    heuristic = (float**) malloc(N*sizeof(float*));
    for (i=0; i<N; i++){
    	heuristic[i] = (float*) malloc(N*sizeof(float));
    }
    srand(time(NULL));
    for (i=0; i<N; i++){
    	for (j=0; j<N; j++){
    		if ((rand()%100)/100.00 < p){
    			heuristic[i][j] = -1;
    		}
    		else {
				heuristic[i][j] = 0;
    		}
    	}
    }

    printMaze(heuristic, N, S, G1, G2, NULL, NULL);		// print maze with obstacles to choose S, G1, G2 values

    // INPUT PROMPTS (S, G1, G2)
    while (1) {
    	printf("Input S <row,column>: ");
		scanf("%d,%d", &S[0], &S[1]);
		if (S[0]>=0 && S[0]<N && S[1]>=0 && S[1]<N && heuristic[S[0]][S[1]] != -1){
			break;
		}
		printf("Invalid input, try again.\n");
		while ( (c = getchar()) != '\n' && c != EOF ) { }
	} 
	while (1) {
    	printf("Input G1 <row,column>: ");
		scanf("%d,%d", &G1[0], &G1[1]);
		if (G1[0]>=0 && G1[0]<N && G1[1]>=0 && G1[1]<N && heuristic[G1[0]][G1[1]] != -1){
			if (G1[0] != S[0] || G1[1] != S[1]){
				break;
			}
		}
		printf("Invalid input, try again.\n");
		while ( (c = getchar()) != '\n' && c != EOF ) { }
	} 
	while (1) {
    	printf("Input G2 <row,column>: ");
		scanf("%d,%d", &G2[0], &G2[1]);
		if (G2[0]>=0 && G2[0]<N && G2[1]>=0 && G2[1]<N && heuristic[G2[0]][G2[1]] != -1){
			if ((G2[0] != S[0] || G2[1] != S[1]) && (G2[0] != G1[0] || G2[1] != G1[1])){
				break;
			}
		}
		printf("Invalid input, try again.\n");
		while ( (c = getchar()) != '\n' && c != EOF ) { }
	}


	// MAIN ALGORITHM (2 PATHS)
	for (n=0; n<2; n++){

		printf("\n");
	    for (i=0; i<4*N; i++){
	    	printf("═");
	    }
	    printf("\n");

		// UPDATE HEURISTIC VALUES MATRIX (MIN(hG1,hG2))
		for (i=0; i<N; i++){
	    	for (j=0; j<N; j++){
	    		if (heuristic[i][j] != -1){
					hG1 = abs(G1[1]-j)*0.5 + abs(G1[0]-i)*1.0;
	    			hG2 = abs(G2[1]-j)*0.5 + abs(G2[0]-i)*1.0;
	    			if (hG1 < hG2){
	    				heuristic[i][j] = hG1;
	    			}
	    			else {
	    				heuristic[i][j] = hG2;
	    			}
	    		}
	    	}
	    }


		// >>>>>>>>> A* ALGORITHM <<<<<<<<<<<<<<
		
	    // current node
		current = (node*) malloc(sizeof(node));
		current->pos[0] = S[0];
		current->pos[1] = S[1];
		current->d = 0;
		current->f = current->d + heuristic[current->pos[0]][current->pos[1]];
		current->parent = NULL;
		current->next = NULL;

		// put current in closed list
		closedList[current->pos[0]][current->pos[1]] = 1;

		// until you reach G1 or G2 or nopath...
		do  {
			// remove the current node from the open list
			head = current->next;

			// check UP neighbour position, if there is a valid one or its node is not in closed list
			if (current->pos[0] > 0 && heuristic[current->pos[0]-1][current->pos[1]] != -1 && closedList[current->pos[0]-1][current->pos[1]] != 1){
				// chech if is in open list
				child = head;
				previous = NULL;
				while (child != NULL){
					if (child->pos[0] == current->pos[0]-1 && child->pos[1] == current->pos[1]){
						// update d value if it is better
						if (child->d > current->d + 1){
							child->d = current->d + 1;
							child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
							child->parent = current;
							// remove from open list
							if (previous != NULL){
								previous->next = child->next;
								child->next = NULL;
								// put it back in in the new sorted position
								head = insertOpenList(head, child);
							}
						}
						flag = 1;
						break;
					}
					previous = child;
					child = child->next;
				}
				// if a node is not exist, create one and put it in the sorted open list
				if (flag == 0){
					child = (node*) malloc(sizeof(node));
					child->pos[0] = current->pos[0] - 1;
					child->pos[1] = current->pos[1];
					child->d = current->d + 1;
					child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
					child->parent = current;
					child->next = NULL;

					head = insertOpenList(head, child);
				}
				else{ 
					flag = 0;
				}	
			}
			// check DOWN neighbour position, if there is a valid one or its node is not in closed list
			if (current->pos[0] < N-1 && heuristic[current->pos[0]+1][current->pos[1]] != -1 && closedList[current->pos[0]+1][current->pos[1]] != 1){
				child = head;
				previous = NULL;
				while (child != NULL){
					if (child->pos[0] == current->pos[0]+1 && child->pos[1] == current->pos[1]){
						if (child->d > current->d + 1){
							child->d = current->d + 1;
							child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
							child->parent = current;
							if (previous != NULL){
								previous->next = child->next;
								child->next = NULL;

								head = insertOpenList(head, child);
							}
						}
						flag = 1;
						break;
					}
					previous = child;
					child = child->next;
				}
				if (flag == 0){
					child = (node*) malloc(sizeof(node));
					child->pos[0] = current->pos[0] + 1;
					child->pos[1] = current->pos[1];
					child->d = current->d + 1;
					child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
					child->parent = current;
					child->next = NULL;
				
					head = insertOpenList(head, child);
				}
				else{ 
					flag = 0;
				}		
			}
			// check LEFT neighbour position, if there is a valid one or its node is not in closed list
			if (current->pos[1] > 0 && heuristic[current->pos[0]][current->pos[1]-1] != -1 && closedList[current->pos[0]][current->pos[1]-1] != 1){
				child = head;
				previous = NULL;
				while (child != NULL){
					if (child->pos[0] == current->pos[0] && child->pos[1] == current->pos[1]-1){
						if (child->d > current->d + 0.5){
							child->d = current->d + 0.5;
							child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
							child->parent = current;
							if (previous != NULL){
								previous->next = child->next;
								child->next = NULL;

								head = insertOpenList(head, child);
							}
						}
						flag = 1;
						break;
					}
					previous = child;
					child = child->next;
				}
				if (flag == 0){
					child = (node*) malloc(sizeof(node));
					child->pos[0] = current->pos[0];
					child->pos[1] = current->pos[1] - 1;
					child->d = current->d + 0.5;
					child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
					child->parent = current;
					child->next = NULL;
				
					head = insertOpenList(head, child);
				}
				else{ 
					flag = 0;
				}		
			}
			// check RIGHT neighbour position, if there is a valid one or its node is not in closed list
			if (current->pos[1] < N-1 && heuristic[current->pos[0]][current->pos[1]+1] != -1 && closedList[current->pos[0]][current->pos[1]+1] != 1){
				child = head;
				previous = NULL;
				while (child != NULL){
					if (child->pos[0] == current->pos[0] && child->pos[1] == current->pos[1]+1){
						if (child->d > current->d + 0.5){
							child->d = current->d + 0.5;
							child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
							child->parent = current;
							if (previous != NULL){
								previous->next = child->next;
								child->next = NULL;

								head = insertOpenList(head, child);
							}
							
						}
						flag = 1;
						break;
					}
					previous = child;
					child = child->next;
				}
				if (flag == 0){
					child = (node*) malloc(sizeof(node));
					child->pos[0] = current->pos[0];
					child->pos[1] = current->pos[1] + 1;
					child->d = current->d + 0.5;
					child->f = child->d + heuristic[child->pos[0]][child->pos[1]];
					child->parent = current;
					child->next = NULL;
				
					head = insertOpenList(head, child);
				}	
				else{ 
					flag = 0;
				}	
			}

			// update closed list
			closedList[current->pos[0]][current->pos[1]] = 1;
			closedListCounter++;

			// new current node (the one with the min f value)
			current = head;


			//************debug************
			
			// show current open list
			//current1 = head;
   	 		//while(current1 != NULL){
   	 		//	printf("%d,%d ", current1->pos[0], current1->pos[1]);
   	 		//	current1 = current1->next;
   	 		//}
   	 		//printf("\n");

   	 		// show closed nodes in the maze
			//printMaze(heuristic, N, S, G1, G2, NULL, closedList);
	   	 	//waitFor(1);  
	   	 	//system("cls");
	   	 	//******************************

			// if you reach G1,G2, break (tempG: keeps the info whether G1 or G2 reached first)
			if (head != NULL){
				if (head->pos[0] == G1[0] && head->pos[1] == G1[1]){
					tempG[0] = G1[0];
					tempG[0] = G1[0];
					break;
				}
				else if (head->pos[0] == G2[0] && head->pos[1] == G2[1]){
					tempG[0] = G2[0];
					tempG[0] = G2[0];
					break;
				}
			}
	    } while (current != NULL);
		// >>>>>>END OF A* ALGORITHM<<<<<<


		// PRINT TITLE
		if (n == 0){
	    	printf("\n   FIRST PATH\n\n");
	    }
	    else{
	    	printf("\n   SECOND PATH\n\n");
	    }


		// PRINT PATH, COST AND NODES CHECKED NUMBER
		if (head == NULL){
	    	printf("No Path\n");
	    	if (n == 0){
	    		n = 1;
	    	}
	    }
	    else {
	    	current = head;
	    	while (current->parent != NULL){
	    		if (current->pos[0] == current->parent->pos[0]){
	    			pathCost += 0.5;
	    		}
	    		else{
	    			pathCost += 1.0;
	    		}
	    		current = current->parent;
	    	}
	    }
	    
		printMaze(heuristic, N, S, G1, G2, head, NULL);

		printf("Path Cost: %1.1f\n", pathCost);
	    printf("Nodes checked: %d\n", closedListCounter);
	    
	    //printMaze(heuristic, N, S, G1, G2, NULL, closedList);	// debug (print the maze with the nodes checked)


	    // RESET ALGORITHM FOR SECOND PATHFINDING
	    
	    // reset counters/flags
	    pathCost = 0;
	    closedListCounter = 1;
	    flag = 0;

	    // reset start/stop points
	    if (tempG[0] == G1[0] && tempG[0] == G1[0]){
	    	S[0] = G1[0];
	    	S[1] = G1[1];
	    	G1[0] = 4*N;
	    	G1[1] = 4*N;
	    }
	    else if (tempG[0] == G2[0] && tempG[0] == G2[0]){
	    	S[0] = G2[0];
	    	S[1] = G2[1];
	    	G2[0] = 4*N;
	    	G2[1] = 4*N;
	    }

	    // reset open list
	    current = head;
	    while (current != NULL){
	    	temp = current;
	    	current = current->next;
	    	free(temp);
	    }
	    head = NULL;

	    // reset closed list
	    for (i=0; i<N; i++){
	    	for (j=0; j<N; j++){
    			closedList[i][j] = 0;
    		}
    	}
	}
}