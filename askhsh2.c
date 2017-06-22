// TSALESIS EVANGELOS
// AM: 1779

// It solves a hatori puzzle (MxM) containing the numbers 1-K (K<=M) using Simulated Annealing.

// The cost of every state is the number of duplicates in rows and columns.
// In every loop the cost of a random new state is calculated. 
// (new state -> a random box changes state (shaded<->unshaded)

// Every new state, that is tested, satisfies the constrains of the puzzle:
// CSP1: A shaded box must not touch another shaded box.
// CSP2: All numbered boxes must be connected to each other.

// 2d array 'hitori' keeps the number of each box. 
// When a box is shaded its number turns negative.
// When a box is unshaded its number turns positive.

// ADD -lm ARGUMENT IN GCC COMPILER


#include <stdio.h>  
#include <stdlib.h>
#include <time.h> 
#include <math.h>


// PRINT HITORI
void printHitori(int** hitori, int M){
	int i;
   	int j;
   	int k;

	
    printf("  ┌");
 	for (i=0; i<M-1; i++){
 		printf("──┬");
 	}
 	printf("──┐");
 	printf("\n");
    for (i=0; i<M; i++){
    	printf("  ");
    	printf("│");
    	for (j=0; j<M; j++){
    		if (hitori[i][j] < 0){
    			printf("██");
    		}
    		else{
    			printf("%2d", hitori[i][j]);
    		}
    		printf("│");
    	}
    	printf("\n");

    	if (i != M-1){
    		printf("  ├");
    		for (k=0; k<M-1; k++){
    			printf("──┼");
    		}
    		printf("──┤");
    		printf("\n");
    	}
    }
    printf("  └");
    for (k=0; k<M-1; k++){
    	printf("──┴");
    }
    printf("──┘");
    printf("\n");    
}


// CHECK IF THE NUMBERED (WHITES) BOXES ARE CONNECTED IN A HITORI
char areConnected(int** hitori, int M){
	int i;
	int j;
	int count;

	int whites = 0;
	int flag = 0;

	// Linked Open List
	typedef struct box{
		int pos[2];
		struct box* next;
	} box;
	box* head;
	box* current;
	box* new;
	box* current2;

	// 2d Array Closed List
	char** closedList = (char**) malloc(M*sizeof(char*));
    for (i=0; i<M; i++){
    	closedList[i] = (char*) calloc(M, sizeof(char));
    }

	// count white boxes, find first white box
	for (i=0; i<M; i++){
		for (j=0; j<M; j++){
			if (hitori[i][j] > 0) {
				if (flag == 0){
					head = (box*) malloc(sizeof(box));
					head->pos[0] = i;
					head->pos[1] = j;
					head->next = NULL;
					
					flag = 1;
				}
				whites++;
			}
		}
	}

	// traverse white boxes in open list
	while (head != NULL){
		if (head->pos[0] > 0 && hitori[head->pos[0]-1][head->pos[1]] > 0){
			if (closedList[head->pos[0]-1][head->pos[1]] != 1){
				// check if the new box is in the open list before it is inserted
				current = head;
				while (current != NULL){
					if (current->pos[0] == head->pos[0]-1 && current->pos[1] == head->pos[1]){
						break;
					}
					if (current->next == NULL){
						new = (box*) malloc(sizeof(box));
						new->pos[0] = head->pos[0]-1;
						new->pos[1] = head->pos[1];
						new->next = NULL;
						current->next = new;
					}
					current = current->next;
				}
			}
		}
		if (head->pos[0] < M-1 && hitori[head->pos[0]+1][head->pos[1]] > 0){
			if (closedList[head->pos[0]+1][head->pos[1]] != 1){
				// check if the new box is in the open list before it is inserted
				current = head;
				while (current != NULL){
					if (current->pos[0] == head->pos[0]+1 && current->pos[1] == head->pos[1]){
						break;
					}
					if (current->next == NULL){
						new = (box*) malloc(sizeof(box));
						new->pos[0] = head->pos[0]+1;
						new->pos[1] = head->pos[1];
						new->next = NULL;
						current->next = new;
					}
					current = current->next;
				}
			}
		}
		if (head->pos[1] > 0 && hitori[head->pos[0]][head->pos[1]-1] > 0){
			if (closedList[head->pos[0]][head->pos[1]-1] != 1){
				// check if the new box is in the open list before it is inserted
				current = head;
				while (current != NULL){
					if (current->pos[0] == head->pos[0] && current->pos[1] == head->pos[1]-1){
						break;
					}
					if (current->next == NULL){
						new = (box*) malloc(sizeof(box));
						new->pos[0] = head->pos[0];
						new->pos[1] = head->pos[1]-1;
						new->next = NULL;
						current->next = new;
					}
					current = current->next;
				}
			}
		}
		if (head->pos[1] < M-1 && hitori[head->pos[0]][head->pos[1]+1] > 0){
			if (closedList[head->pos[0]][head->pos[1]+1] != 1){
				// check if the new box is in the open list before it is inserted
				current = head;
				while (current != NULL){
					if (current->pos[0] == head->pos[0] && current->pos[1] == head->pos[1]+1){
						break;
					}
					if (current->next == NULL){
						new = (box*) malloc(sizeof(box));
						new->pos[0] = head->pos[0];
						new->pos[1] = head->pos[1]+1;
						new->next = NULL;
						current->next = new;
					}
					current = current->next;
				}
			}
		}

		closedList[head->pos[0]][head->pos[1]] = 1;

		current = head;
		head = head->next;
		free (current);
	}

	// count white boxes that have been traversed
	for (i=0; i<M; i++){
		for (j=0; j<M; j++){
			if (closedList[i][j] == 1){
				whites--;
			}
		}
	}

	// if (total number of white boxes) = (number of white boxes have been traversed) return true
	if (whites == 0){
		return 1;
	}
	else{
		return 0;
	}
}


