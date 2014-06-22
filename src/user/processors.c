#include <user.h>

#define BUFFER_MAX_SIZE 256

void initUI(){
	//switch states
	sprintf( COM2, "\033[3;0H%sSwitch states:%s", yellow, resetColor);
	sprintf( COM2, "%s\033[4;4H1  : %sC%s\033[4;12H2  : %sC%s\033[4;20H3  : %sC%s\033[4;28H4  : %sC%s", 
		yellow, cyan, yellow, cyan, yellow, cyan, yellow, cyan, yellow);
	sprintf( COM2, "\033[5;4H5  : %sC%s\033[5;12H6  : %sC%s\033[5;20H7  : %sC%s\033[5;28H8  : %sC%s", 
		cyan, yellow, cyan, yellow, cyan, yellow, cyan, yellow);
	sprintf( COM2, "\033[6;4H9  : %sC%s\033[6;12H10 : %sC%s\033[6;20H11 : %sC%s\033[6;28H12 : %sC%s",
		cyan, yellow, cyan, yellow, cyan, yellow, cyan, yellow);
	sprintf( COM2, "\033[7;4H13 : %sC%s\033[7;12H14 : %sC%s\033[7;20H15 : %sC%s\033[7;28H16 : %sC%s",
		cyan, yellow, cyan, yellow, cyan, yellow, cyan, yellow);
	sprintf( COM2, "\033[8;4H17 : %sC%s\033[8;12H18 : %sC%s",
		cyan, yellow, cyan, yellow);
	sprintf( COM2, "\033[9;4H153: %sC%s\033[9;12H154: %sS%s\033[9;20H155: %sC%s\033[9;28H156: %sS%s", 
		cyan, yellow, magenta, yellow, cyan, yellow, magenta, resetColor);

	//recent snesor
	sprintf( COM2, "\033[12;0H%sRecently triggered sensors: (last to 5th last)%s", yellow, resetColor);

	//command
	sprintf( COM2, "\033[16;0H%scommand> %s", green, resetColor);
}

void updateSwitchState(int sn, int sv){
	switch(sn){
		case 1:
			sprintf(COM2, "\033[4;9H");
			break;
		case 2:
			sprintf(COM2, "\033[4;17H");
			break;
		case 3:
			sprintf(COM2, "\033[4;25H");
			break;
		case 4:
			sprintf(COM2, "\033[4;33H");
			break;
		case 5:
			sprintf(COM2, "\033[5;9H");
			break;
		case 6:
			sprintf(COM2, "\033[5;17H");
			break;
		case 7:
			sprintf(COM2, "\033[5;25H");
			break;
		case 8:
			sprintf(COM2, "\033[5;33H");
			break;
		case 9:
			sprintf(COM2, "\033[6;9H");
			break;
		case 10:
			sprintf(COM2, "\033[6;17H");
			break;
		case 11:
			sprintf(COM2, "\033[6;25H");
			break;
		case 12:
			sprintf(COM2, "\033[6;33H");
			break;
		case 13:
			sprintf(COM2, "\033[7;9H");
			break;
		case 14:
			sprintf(COM2, "\033[7;17H");
			break;
		case 15:
			sprintf(COM2, "\033[7;25H");
			break;
		case 16:
			sprintf(COM2, "\033[7;33H");
			break;
		case 17:
			sprintf(COM2, "\033[8;9H");
			break;
		case 18:
			sprintf(COM2, "\033[8;17H");
			break;
		case 153:
			sprintf(COM2, "\033[9;9H");
			break;
		case 154:
			sprintf(COM2, "\033[9;17H");
			break;
		case 155:
			sprintf(COM2, "\033[9;25H");
			break;
		case 156:
			sprintf(COM2, "\033[9;33H");
			break;
	}
	if (sv == 33){
		sprintf(COM2, "%sS", magenta);
	}
	else{
		sprintf(COM2, "%sC", cyan);
	}
}

