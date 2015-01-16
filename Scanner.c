void nextLine()
{
  nMotorEncoder[motorA]=0;
  motor[motorA] = -8;
  motor[motorC] = -8;
  while (nMotorEncoder[motorA]>-6);//######################################
  motor[motorA] = 0;
  motor[motorC] = 0;
}

bool readPoint()
{
	if(SensorValue[S1] < 20) //if black
		return true;
	return false;
}

void printPoint(int row, int col, bool b)
{
	if(b) nxtSetPixel(row, col);
}

void donePrinting()
{
	motor[motorB] = 0;
	wait1Msec(500);
	motor[motorB] = -20;
	while(SensorValue[S3] != 1);
	motor[motorB] = 0;

	nMotorEncoder[motorA]=0;
	motor[motorA] = motor[motorC] = 50;
	while (nMotorEncoder[motorA] > -360*10);//##############################
	motor[motorA] = 0;
	motor[motorC] = 0;
}

task main()
{
	//sensor setup
	SensorType[S1] = sensorLightActive;
	SensorType[S2] = sensorCOLORFULL;
	SensorType[S3] = sensorTouch;

	bool stopScan = false;
	eraseDisplay();

	wait1Msec(1000);

  //left triangle
	nxtDrawLine(10,20,25,30);
	nxtDrawLine(10,20,25,10);
	nxtDrawLine(25,10,25,30);
  //right triangle
	nxtDrawLine(90,20,75,30);
	nxtDrawLine(90,20,75,10);
	nxtDrawLine(75,10,75,30);
  //rectangle
	nxtDrawRect(38,30,62,10);
  //prompts
	nxtDisplayStringAt(3,60,"Resolution:");
	nxtDisplayStringAt(42,45,"Low");
	nxtDisplayStringAt(7,45,"High");
	nxtDisplayStringAt(72,45,"Mid");
  //get user button press for resolution
	while (nNxtButtonPressed == -1);
	int resolution = nNxtButtonPressed; //left; 2med. middle; 3big. right; 1small.
	while (nNxtButtonPressed != -1);

  //touch for start

	eraseDisplay();
	nxtDisplayTextLine(1, "Place paper in");
	nxtDisplayTextLine(2, "front of the");
	nxtDisplayTextLine(3, "wheel");
  //color sensor read table or paper

	int tableColor = SensorValue[S2];
	while (SensorValue[S2] == tableColor);

	eraseDisplay();

	int color = SensorValue[S1];
	nMotorEncoder[motorA]=0;
	motor[motorA] = motor[motorC] = -5;
	while (abs(SensorValue[S1] - color) <2); //##################################
	motor[motorA] =0;
	motor[motorC] = 0;

  //take in paper a little bit more for encoder
	nMotorEncoder[motorA]=0;
	motor[motorA] = motor[motorC] = -5;
	while (nMotorEncoder[motorA] > -90); //##################################
	motor[motorA] =0;
	motor[motorC] = 0;

  //START TIMER HERE
	timer[T1] = 0;

  //take in paper at 10% power for correct distance - (100/resolution)*0.11 inches in encoder
	for (int i=0; i<100 && (stopScan == false); i++)
	{
		//move row down
		nextLine();

		//sensor moves 64 pixels (each pixel is multiplier of resolution) number of degrees on the encoder
		nMotorEncoder[motorB]=0;
		for (int j=0; j<64 && (stopScan == false); j++)
		{
			while(abs(nMotorEncoder[motorB])< j*resolution*2);//#######################
			//motor[motorB] = 0;

			if (i%2.0 == 1) printPoint(i,j,readPoint());
			if (i%2.0 == 0) printPoint(i,24*resolution-j,readPoint());

			motor[motorB] = -40*(i%2.0 - 0.5);

			if (nNxtButtonPressed == 3) stopScan = true;
		}
	}
	//spit paper back out, get ready to scan next paper

	float timeTaken = timer[T1] / 1000.0;
	donePrinting();

	//press button to end
	while (nNxtButtonPressed == -1);
	while (nNxtButtonPressed != -1);

	eraseDisplay();
	nxtDisplayTextLine(1, "took %f seconds",timeTaken);
  	while (nNxtButtonPressed == -1);
	while (nNxtButtonPressed != -1);
}
