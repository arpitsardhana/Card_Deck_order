#include "deck.h"

struct card *deck; /* declaring deck of cards */
struct card *deck_end; /* End of Card Deck to keep track of last card */
struct card *table; /* Cards put on table */
struct card *transit; /* temp struct to hold card during transfer from deck to table */
uint32_t card_count; /* Keep track of number of card entered as input */
struct index_map  *index_mapper; /* structure to map position during rounds */
clock_t start; /*Start of clock */
clock_t end;  /*end of program */
double time_taken; /*total time taken by program to execute */

// Calculate GCD based on euclidean algorithma and recursion 
uint64_t gcd(uint64_t num1,uint64_t num2) {
	if (num2 == 0 || num1 == num2) {
		return num1;
	}
	
	if (num1 > num2) {
		return gcd(num2,num1%num2);
	} else {
		return gcd(num1,num2%num1);
	}
}
//Function to initialize deck
void initialze_deck() {
	uint32_t i;
        for (i=0;i<card_count;i++) {
                deck[i].index = i;
                if (i < card_count-1) {
                        deck[i].next = &(deck[i+1]);
                } else {
                        deck[i].next =  NULL;
                } 
        }
        deck_end = &(deck[card_count-1]);
}

//Function to perform first round
void do_first_round() {

        while (deck) {

                //Place cards on table from deck
                transit = table;
                table = deck;
                deck = deck->next;
                table->next = transit;

                //Round is complete once no card is left in deck
                if (deck == NULL)
                        break;

                if (deck->next == NULL) {
                        //Last card in deck handling
                        transit = table;
                        table = deck;
                        deck = deck->next;
                        table->next = transit;

                } else {
                        //Put cards at end of deck
                        transit = deck;
                        deck = deck->next;
                        deck_end->next  = transit;
                        deck_end = transit;
                        deck_end->next = NULL;
                }

        }
        //Take cards from table which is new deck
        deck = table;
        table = NULL;
        transit = deck;

}

//Function to map positions obtained from first round
void map_positions() {
	uint32_t i;
        index_mapper = (struct index_map *)malloc(card_count * sizeof(struct index_map));
        memset(index_mapper,0,sizeof(index_mapper));
        int count = 0;
        while (transit != NULL) {
                if (DEBUG)
                        printf("%d \n",transit->index);

                //Updating Mapping obtained from first round
                index_mapper[transit->index].next = count;
                transit = transit->next;
                count++;
        }
        if (DEBUG) {
                for (i=0;i<count;i++) {
                        printf("%d ---> %d \n",i,index_mapper[i].next);
                } 

        }
}
//Function to calculate rounds
uint64_t calculate_rounds() {
        uint32_t i = 0;
        uint32_t next;
        uint64_t lcm = 1;
        uint64_t round;
        for(i=0;i<card_count;i++) {

                //Index with marked are already part of cycle,thus ignored
                if(index_mapper[i].cycle_mark == 1) {
                        continue;

                }
                next = index_mapper[i].next;
                index_mapper[i].cycle_mark = 1;
                round = 1;
                //Finding no of cycles required to get back original index
                while (i !=  next) {
                        index_mapper[next].cycle_mark = 1;
                        next = index_mapper[next].next;
                        round++;

                }
                if (DEBUG)
                        printf("round for %d ---- %lu \n",i,round);

                //Calculating Cumulative LCM
                lcm = (lcm*round)/gcd(lcm,round);
        }
	return lcm;
}
//Main function declaration
int main(int argc,char *argv[]) { 
	start = clock();
	uint64_t rounds;
	if (argc <= 1) {
		printf("Enter valid number of cards\n");
		return 0;
	}

	card_count = atoi(argv[1]);
	if (card_count == 0) {
		printf("Invalid Value entered \n");
		return -1;
	}


	// Allocate memory for card deck
	deck = (struct card *)malloc(card_count * sizeof(struct card));

	if (deck == NULL) {
		perror("No Memory");	
		return ENOMEM;
	}
	table = NULL;

	//Intialize deck
	initialze_deck();

	//Undergo first round.
	do_first_round();

	//From first round, we obtain mapping of position in each round
	map_positions();

	//No of Rounds calculation
	rounds = calculate_rounds();

	printf("Number of rounds to get back original deck are %lu \n",rounds);	
	end = clock();
	time_taken = (double)(end-start) / CLOCKS_PER_SEC;
	printf("Total Time taken %f sec \n",time_taken);
	return 0;
}
