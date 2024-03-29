#include <user.h>
#include <train.h>

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
	sprintf( COM2, "\033[13;0HExpect Sensor:      at: ");
	sprintf( COM2, "\033[15;0HPrevious sensor: ");
	sprintf( COM2, "\033[16;4HActual time: ");
	sprintf( COM2, "\033[17;4HExpected time: ");
	sprintf( COM2, "\033[18;4HTime difference: %s", resetColor);


	sprintf( COM2, "\033[21;0H%sCurrent Location  ", yellow);
	sprintf( COM2, "\033[22;0H%sCurrent Location  ", yellow);
	sprintf( COM2, "\033[23;0H%sCurrent Location  ", yellow);
	// sprintf( COM2, "\033[23;0H%sDistance between tr45 and tr48: ", yellow);

	//Train status
	// sprintf( COM2, "\033[4;40HCurrent Train Direction:      Forward");
	sprintf( COM2, "\033[5;40HTrain 45 Speed:          0");	
	sprintf( COM2, "\033[6;40HTrain 48 Speed:          0");	
	sprintf( COM2, "\033[7;40HTrain 54 Speed:          0");	

	//track
	// sprintf( COM2, "\033[20;0H%s*  *  *  *  A $12 *  * $11 *  *  A  *  *  *  *  *  A  *  *  * ", "");
	// sprintf( COM2, "\033[21;0H%s              *       *                                       *", "");
	// sprintf( COM2, "\033[22;0H%s*  *  *  A $04      *   *  A  *  * $13 A  *  A $10 A  *  *  *   A", "");
	// sprintf( COM2, "\033[23;0H%s         *           *                A       A                A", "");
	// sprintf( COM2, "\033[24;0H%s*  A  *            $14                  A * A                  09$", "");
	// sprintf( COM2, "\033[25;0H%s                  A                   156$ $155                   *", "");
	// sprintf( COM2, "\033[26;0H%s                  *                       *                       *", "");
	// sprintf( COM2, "\033[27;0H%s                  *                       *                       *", "");
	// sprintf( COM2, "\033[28;0H%s                  A                   153$ $154                   *", "");
	// sprintf( COM2, "\033[29;0H%s*  A  *            $15                  A * A                  08$", "");
	// sprintf( COM2, "\033[30;0H%s         *           *                A       A                A", "");
	// sprintf( COM2, "\033[31;0H%s*  A  *  A $01      *   *  A  *  * $16 A  *  A $17 *  A  *  *   A", "");
	// sprintf( COM2, "\033[32;0H%s               *      *                                       *", "");
	// sprintf( COM2, "\033[33;0H%s*  A  *  *  *  A $02    *  *  A $06 *  A  *  A  * $07 A  *  *", "");
	// sprintf( COM2, "\033[34;0H%s                     *             *             *", "");
	// sprintf( COM2, "\033[35;0H%s*  A  *  *  *  *  A  * $03 *  *  A $18 *  *  * $05 A  *  *  *  *  *  *  *  *", "");

	//command
	sprintf( COM2, "\033[38;0H%scommand> %s", green, resetColor);
}

