
#include <user.h>

static void
InitNameServer(nameServerTable* nameServerData){
	int i;
	
	for (i = 0; i < MAX_TASK; i++)
	{
		nameServerData->entries[i].tid = i;
		nameServerData->entries[i].nextSpot = 0;
		
		nameServerData->entryStatus[i] = 0;
	}
	
}

static int
findMatchInNameServerEntry(nameServerEntry* entry, int howMany, char* matcher, int matcher_size){
	int i;
	for (i = 0; i < howMany; i++)
	{
		if( entry->names[i].size == matcher_size){
			if(strEnq(entry->names[i].data, matcher))
				return 1;			
		}			
	}
	return 0;
}


void 
nameServer(){
	nameServerTable nameServerData;	
	syscallRequest_NameServer req;
	int tid, reply;
	
	InitNameServer(&nameServerData);
		
	for (;;){
		Receieve(&tid, &req, sizeof(syscallRequest_NameServer));
				
		switch (req.syscall_uid)
		{
			case SYSCALL_REGISTERAS:
			{
				// error checking to be added
				
				nameServerEntry* entry = &(nameServerData.entries[req.tid]);
				nameEntry* name = &(entry->names[entry->nextSpot]);
				
				name->size = req.size;
				memcpy(req.name, name->data, (req.size) + 1);
				
				entry->nextSpot = (entry->nextSpot + 1) % MAX_NAME_ENTRY_PER_TID;
				nameServerData.entryStatus[req.tid] = nameServerData.entryStatus[req.tid] == MAX_NAME_ENTRY_PER_TID ?
														MAX_NAME_ENTRY_PER_TID : nameServerData.entryStatus[req.tid] + 1 ;
			
				reply = 0;				
				Reply(tid,&reply, sizeof(int));
				break;
			}
			case SYSCALL_WHOIS:
			{
				int i, howMany;
				//nameServerEntry* entry;
				
				reply = -3;	//not found
				for (i = 0; i < MAX_TASK; i++)
				{
					nameServerEntry* entry = &(nameServerData.entries[i]);
					howMany = nameServerData.entryStatus[i];
					
					if(howMany){
						if (findMatchInNameServerEntry(entry, howMany, req.name, req.size)){
							reply = entry->tid;
							break;
						}
					}
				}

				Reply(tid,&reply, sizeof(int));
				break;	
			}		
		}			
	}
	
}