int processCmd(char *cmd, int *trainSpeed){
	int i = 0, j = 0; 
	char cmdType[5];
	char cmdTarget[5];
	char cmdValue[5];
	int a;
	for ( a = 0; a < 5; a++){
		cmdType[a] = '\0';
		cmdTarget[a] = '\0';
		cmdValue[a] = '\0';
	}

	int value;
	int target;

	//get command type
	while (cmd[i] == ' ') i++;
	while (cmd[i] != ' ' && cmd[i] != '\0' && i < BUFFER_MAX_SIZE){
		if (j >= 2){
			return -1;
		}
		cmdType[j] = cmd[i % BUFFER_MAX_SIZE];
		i++;
		j++;
	}

	if (cmdType[0] == 'q' && cmdType[1] == '\0' && cmdType[2] == '\0'){
		return 1;
	}
	else if (cmdType[0] == 't' && cmdType[1] == 'r' && cmdType[2] == '\0'){
		value = 0;
	}
	else if (cmdType[0] == 'r' && cmdType[1] == 'v' && cmdType[2] == '\0'){
		value = 15;
	}
	else if (cmdType[0] == 's' && cmdType[1] == 'w' && cmdType[2] == '\0'){
		value = 33;
	}
	else{
		return -1;
	}

	//get command target
	j = 0;
	while (cmd[i] == ' ') i++;
	while (cmd[i] != ' ' && cmd[i] != '\0' && i < BUFFER_MAX_SIZE){
		if (j >= 3){
			return -1;
		}
		cmdTarget[j] = cmd[i % BUFFER_MAX_SIZE];
		i++;
		j++;
	}

	target = array2int(cmdTarget);

	if (value > 15){
		if (target <= 0 || 
			(target > 18 && !((target >= 153) && (target <= 156)))){
				return -1;
		}
	}
	else{
		if(target <= 0 || target > 80){
			return -1;
		}
	}

	//get command value
	j = 0;
	while (cmd[i] == ' ') i++;
	while (cmd[i] != ' ' && cmd[i] != '\0' && i < BUFFER_MAX_SIZE){
		if (j >= 2){
			return -1;
		}
		cmdValue[j] = cmd[i];
		i++;
		j++;
	}

	if (cmd[i] != '\0'){
		return -1;
	}
	else if (value > 15){
		if ((cmdValue[0] == 'S' || cmdValue[0] == 's') && 
			cmdValue[1] == '\0' && cmdValue[2] == '\0'){
			value = 33;
		} 
		else if ((cmdValue[0] == 'C' || cmdValue[0] == 'c') && 
			cmdValue[1] == '\0' && cmdValue[2] == '\0'){
			value = 34;
		} 
		else{
			return -1;
		}
		updateSwitchState(target, value);
		putc(COM1, value);
		putc(COM1, target);
		putc(COM1, 32);
	}
	else if (value < 15){
		value = array2int(cmdValue); 
		if (value < 0){
			return -1;
		}
		if (value % 16 != 15){
			trainSpeed[target - 1] = value;
			putc(COM1, value);
			putc(COM1, target);
		}
	}
	
	if (value == 15){
		putc(COM1, 0);
		putc(COM1, target);
		putc(COM1, value);
		putc(COM1, target);
		putc(COM1, trainSpeed[target-1]);
		putc(COM1, target);
	}

	sprintf(COM2, "\033[18;0H%s%sCommand '%s' processed.%s", clearLine, green, cmd, resetColor);
	return 0;
}

int array2int (char *ca){
	int num = 0;
	int base = 10;
	int digit;
	int i = 0;
	while ((digit = a2d(ca[i])) >= 0){
		if (digit >= base) {
			return -1;
		};
		num = num * base + digit;
		i++;
	} 
	return num;
}