// CALCULATE COST (+1 WHEN A DUPLICATE NUMBER IS FOUND)
int calculateCost(int** hitori, int M){
	int cost = 0;

	int i;
	int j;
	int m;

	for (i=0; i<M; i++){
		for (j=0; j<M; j++){
			if (hitori[i][j] > 0){
				// check if there are doubles in the same row
				for (m=0; m<j; m++){
					if (hitori[i][j] == hitori[i][m]){
						cost++;
					}
				}
				for (m=j+1; m<M; m++){
					if (hitori[i][j] == hitori[i][m]){
						cost++;
					}
				}
				// check if there are doubles in the same column
				for (m=0; m<i; m++){
					if (hitori[i][j] == hitori[m][j]){
						cost++;
					}
				}
				for (m=i+1; m<M; m++){
					if (hitori[i][j] == hitori[m][j]){
						cost++;
					}
				}
			}
		}
   	}
   	return cost;
}


// MAIN ROUTINE
int main(){

	int i;
	int j;
	int k;
	int m;

	char c;

	int CSP;
	int choosePuzzle;
	int choosePuzzle2;

	FILE* fid;

	int M;
	int K;

	float T = 10.0;

	int** hitori;		// keeps the box's values (negative when shaded)

	int cost;
	int new_cost;

	int failCounter = 0;


	// test hitori puzzles
	int test1[8][8] = {{4,8,1,6,3,2,5,7},
    					{3,6,7,2,1,6,5,4},
    					{2,3,4,8,2,8,6,1},
    					{4,1,6,5,7,7,3,5},
    					{7,2,3,1,8,5,1,2},
    					{3,5,6,7,3,1,8,4},
    					{6,4,2,3,5,4,7,8},
    					{8,7,1,4,2,3,5,6}};

    int test2[8][8] = {{5,5,7,5,1,5,4,2},
    					{7,5,1,5,8,2,8,6},
    					{6,2,5,7,2,1,3,8},
    					{4,1,7,3,2,3,6,8},
    					{4,6,2,5,4,8,1,7},
    					{5,5,3,6,2,4,7,4},
    					{1,8,4,6,5,1,2,3},
    					{7,7,2,4,6,5,8,2}};	

    int test3[8][8] ={{1,1,2,7,1,8,1,3},
    					{1,8,7,1,5,6,3,2},
    					{8,2,2,4,7,4,5,7},
    					{6,7,1,7,2,4,3,6},
    					{3,5,1,2,7,7,6,1},
    					{5,4,8,6,1,2,3,7},
    					{1,6,3,7,8,3,7,8},
    					{7,3,2,1,4,5,2,8}};	

   	int test4[5][5] = {{3,3,1,2,4},
						{5,4,4,1,4},
						{5,2,3,5,1},
						{1,5,2,1,3},
						{3,3,3,4,5}};	

	int test5[5][5]	= {{4,1,5,3,2},
						{1,2,3,5,5},
						{3,4,4,5,1},
						{3,5,1,5,4},
						{5,2,5,1,3}};

	int test6[12][12] = {{12,6,12,7,11,12,9,11,2,6,3,2},
						{12,9,6,9,10,2,4,3,8,12,7,1},
						{1,2,6,3,5,4,5,7,11,5,10,5},
						{1,3,11,3,2,5,10,12,1,6,6,5},
						{8,12,9,11,12,1,4,5,12,3,2,4},
						{1,11,6,1,3,7,8,11,6,8,4,7},
						{2,11,8,11,7,6,3,10,12,5,9,11},
						{4,10,1,6,4,11,6,8,12,7,9,12},
						{12,1,6,4,8,4,5,11,7,9,12,4},
						{3,12,7,7,12,10,6,1,7,11,4,2},
						{9,5,10,2,1,5,7,1,4,10,11,3},
						{1,7,1,9,8,12,2,6,5,4,8,4}};				

	srand(time(NULL));

	
	// ***************** CHOICES ***********************
	// Choose Random puzzle, a Test Sample puzzle, or a puzzle from a txt file
	while (1) {
    	printf("Choose Random (1), a Test puzzle (2) or an imported (3) puzzle: ");
		scanf("%d", &choosePuzzle);
		if (choosePuzzle == 1 || choosePuzzle == 2 || choosePuzzle == 3) {
			break;
		}
		printf("Invalid input, try again.\n");
		while ( (c = getchar()) != '\n' && c != EOF ) { }	//flush stdin when invalid input character
	}
	
	// Choose random puzzle size
	if (choosePuzzle == 1){ 
		// Choose M,K
		while (1) {
	    	printf("Choose M (>=2): ");
			scanf("%d", &M);
			if (M>1 && M<= 99) {
				break;
			}
			printf("Invalid input, try again.\n");
			while ( (c = getchar()) != '\n' && c != EOF ) { }	//flush stdin when invalid input character
		} 
		while (1) {
	    	printf("Choose K (1-M): ");
			scanf("%d", &K);
			if (K>0 && K<=M) {
				break;
			}
			printf("Invalid input, try again.\n");
			while ( (c = getchar()) != '\n' && c != EOF ) { }	//flush stdin when invalid input character
		}
	}
	// Choose test puzzle
	else if (choosePuzzle == 2) {
		while (1) {
	    	printf("Choose number of a test puzzle (1-6): ");
			scanf("%d", &choosePuzzle2);
			if (choosePuzzle2 >= 1 || choosePuzzle2 <= 6) {
				break;
			}
			printf("Invalid input, try again.\n");
			while ( (c = getchar()) != '\n' && c != EOF ) { }	//flush stdin when invalid input character
		} 
		switch (choosePuzzle2){
			case 1: 
				M = 8;
				K = 8;
				break;
			case 2: 
				M = 8;
				K = 8;
				break;
			case 3: 
				M = 8;
				K = 8;
				break;
			case 4: 
				M = 5;
				K = 5;
				break;
			case 5: 
				M = 5;
				K = 5;
				break;
			case 6: 
				M = 12;
				K = 12;
				break;							
		}
	}
	// import a puzzle
	else {
		fid = fopen("hitori.txt", "r");
		if (fid == NULL){
			printf("Import file 'hitori.txt' does not exist");
			exit(0);
		}
		fscanf(fid, "Size = %d", &M);
		K = M;
	}
	// Choose CSP mode
	while (1) {
	    printf("Choose CSP mode (1 or 2): ");
		scanf("%d", &CSP);
		if (CSP == 1 || CSP == 2) {
			break;
		}
		printf("Invalid input, try again.\n");
		while ( (c = getchar()) != '\n' && c != EOF ) { }	//flush stdin when invalid input character
	} 
	 //*******************************************************


	
	// INITIATE HITORI
	hitori = (int**) malloc(M*sizeof(int*));
    for (i=0; i<M; i++){
    	hitori[i] = (int*) calloc(M, sizeof(int));
    }
    for (i=0; i<M; i++){
    	for (j=0; j<M; j++){
			if (choosePuzzle == 1){
				hitori[i][j] = rand()%K+1;
			}
			else if (choosePuzzle == 2) {
				switch (choosePuzzle2){
					case 1: 
						hitori[i][j] = test1[i][j];
						break;
					case 2: 
						hitori[i][j] = test2[i][j];
						break;
					case 3: 
						hitori[i][j] = test3[i][j];
						break;
					case 4: 
						hitori[i][j] = test4[i][j];
						break;
					case 5: 
						hitori[i][j] = test5[i][j];
						break;
					case 6: 
						hitori[i][j] = test6[i][j];
						break;							
				}
			}
			else {
				for (i=0; i<M; i++){
					for (j=0; j<M; j++){
						fscanf(fid, "%d", &hitori[i][j]);
					}
				}
				fclose(fid);
			}
		}
    }

    // PRINT INITIAL HITORI (WITHOUT SHADED BOXES)
    printHitori(hitori, M);
    printf("Calculating...\n");
    
    // >>>>> MAIN ALGORITHM (SIMULATED ANNEALING) <<<<<<<
  	// calculate initial cost
    cost = calculateCost(hitori, M);
    // while cost > 0 or consecutive failed movements are more than 6N...
    while (cost != 0 && failCounter < 6*M*M){
    	// 2*M*M tests per temperature
	    for (k=0; k<2*M*M; k++){
	    	// shade or unshade a random valid box
	    	while (1) {
	    		i = rand()%M;
	    		j = rand()%M;
	    		if (hitori[i][j] > 0){
	    			// shade a box unless it touches a shaded box
	    			if ((i == 0 || hitori[i-1][j] > 0) && (i == M-1 || hitori[i+1][j] > 0)){
	    				if ((j == 0 || hitori[i][j-1] > 0) && (j == M-1 || hitori[i][j+1] > 0)){
	    					hitori[i][j] = -hitori[i][j];
	    					// check if the numbered boxes are connected in the new state (CSP2)...
	    					if (CSP == 2){
	    						if (areConnected(hitori, M)){
	    							break;
	    						}
	    						// ...if not, revert.
	    						else {
	    							hitori[i][j] = -hitori[i][j];
	    						}
	    					}
	    					else {
	    						break;
	    					}
	    				}
	    			}
	    		}
	    		else {
	    			// unshade a box
	    			hitori[i][j] = -hitori[i][j];
	    			break;
	    		}
	    	}

	    	// calculate new cost
	    	new_cost = calculateCost(hitori, M);
	  
	    	// keep new state
	    	if (new_cost <= cost){
	    		failCounter = 0;
	    	}
	    	else if (rand()%100/100.00 < 1.00/(1.00 + exp((new_cost-cost)/T))) {
	    		failCounter = 0;
	    	}
	    	// return to previous state
	    	else {
	    		hitori[i][j] = -hitori[i][j];
	    		new_cost = cost;

	    		failCounter++;
	    	}
	    	cost = new_cost;
	    }
	    // update temperature
	    T = 0.999 * T;
	    printf("\rT = %1.3f", T);
	}
	// >>>> END OF ALGORITHM <<<<

	// result
	if (cost != 0){
		printf("\n\nNo Solution, Cost = %d\n", cost);		
	}
	else {
		printf("\n\nSolution:\n");
    }
    printHitori(hitori, M);
}