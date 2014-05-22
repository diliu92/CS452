#include <user.h>



static void
kernelPartOne_code(){
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Pass();
	bwprintf( COM2, "My TID: %u, My Parent TID:%u\r\n", MyTid(), MyParentTid());
	Exit();	
}






void
kernelPartOne(){	//priority of FUT is 3
	bwprintf( COM2, "First:entering\r\n" );
	
	int Tid_low1 = Create(4,&kernelPartOne_code);
	int Tid_low2 = Create(4,&kernelPartOne_code);
	int Tid_high1 = Create(2,&kernelPartOne_code);
	int Tid_high2 = Create(2,&kernelPartOne_code);	
	
	bwprintf( COM2, "First:exiting\r\n" );
	
	Exit();	
}
