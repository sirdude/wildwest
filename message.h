struct message {
	int sender;
	int receiver;
	int msg;
	double dispatchtime;
	void *extrainfo;
	struct message *next;
};

extern struct message *mailqueue;

enum messagetypes { HONEYIMHOME, ROASTDONE, HITS};

void process_mail();
void handle_message(struct message *ptr);
int send_message(int sender, int receiver, int message, double timestamp,
        void *extrainfo);
