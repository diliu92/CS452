




typedef enum taskState_t{
	Ready,
	Active,
	Zombie,
	Send_blocked,
	Receive_blocked,
	Reply_blocked,
	Event_blocked 
}taskState_t;


typedef struct task{
	int tid;
	taskState_t state;
	int priority;
	int parent_tid;
	char* sp;
	
}task;
