// #include <train.h>

// void
// routeServer(){
// 	track_node trackA[TRACK_MAX];
// 	init_tracka(trackA);

// 	int requester;
// 	routeServerRequest req;
	
// 	while (1)
// 	{
// 		Receive(&requester, &req, sizeof(routeServerRequest));
		
// 		switch (req.rtSvrReq_uid)
// 		{
// 			case ROUTESERVER_PREDICT_NEXT_SENSOR:
// 			{	
// 				int group = req.curValue / 17;
// 				int id = req.curValue % 17;
// 				int index = (id + (group - 'A') * 16) - 1;

// 				track_node *nextLandmark = trackA[index].edge[DIR_AHEAD].dest;
// 				int totalDist = trackA[index].edge[DIR_AHEAD].dist;

// 				while(nextLandmark->type != NODE_SENSOR && nextLandmark->type != NODE_EXIT){
// 					int direction = DIR_AHEAD;
// 					if(nextLandmark->type == NODE_BRANCH){
// 						int status = getSwitchStatus(nextLandmark->num);
// 						switch(status){
// 							case STRAIGHT:
// 								direction = DIR_STRAIGHT;
// 								break;
// 							case CURVED:
// 								direction = DIR_CURVED;
// 								break;
// 						}
// 					}
// 					totalDist += nextLandmark->edge[direction].dist;
// 					nextLandmark = nextLandmark->edge[direction].dest;
// 				}

// 				nextSensorInfo response;
// 				response.nextSensorLabel = nextLandmark->name;
// 				response.dist = totalDist;

// 				Reply(requester, &response, sizeof(nextSensorInfo));
// 				break;
// 			}			
// 		}
// 	}
// }

// nextSensorInfo
// predictNextSensor(int curSensor, int ts){
// 	routeServerRequest req;

// 	req.rtSvrReq_uid = ROUTESERVER_PREDICT_NEXT_SENSOR;
// 	req.curValue = curSensor;
// 	req.ts = ts;

// 	nextSensorInfo retval;

// 	Send(ROUTESERVER_TID, &req, sizeof(routeServerRequest), &retval, sizeof(nextSensorInfo));

// 	return retval;
// }