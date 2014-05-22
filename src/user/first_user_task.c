
#include <user.h>



void
kernelPartOne_code(){
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Pass();
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Exit();	
}






void
kernelPartOne(){	//priority of FUT is 3

	int Tid_low1 = Create(4,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_low1);
		
	int Tid_low2 = Create(4,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_low2);
	
	int Tid_high1 = Create(2,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_high1);
		
	int Tid_high2 = Create(2,kernelPartOne_code);
	bwprintf( COM2, "Created: %u\r\n", Tid_high2);	
	
	bwprintf( COM2, "First:exiting\r\n" );
	
	Exit();	
}
