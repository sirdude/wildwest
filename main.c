#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "states.h"
#include "message.h"

struct person people[NUMP];
extern void process_mail();

void print_stats() {
	int x;
	for (x=0;x<NUMP;x++) {
		printf("Player %d:%s\n",x, people[x].name);
		printf("\tState: %d LastState: %d\n", people[x].state, 
			people[x].laststate);
		printf("\tGold: %d Money in the Bank: %d\n", people[x].gold,
			people[x].moneyinbank);
		printf("\tThirst %d Fatigue %d Bladder %d\n", people[x].thirst,
			people[x].fatigue, people[x].bladder);
	}
}

void init_people() {
	people[0].who = MINER;
	strcpy(people[0].name,"Doug the Miner");
	people[0].state = EnterMineDig;
	people[0].laststate = GoHomeAndSleep;
	people[0].gold = 0;
	people[0].moneyinbank = 0;
	people[0].thirst = 0;
	people[0].fatigue = 0;
	people[0].bladder = 0;

	people[1].who = WIFE;
	strcpy(people[1].name,"Doug's Wife");
	people[1].state = DoHouseWork;
	people[1].laststate = DoHouseWork;
	people[1].gold = 0;
	people[1].moneyinbank = 0;
	people[1].thirst = 0;
	people[1].fatigue = 0;
	people[1].bladder = 0;

	people[2].who = BARFLY;
	strcpy(people[2].name,"Barny the Drunk");
	people[2].state = QuenchThirst;
	people[2].laststate = QuenchThirst;
	people[2].gold = 0;
	people[2].moneyinbank = 0;
	people[2].thirst = 0;
	people[2].fatigue = 0;
	people[2].bladder = 0;
}

void do_quenchthirst(int x) {

	if (people[x].state != people[x].laststate) {
		printf("Player %s goes to the bar and takes a swig.\n", 
			people[x].name);
		people[x].laststate = QuenchThirst;
	}
	printf("Player %s Burps.\n", people[x].name);
	people[x].thirst = people[x].thirst -5;

	if ((people[x].who == BARFLY) && (people[0].state == QuenchThirst)) {
		// Fight stuff goes here...
	}

	if (people[x].thirst < 0) {
		if (people[x].who == MINER) people[x].state = GoHomeAndSleep;
		else people[x].state = QuenchThirst;
		people[x].laststate = QuenchThirst;
	}
}

void do_entermine(int x) {
	if (people[x].state != people[x].laststate) {
		printf("%s enters the mine.\n", people[x].name);
		people[x].laststate = EnterMineDig;
	}

	if (people[x].thirst > 15) {
		printf("%s is thursty.  He goes to the Saloon\n", 
			people[x].name);
		people[x].state = QuenchThirst;
		people[x].laststate = EnterMineDig;

	} else if (people[x].gold > 10) {
		printf("%s leaves for the bank.\n", people[x].name);
		people[x].state = VisitBankDeposit;
		people[x].laststate = EnterMineDig;
	} else if (people[x].fatigue > 5) {
		printf("%s leaves for home.\n", people[x].name);
		people[x].state = GoHomeAndSleep;
		people[x].laststate = EnterMineDig;
	} else {
		printf("%s Finds some gold.\n", people[x].name);
		people[x].gold = people[x].gold +2;
		people[x].fatigue = people[x].fatigue + 1;
	}
}

void do_visitbank(int x) {
	printf("%s Visits the bank.\n", people[x].name);
	people[x].moneyinbank += people[x].gold;
	people[x].gold = 0;
	people[x].state = people[x].laststate;
	people[x].laststate = VisitBankDeposit;
}

void do_fight(int x) {
	if (people[x].state != people[x].laststate) {
	}
	
}

void go_home(int x) {
	if (people[x].state != people[x].laststate) {
		printf("%s arrives home.\n", people[x].name);
		people[x].laststate = GoHomeAndSleep;
	}

	people[x].fatigue = people[x].fatigue -2;
	printf("%s is sitting at home.\n", people[x].name);

	if (people[x].fatigue < 0) {
		people[x].state = EnterMineDig;
		people[x].laststate = GoHomeAndSleep;
		printf("%s leaves to work in the mine.\n", people[x].name);
	}
	
}

void cook_stew(int x) {
	printf("%s Cooks Stew.\n", people[x].name);
}

void visit_bathroom(int x) {
	printf("%s Uses the can.\n", people[x].name);

	people[x].bladder = 0;

	people[x].state = people[x].laststate;
	people[x].laststate = VisitBathroom;
}

void do_housework(int x) {
	printf("%s sweeps the floor\n", people[x].name);

}

void global_update_player(int x) {
	people[x].fatigue = people[x].fatigue +1;
	people[x].thirst = people[x].thirst +1;
	people[x].bladder = people[x].bladder +1;

	if (people[x].bladder > 10) {
		people[x].laststate = people[x].state;
		people[x].state = VisitBathroom;
	}
}

void event_loop() {
	int x, c=0;

	while(c<100) {
		// Take care of sending messages...
		process_mail();

		for(x=0;x<NUMP;x++) {
			switch(people[x].state) {
				case QuenchThirst:
					do_quenchthirst(x);
					break;
				case EnterMineDig:
					do_entermine(x);
					break;
				case VisitBankDeposit:
					do_visitbank(x);
					break;
				case GoHomeAndSleep:
					go_home(x);
					break;
				case Fight:
					do_fight(x);
					break;
				case CookStew:
					cook_stew(x);
					break;
				case VisitBathroom:
					visit_bathroom(x);
					break;
				case DoHouseWork:
					do_housework(x);
					break;
				default:
					printf("Error player %d fell out of state.\n",x);
					break;
			}
			global_update_player(x);
		}
		c++;
	}
	print_stats();
}


int main(int argc, char **argv) {
	init_people();
	event_loop();
	return 1;
}
