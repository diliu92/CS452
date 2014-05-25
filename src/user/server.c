
#include <user.h>

static void
InitNameServer(nameServerTable* nameServerData){
	int i;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		nameServerData->entries[i].tid = i;
		nameServerData->entries[i].tid = i;
		
		nameServerData->entryStatus[i] = 0;
	}
	
}


void 
nameServer(){
	nameServerTable nameServerData;	
	syscallRequest_NameServer req;
	int tid, response;
	
	InitNameServer(&nameServerData);
		
	for (;;){
		Receieve(&tid, &req, sizeof(syscallRequest_NameServer));
		bwprintf( COM2, "NameServer: got a request\n");	
		
		switch (req.syscall_uid)
		{
			case SYSCALL_REGISTERAS:
				response = 3;
				Reply(tid,&response, sizeof(int));
				break;
			case SYSCALL_WHOIS:
			
				break;			
		}			
	}
	
}
