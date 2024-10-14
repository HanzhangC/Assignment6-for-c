/* A simplified community detection algorithm:
 *
 * Skeleton code written by Jianzhong Qi, May 2023
 * Edited by: Hanzhang Chen 1173968
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */

#define YEAR_ARRAY_LENGTH 4
#define HASHTAGS_LENGTH 10
#define MAX_LENGTH 20
#define MAX_USERS 50

typedef struct {
	/* add your user_t struct definition */
	int id;
	int year[YEAR_ARRAY_LENGTH + 1];
	char tags[HASHTAGS_LENGTH][MAX_LENGTH + 1];
	int tagnums;
} user_t;

typedef int data_t;							  /* to be modified for Stage 4 */

/* linked list type definitions below, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c 
*/
typedef struct node node_t;

struct node {
	data_t data;
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;

/****************************************************************/

/* function prototypes */
list_t *make_empty_list(void);
void free_list(list_t *list);
void print_list(list_t *list);
list_t *insert_unique_in_order(list_t *list, data_t value);

void print_stage_header(int stage_num);

int stage_one(user_t store_users[]);
void stage_two(int store_inputs_matrix[][MAX_USERS], int usernums);
void stage_three(int store_inputs_matrix[][MAX_USERS], double socs[][MAX_USERS], int usernums);
void stage_four(double socs[][MAX_USERS], int usernums, user_t store_users[]);

/* add your own function prototypes here */
double calculate_soc(int store_inputs_matrix[][MAX_USERS], int num1, int num2, int usernums);
int is_core(int id, double socs[][MAX_USERS], int usernums, double ths, int thc);
/****************************************************************/

/* main function controls all the action; modify if needed */
int
main(int argc, char *argv[]) {
	/* add variables to hold the input data */
    user_t store_users[MAX_USERS];
    int usernums = 0;
	/* stage 1: read user profiles */
    usernums = stage_one(store_users); 
	
	/* stage 2: compute the strength of connection between u0 and u1 */
    int store_inputs_matrix[MAX_USERS][MAX_USERS];
    stage_two(store_inputs_matrix, usernums);
	
	/* stage 3: compute the strength of connection for all user pairs */
    double socs[MAX_USERS][MAX_USERS];
    stage_three(store_inputs_matrix, socs, usernums);
	
	/* stage 4: detect communities and topics of interest */
    stage_four(socs, usernums, store_users);
	
	/* all done; take some rest */
    return 0;
}
/* algorithms are fun！！！ */

/****************************************************************/

/* add your code below; you can also modify the function return type 
   and parameter list 
*/

/* stage 1: read user profiles */
int 
stage_one(user_t store_users[]) {
	/* add code for stage 1 */
    int usernums = 0;
    int max_tags = 0;
    int max_users = 0;

    // Read the first two items of the tags.
    while(scanf("u%d %d ", &store_users[usernums].id, store_users[usernums].year) == 2){
        int tagnums = 0;
		// Read the left items of the tags.
        while(scanf("#%s ", store_users[usernums].tags[tagnums]) == 1){
            tagnums += 1;
        }
        store_users[usernums].tagnums = tagnums;
        if(tagnums > max_tags){
            max_tags = tagnums;
            max_users = usernums;
        }
        usernums += 1;
    }
    
	/* print stage header */
    print_stage_header(STAGE_NUM_ONE);
    printf("Number of users: %d\n", usernums);
    printf("u%d has the largest numbers of hashtags:\n", max_users);
    for(int i = 0; i < max_tags; i++){
        printf("#%s", store_users[max_users].tags[i]);
        if (i != usernums - 1) {
            printf(" ");
        }
    }
    printf("\n\n");
    return usernums;
}

/* stage 2: compute the strength of connection between u0 and u1 */
void 
stage_two(int store_inputs_matrix[][MAX_USERS], int usernums) {
	/* add code for stage 2 */

	//Read the first line.
    for(int i = 0; i < usernums; i++){
        int num;
        scanf("%d", &num);
        store_inputs_matrix[0][i] = num;
    }
    // Read the second line.
    for(int i = 0; i < usernums; i++){
        int num;
        scanf("%d", &num);
        store_inputs_matrix[1][i] = num;
    }

    // Calculate soc.
    double soc = 0;
    if(store_inputs_matrix[0][1] == 0){
        soc = 0;
    } else{
        soc = calculate_soc(store_inputs_matrix, 0, 1, usernums);
    }
	/* print stage header */
    print_stage_header(STAGE_NUM_TWO);
    printf("Strength of connection between u0 and u1: %4.2f\n", soc);
    printf("\n");
}

/* stage 3: compute the strength of connection for all user pairs */
void 
stage_three(int store_inputs_matrix[][MAX_USERS], double socs[][MAX_USERS], int usernums) {
	/* add code for stage 3 */

    // Read the left inputs.
    for(int i = 2; i < usernums; i++){
        for(int j = 0; j < usernums; j++){
            int num;
            if(scanf("%d", &num) == 1){
                store_inputs_matrix[i][j] = num;
            }
        }
    }

	// Calculate socs.
    for(int i = 0; i < usernums; i++){
        for(int j = 0; j < usernums; j++){
            if(store_inputs_matrix[i][j] == 0){
                socs[i][j] = 0;
            } else{
                socs[i][j] = calculate_soc(store_inputs_matrix, i, j, usernums);
            }
        }
    }

	/* print stage header */
    print_stage_header(STAGE_NUM_THREE);
    for(int i = 0;i < usernums; i++){
        for(int j = 0; j < usernums; j++){
            printf("%4.2f", socs[i][j]);
            if (j != usernums - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* stage 4: detect communities and topics of interest */
void stage_four(double socs[][MAX_USERS], int usernums, user_t store_users[]) {
	/* add code for stage 4 */

    /* print stage header */
    print_stage_header(STAGE_NUM_FOUR);

    // 4.1
    double ths = 0;
    int thc = 0;
    scanf("%lf %d\n", &ths, &thc);
    for(int i = 0; i < usernums; i++){
        if(is_core(i, socs, usernums, ths, thc) == 1){
            printf("Stage 4.1. Core user: u%d; ", i);
            printf("close friends: ");
            for(int j = 0; j < usernums; j++){
                if(socs[i][j] > ths){
                    printf(" u%d", j);
                }
            }
            printf("\n");
        }
    }

    printf("\n");
}

// A soc_calculate function.
double calculate_soc(int store_inputs_matrix[][MAX_USERS], int num1, int num2, int usernums){
    int union_fred = 0, intersec_fred = 0;
    for(int i = 0; i < usernums; i++){
        if(store_inputs_matrix[num1][i] == 1 && store_inputs_matrix[num2][i] == 1){
            intersec_fred += 1;
        } if(store_inputs_matrix[num1][i] == 1 || store_inputs_matrix[num2][i] == 1){
            union_fred += 1;
        }
    }
    double soc = (double)intersec_fred / union_fred;
    return soc;
}

// Check whether a user is a core user or not.
int is_core(int id, double socs[][MAX_USERS], int usernums, double ths, int thc){
    int count = 0;
    for(int i = 0; i < usernums; i++){
        if(socs[id][i] > ths){
            count += 1;
        }
    }
    if(count > thc){
        return 1;
    }
    return 0;
}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
    printf(STAGE_HEADER, stage_num);
}

/****************************************************************/
/* linked list implementation below, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c 
*/

/* create an empty list */
list_t
*make_empty_list(void) {
    list_t *list;

    list = (list_t*)malloc(sizeof(*list));
    assert(list!=NULL);
    list->head = list->foot = NULL;

    return list;
}

/* free the memory allocated for a list (and its nodes) */
void
free_list(list_t *list) {
    node_t *curr, *prev;

    assert(list!=NULL);
    curr = list->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }

    free(list);
}

/* insert a new data element into a linked list, keeping the
	data elements in the list unique and in alphabetical order
*/
list_t
*insert_unique_in_order(list_t *list, data_t value) {
	/* the following code inserts a new node to the end of list.
		Modify it to suit Stage 4.2. Start by modifying it to 
		insert a new node while keeping an alphabetical order
		(think about how to insert in the middle of a list), 
		then, only insert when value is not in list already.
		[remove this comment to save line space if needed]
	*/

    node_t *new;

    new = (node_t*)malloc(sizeof(*new));
    assert(list!=NULL && new!=NULL);
    new->data = value;
    new->next = NULL;

    if (list->foot==NULL) {
		/* this is the first insertion into the list */
        list->head = list->foot = new;
    } else {
        list->foot->next = new;
        list->foot = new;
    }

    return list;
}

/* print the data contents of a list */
void
print_list(list_t *list) {
	/* add code to print list */
}

/****************************************************************/
/*
	Write your time complexity analysis below for Stage 4.2, 
	assuming U users, C core users, H hashtags per user, 
	and a maximum length of T characters per hashtag:

*/
