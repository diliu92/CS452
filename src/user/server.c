
#include <user.h>

static void
InitNameServer(){
	return;//to be added
}


void 
nameServer(){
	syscallRequest_NameServer req;
	int tid, response;
		
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
