enum states {QuenchThirst, EnterMineDig, VisitBankDeposit,
	GoHomeAndSleep, Fight, CookStew, GetCookedStew, VisitBathroom, 
	DoHouseWork};

enum persontype {MINER, WIFE, BARFLY};
#define NUMP 3

struct person {
	int who; 
	char name[256];
	int state;
	int laststate;
	int gold;
	int moneyinbank;
	int thirst;
	int fatigue;
	int bladder;
};

extern struct person people[NUMP];