void updateSwitchState(int sn, int sv){
	int r, c;

	r = 4 + (sn/4);
			c = ((sn%4) * 8) + 1; 
			sprintf(COM2, "\033[4;9H");

	switch(sn){
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
		default:
			r = 4 + ((sn-1) /4);
			c = ((((sn - 1) %4) +1) * 8) + 1; 
			sprintf(COM2, "\033[%u;%uH", r, c);
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
	else if (cmdType[0] == 'g' && cmdType[1] == 'o' && cmdType[2] == '\0'){
		value = 1;
	}
	else if (cmdType[0] == 'r' && cmdType[1] == 'v' && cmdType[2] == '\0'){
		value = 15;
	}
	else if (cmdType[0] == 's' && cmdType[1] == 'w' && cmdType[2] == '\0'){
		value = 33;
	}
	else if (cmdType[0] == 'i' && cmdType[1] == 't' && cmdType[2] == '\0'){
		value = -1;
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

	

	if (value > 15 && 
		(target <= 0 || (target > 18 && !((target >= 153) && (target <= 156))))){
			return -1;
	}
	else if ((value == 1) && (target <= 0 || target > 80)){
		return -1;
	}
	else if(value <= 15 && (target <= 0 || target > 80)){
		return -1;
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

	if (value != 1 && cmd[i] != '\0'){
		return -1;
	}
	else if (value == -1){
		value = array2int(cmdValue);
		initTrain(target, value);
	}
	else if (value == 1){	//go cmd
		value = array2int(cmdValue); 

		char cmdValue2[5];
		j = 0;
		while (cmd[i] == ' ') i++;

		int group = cmd[i];
		i++;

		while (cmd[i] != ' ' && cmd[i] != '\0' && i < BUFFER_MAX_SIZE){
			if (j >= 3){
				return -1;
			}
			cmdValue2[j] = cmd[i % BUFFER_MAX_SIZE];
			i++;
			j++;
		}
		int id = array2int(cmdValue2);

		if (group >= 'A' && group <= 'E' && id >= 1 && id <= 16){
			int dest = group * 17 + id;
			int speed = value;
			sprintf(COM2, "%s\033[7;40H%sExpect train to stop at: %c%d + %d%s", 
				save, clearLine, (char)group, id, value, restore);
			GoTo(target, speed, dest);
		}
		else{
			return -1;
		}
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
		changeSwitchStatus(target, value);
	}
	else if (value < 15){
		value = array2int(cmdValue); 
		if (value < 0){
			return -1;
		}
		if (value % 16 != 15){
			trainSpeed[target - 1] = value;
			// putc(COM1, value);
			// putc(COM1, target);
			changeTrainSpeed(target, value);
		}
	}
	else if (value == 15){
		reverseTrain(target);
	}

	sprintf(COM2, "\033[39;0H%s%sCommand '%s' processed.%s", clearLine, green, cmd, resetColor);
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

int processFeed(int *sensorFeed, int *triggeredSensors){
	int cur, i;
	int x = 0;
	for(i = 0; i < 10; i++){
		cur = sensorFeed[i];
		if (cur != 0){
			int group = (i / 2) + 'A';
			int base = (i % 2) * 8;
			int id; 

			int offset = 128;
			int j = 1;
			while (cur > 0){
				if (cur >= offset){
					id = base + j;
					triggeredSensors[x] = group * 17 + id;

					cur = cur - offset;
					x++;
				}
				offset = offset / 2;
				j++;
			}
		}
	}
	return x;
}

int highlightSensor(int sensor, int prevLoc){
	int r,c;
	if (prevLoc > 0){
		r = prevLoc / 100;
		c = prevLoc % 100;
		sprintf(COM2, "%s\033[%u;%uH%sA%s", save, r, c, resetColor, restore);
	}
	r = 0;
	c = 0;
	//sprintf(COM2, "%d\r\n" ,sensor);

	switch(sensor){
		case ('A' * 17 + 1):
		case ('A' * 17 + 2):
			r = 1;
			c = 12;
			break;
		case ('A' * 17 + 3):
		case ('A' * 17 + 4):
			r = 6;
			c = 18;
			break;
		case ('A' * 17 + 5):
		case ('A' * 17 + 6):
			r = 16;
			c = 18;
			break;
		case ('A' * 17 + 7):
		case ('A' * 17 + 8):
			r = 14;
			c = 15;
			break;
		case ('A' * 17 + 9):
		case ('A' * 17 + 10):
			r = 12;
			c = 9;
			break;
		case ('A' * 17 + 11):
		case ('A' * 17 + 12):
			r = 10;
			c = 3;
			break;
		case ('A' * 17 + 13):
		case ('A' * 17 + 14):
			r = 3;
			c = 9;
			break;
		case ('A' * 17 + 15):
		case ('A' * 17 + 16):
			r = 5;
			c = 3;
			break;
		case ('B' * 17 + 1):
		case ('B' * 17 + 2):
			r = 12;
			c = 39;
			break;
		case ('B' * 17 + 3):
		case ('B' * 17 + 4):
			r = 11;
			c = 38;
			break;
		case ('B' * 17 + 5):
		case ('B' * 17 + 6):
			r = 3;
			c = 39;
			break;
		case ('B' * 17 + 7):
		case ('B' * 17 + 8):
			r = 12;
			c = 3;
			break;
		case ('B' * 17 + 9):
		case ('B' * 17 + 10):
			r = 16;
			c = 3;
			break;
		case ('B' * 17 + 11):
		case ('B' * 17 + 12):
			r = 14;
			c = 3;
			break;
		case ('B' * 17 + 13):
		case ('B' * 17 + 14):
			r = 10;
			c = 44;
			break;
		case ('B' * 17 + 15):
		case ('B' * 17 + 16):
			r = 9;
			c = 18;
			break;
		case ('C' * 17 + 1):
		case ('C' * 17 + 2):
			r = 10;
			c = 40;
			break;
		case ('C' * 17 + 3):
		case ('C' * 17 + 4):
			r = 16;
			c = 51;
			break;
		case ('C' * 17 + 5):
		case ('C' * 17 + 6):
			r = 14;
			c = 30;
			break;
		case ('C' * 17 + 7):
		case ('C' * 17 + 8):
			r = 16;
			c = 33;
			break;
		case ('C' * 17 + 9):
		case ('C' * 17 + 10):
			r = 12;
			c = 27;
			break;
		case ('C' * 17 + 11):
		case ('C' * 17 + 12):
			r = 3;
			c = 27;
			break;
		case ('C' * 17 + 13):
		case ('C' * 17 + 14):
			r = 1;
			c = 33;
			break;
		case ('C' * 17 + 15):
		case ('C' * 17 + 16):
			r = 14;
			c = 39;
			break;
		case ('D' * 17 + 1):
		case ('D' * 17 + 2):
			r = 5;
			c = 44;
			break;
		case ('D' * 17 + 3):
		case ('D' * 17 + 4):
			r = 3;
			c = 45;
			break;
		case ('D' * 17 + 5):
		case ('D' * 17 + 6):
			r = 4;
			c = 63;
			break;
		case ('D' * 17 + 7):
		case ('D' * 17 + 8):
			r = 3;
			c = 64;
			break;
		case ('D' * 17 + 9):
		case ('D' * 17 + 10):
			r = 12;
			c = 64;
			break;
		case ('D' * 17 + 11):
		case ('D' * 17 + 12):
			r = 14;
			c = 45;
			break;
		case ('D' * 17 + 13):
		case ('D' * 17 + 14):
			r = 12;
			c = 45;
			break;
		case ('D' * 17 + 15):
		case ('D' * 17 + 16):
			r = 11;
			c = 46;
			break;
		case ('E' * 17 + 1):
		case ('E' * 17 + 2):
			r = 5;
			c = 40;
			break;
		case ('E' * 17 + 3):
		case ('E' * 17 + 4):
			r = 4;
			c = 46;
			break;
		case ('E' * 17 + 5):
		case ('E' * 17 + 6):
			r = 3;
			c = 51;
			break;
		case ('E' * 17 + 7):
		case ('E' * 17 + 8):
			r = 1;
			c = 51;
			break;
		case ('E' * 17 + 9):
		case ('E' * 17 + 10):
			r = 11;
			c = 63;
			break;
		case ('E' * 17 + 11):
		case ('E' * 17 + 12):
			r = 14;
			c = 54;
			break;
		case ('E' * 17 + 13):
		case ('E' * 17 + 14):
			r = 12;
			c = 54;
			break;
		case ('E' * 17 + 15):
		case ('E' * 17 + 16):
			r = 4;
			c = 38;
			break;
	}


	int loc = -1;
	if (r != 0 && c != 0){
		r = r + 19;
		c = c + 1;
		loc = r * 100 + c;
		sprintf(COM2, "%s\033[%u;%uH%sA%s", save, r, c, magenta, restore);
	}
	return loc;	
}

int sensorFeedProcessor (){
	int sensorFeed[10];
	int sensorCount = 0;

	//send first query
	putc(COM1, 192);
	putc(COM1, 133);


	int numSensorTriggered = 0;
	int triggeredSensors[10];
	int t = 0;


	while(1){
		int feed = getc(COM1);
		sensorFeed[sensorCount] = feed;
		sensorCount++;
		if (sensorCount == 10){
			sensorCount = 0;
			t = Time();
			numSensorTriggered = processFeed(sensorFeed, triggeredSensors);
			putc(COM1, 192);
			putc(COM1, 133);

			if (numSensorTriggered > 0){
				int i;				
				for (i = 0; i < numSensorTriggered; i++){
					if (triggeredSensors[i] > 'A' * 17){
						updateLastTriggeredSensor(triggeredSensors[i], t);
					}
				}
			};
		}
	 }

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
				case 1: 
					putc(COM1, 97);	
					sprintf(COM2, "\033[39;0H%sShutting down\r\n",clearLine);
					Delay(300);	//Magic number
					ShutDown();
					break;
				case -1:
					sprintf(COM2, "\033[39;0H%s%sInvalid command!%s", clearLine, red, resetColor);
					break;
				case -2:
					sprintf(COM2, "\033[39;0H%s%sCannot accomplish command%s", clearLine, red, resetColor);
			}
			sprintf(COM2, "\033[38;10H%s%s",clearLine,resetColor);
			i = 0;
		}
		else if (cmd[i] == '\b'){
			if (i > 0){
				cmd[i] = '\0';
				cmd[i-1] = '\0';
				i--;
				sprintf(COM2, "\033[1D\033[K");
			}
		}
		else{
			putc(COM2, cmd[i]);
			i++;
		}
	}

	Send(0, NULL, 0, NULL, 0);
	//clearPutBuffers();
	Exit();
}

void showTrainLocation1(){
	int displacement, id, group;
	locationInfo locInfo;
	int trainNo = 45;

	while(!NeedToShutDown()){
		locInfo = getTrainLocation(trainNo); 
		if (locInfo.sensor > 'A'*17){
			group = locInfo.sensor / 17;
			id = locInfo.sensor % 17; 
			displacement = locInfo.displacement;
			sprintf(COM2, "%s\033[21;18H%sTrain %d - %c%d + %dmm%s", 
				save, clearLine, trainNo, (char)group, id, displacement, restore);
		}
		Delay(50);
	}

	Exit();
}

void showTrainLocation2(){
	int displacement, id, group;
	locationInfo locInfo;
	int trainNo = 48;

	while(!NeedToShutDown()){
		locInfo = getTrainLocation(trainNo);
		if (locInfo.sensor > 'A'*17){
		 	group = locInfo.sensor / 17;
		 	id = locInfo.sensor % 17; 
		 	displacement = locInfo.displacement;
		 	sprintf(COM2, "%s\033[22;18H%sTrain %d - %c%d + %dmm%s", 
		 		save, clearLine, trainNo, (char)group, id, displacement, restore);
		}
		Delay(50);
	}

	Exit();
}

void showTrainLocation3(){
	int displacement, id, group;
	locationInfo locInfo;
	int trainNo = 54;

	while(!NeedToShutDown()){
		locInfo = getTrainLocation(trainNo);
		if (locInfo.sensor > 'A'*17){
		 	group = locInfo.sensor / 17;
		 	id = locInfo.sensor % 17; 
		 	displacement = locInfo.displacement;
		 	sprintf(COM2, "%s\033[23;18H%sTrain %d - %c%d + %dmm%s", 
		 		save, clearLine, trainNo, (char)group, id, displacement, restore);
		}
		Delay(50);
	}

	Exit();
}

void initTrack(){
	DelayUntil(500);
	initTrain(45, 2);
	Delay(300);

	initTrain(48, 3);
	Delay(300);

	initTrain(54, 1);
	Delay(300);

	updateSwitchState(9, 33);
	changeSwitchStatus(9, 33);

	updateSwitchState(16, 33);
	changeSwitchStatus(16, 33);

	changeTrainSpeed(45, 9);
	Delay(80);
	changeTrainSpeed(48, 9);
	Delay(130);
	changeTrainSpeed(54, 9);

	Exit();
}

void DistManager1(){
	int diff = -1;
	locationInfo locInfo45;
	locationInfo locInfo48;

	while(!NeedToShutDown()){
		locInfo45 = getTrainLocation(45);
		locInfo48 = getTrainLocation(48);

		if (locInfo45.sensor > 'A'*17 && locInfo48.sensor > 'A'*17){
			if (locInfo45.sensor == locInfo48.sensor){
				diff = locInfo45.displacement - locInfo48.displacement;
			}
			else {
				int sensorDist = getDistBetweenSensors(locInfo48.sensor, locInfo45.sensor);

				if (sensorDist > 0){
					diff = sensorDist + locInfo45.displacement - locInfo48.displacement;
				}
				else{
					diff = -1;
				}
			}
		}

		if (diff >= 0){
			if (diff > 280){
				changeTrainSpeed(48, 10);
				Delay((diff - 280) * 1000 / 4490);
				changeTrainSpeed(48, 9);
			}
			else if (diff < 280){
				changeTrainSpeed(48, 8);
				Delay((280 - diff) * 1000 / 4005);
				changeTrainSpeed(48, 9);
			}
			// sprintf(COM2, "%s\033[23;32H%s%dmm%s", 
		 // 		save, clearLine, diff, restore);
		}

		Delay(25);
	}

	Exit();
}

void DistManager2(){
	int diff = -1;
	locationInfo locInfo45;
	locationInfo locInfo54;
	
	while(!NeedToShutDown()){
		locInfo45 = getTrainLocation(45);
		locInfo54 = getTrainLocation(54);

		if (locInfo45.sensor > 'A'*17 && locInfo54.sensor > 'A'*17){
			if (locInfo45.sensor == locInfo54.sensor){
				diff = locInfo45.displacement - locInfo54.displacement;
			}
			else {
				int sensorDist = getDistBetweenSensors(locInfo54.sensor, locInfo45.sensor);

				if (sensorDist > 0){
					diff = sensorDist + locInfo45.displacement - locInfo54.displacement;
				}
				else{
					diff = -1;
				}
			}
		}

		if (diff >= 0){
			if (diff > 650){
				changeTrainSpeed(54, 10);
				Delay((diff - 650) * 1000 / 4508);
				changeTrainSpeed(54, 9);
			}
			else if (diff < 650){
				changeTrainSpeed(54, 8);
				Delay((650 - diff) * 1000 / 4020);
				changeTrainSpeed(54, 9);
			}
			// sprintf(COM2, "%s\033[23;32H%s%dmm%s", 
		 // 		save, clearLine, diff, restore);
		}

		Delay(25);
	}

	Exit();
}