void processFeed(int *sensorFeed, int *feedHistory){
	int cur, i, j, k;
	for(i = 0; i < 10; i++){
		cur = sensorFeed[i];
		if (cur != 0){
			int group = (i / 2) + 'A';
			int base = (i % 2) * 8;
			int id; 
			j++;
			switch(cur){ 
				case 128:
					id = base + 1;
					break;
				case 64:
					id = base + 2;
					break;				
				case 32:
					id = base + 3;
					break;
				case 16:
					id = base + 4;
					break;				
				case 8:
					id = base + 5;
					break;				
				case 4:
					id = base + 6;
					break;				
				case 2:
					id = base + 7;
					break;				
				case 1: 
					id = base + 8;
					break;
				default:
					return;
					break;
			}

			int newfeed = group * 17 + id;
			int lastfeed = feedHistory[0];
			if (newfeed != lastfeed) {
				for (j = 3; j >= 0; j--){
					feedHistory[j+1] = feedHistory[j];
				}
				feedHistory[0] = newfeed;
				sprintf(COM2, "%s\033[13;0H%s%s%s", save, yellow, clearLine, tab);
				putc(COM2, group);
				sprintf(COM2, "%d%s", id, tab);
				for (k = 1; k < 5; k++){
					if (feedHistory[k] > 0){
						putc(COM2, feedHistory[k] / 17);
						sprintf(COM2, "%d%s", feedHistory[k] % 17, tab);
					}
				}
				sprintf(COM2, "%s%s", resetColor, restore);
			}
		}
	}
}

void sensorFeedProcessor (){
	int sensorFeed[10];
	int feedHistory[5];
	int sensorCount = 0;

	int a;
	for (a = 0; a < 5; a++){
		feedHistory[a] = -1;
	}

	//init track
	putc(COM1, 96);
	int i = 0, j = 44;
	while (j >= 44 && j <= 54){
		putc(COM1, 0); 
		putc(COM1, j + 1);
		j++;
	}
	while (i < 22){
		if ((i == 19) || (i == 21)){ //154, 156
			putc(COM1, 33);
			putc(COM1, i + 135);
		}
		else if (i > 17){
			putc(COM1, 34);
			putc(COM1, i + 135);
		}
		else{
			putc(COM1, 34);
			putc(COM1, i + 1);
		}
		putc(COM1, 32);
		i++;
	}

	//send first query
	// putc(COM1, 192);
	// putc(COM1, 133);

	// while(1){
	// 	int feed = getc(COM1);
	// 	sensorFeed[sensorCount] = feed;
	// 	sensorCount++;
	// 	if (sensorCount == 10){
	// 		sensorCount = 0;
	// 		processFeed(sensorFeed, feedHistory);

	// 		putc(COM1, 192);
	// 		putc(COM1, 133);
	// 	}
	// }
	Exit();
}

void cmdProcessor (){
	int trainSpeed[80];

	char cmd[256];

	initUI();

	int j = 0;
	while (j < 80){
		trainSpeed[j] = 0; //stopped
		j++;
	}


	int i = 0;
	while(1){
		cmd[i] = getc(COM2);
		if (cmd[i] == '\r'){
			cmd[i] = '\0';
			int ret = processCmd(cmd, trainSpeed);
			switch (ret){
				case 1: //q
					sprintf(COM2, "\033[20;0H");
					//putc(COM1, 97);
					Exit();
					break;
				case -1:
					sprintf(COM2, "\033[18;0H%s%sInvalid command!%s", clearLine, red, resetColor);
					break;
			}
			sprintf(COM2, "\033[16;10H%s%s",clearLine,resetColor);
			i = 0;
		}
		else if (cmd[i] == '\b'){
			cmd[i] = '\0';
			cmd[i-1] = '\0';
			i--;
			sprintf(COM2, "\033[1D\033[K");
		}
		else{
			putc(COM2, cmd[i]);
			i++;
		}
	}
	//clearPutBuffers();
	Exit();
}