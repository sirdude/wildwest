#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "message.h"
#include "states.h"

struct message *mailqueue = NULL; 

// Todo  make this smarter so it does not add duplicates....
int send_message(int sender, int receiver, int message, double timestamp, 
	void *extrainfo) {

	struct message *ptr = mailqueue;
	struct message *tmp;

	tmp = malloc(sizeof(struct message));
	tmp->sender = sender;
	tmp->receiver = receiver;
	tmp->msg = message;
	tmp->dispatchtime = timestamp;
	tmp->extrainfo = extrainfo;
	tmp->next = NULL;

	if (ptr == NULL) {
		mailqueue = tmp;
		return 1;
	}
	if (timestamp < ptr->dispatchtime) {
		tmp->next = mailqueue;
		mailqueue = tmp;
		return 1;
	}
	while (ptr->next != NULL) {
		if (timestamp < ptr->next->dispatchtime) {
			tmp->next = ptr->next;
			ptr->next = tmp;
			return 1;
		}
		ptr = ptr->next;
	}
	ptr->next = tmp;
	return 1;
}

/* If we were doing obj oriented programming this 
   would be in the object, were not so its all here */
void handle_message(struct message *ptr) {
	switch (ptr->msg) {
		case HONEYIMHOME: 
			people[ptr->receiver].laststate = 
				people[ptr->receiver].state;
			people[ptr->receiver].state = CookStew;
			break;
		case ROASTDONE: 
			people[ptr->receiver].laststate = 
				people[ptr->receiver].state;
			people[ptr->receiver].state = GetCookedStew;
			break;
		case HITS: 
			people[ptr->receiver].laststate = 
				people[ptr->receiver].state;
			people[ptr->receiver].state = Fight;
// Should do some extra stuff here....
			break;
		default:
			printf("Undefined message from p%d to p%d\n", 
				ptr->sender, ptr->receiver);
		
	}
}

void process_mail() {
	struct message *ptr, *last;
	double curtime = time(NULL);

	ptr = mailqueue;
	last = NULL;
	while (ptr != NULL) {
		if (ptr->dispatchtime < curtime) {
			handle_message(ptr);
			if (last == NULL) mailqueue = ptr->next;
			else last->next = ptr->next;
			free(ptr);
			last = ptr->next;
			if (last != NULL) ptr = last->next;
			else ptr = NULL;
		} else {
			break;
		}
	}
}
	
