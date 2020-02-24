#pragma config(Motor,  motor1,          Left,          tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor2,          Intake,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor3,          BallRelease,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor4,         BallLift,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor5,         IntakeLift,       tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor6,          Right,         tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Sensor, port7,  Main_Gyro,      sensorVexIQ_Gyro)
#pragma config(Sensor, port8,  LED,            sensorVexIQ_LED)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#pragma config(Sensor, port8,  CrossColor,     sensorVexIQ_ColorGrayscale)
//#pragma config(Sensor, port12, BallDetect,     sensorVexIQ_Touch)
//*!!Code automatically generated by 'ROBOTC' configuration wizard on 13/07/2019, 1448 hours           !!*//

//VIQC_SquaredAway_CompCode
/*************************************PROGRAMMER NOTES*****************************************************/
//
/**********************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																			 				Variables																									//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool boot = false;
bool intakeStarted;
bool BallReleased = false;
bool BatteryWarnTrig = false;
bool BallLiftToggle = false;

#define   DATALOG_SERIES_0    0
#define   DATALOG_SERIES_1    1
#define   DATALOG_SERIES_2    2
#define   DATALOG_SERIES_3    3
#define   DATALOG_SERIES_4    4
#define   DATALOG_SERIES_5    5
#define   DATALOG_SERIES_6    6
#define   DATALOG_SERIES_7    7
#define   DATALOG_SERIES_8    8

#define		IntakeSpeed		100

#define		MinimumVoltage		6500
#define		WarningVoltage		7000
#define		SafeVoltage		8500
#define		WarningDelay		0

/*
int   global_1 = 0;
int   global_2 = 0;
int   global_3 = 0;
int   global_4 = 0;
int   global_5 = 0;
int   global_6 = 0;
int   global_7 = 0;
int   global_8 = 0;
*/

long gyroValue;
long gyroError;

task Battery() {
	while (true) {
		if (nImmediateBatteryLevel<MinimumVoltage) {
			delay(WarningDelay);
			playRepetitiveSound(soundCarAlarm4, 100);
			setTouchLEDColor(LED, colorRed);
			setTouchLEDBlinkTime(LED, 2, 1);
			if (nImmediateBatteryLevel<5500) {
				boot = false;
				stopAllTasks();
				stopAllMotors();
			};
			} else if (nImmediateBatteryLevel<WarningVoltage && !BatteryWarnTrig) {
			delay(WarningDelay);
			playSound(soundHeadlightsOff);
			setTouchLEDColor(LED, colorOrange);
			setTouchLEDBlinkTime(LED, 2, 1);
			BatteryWarnTrig = true;
			sleep(3000);
			setTouchLEDBlinkTime(LED, 0, 0);
			}else if (nImmediateBatteryLevel>SafeVoltage) {
			delay(WarningDelay);
			playRepetitiveSound(soundWrongWay, 100);
			setTouchLEDColor(LED, colorRed);
			setTouchLEDBlinkTime(LED, 2, 1);
		};
	};
};

void GyroCustomCalibration(int count = 30) {
	startGyroCalibration( Main_Gyro, gyroCalibrateSamples512 );
	// delay so calibrate flag can be set internally to the gyro
	wait1Msec(100);

	// wait for calibration to finish or 2 seconds, whichever is longer
	while( getGyroCalibrationFlag(Main_Gyro) || (count-- > 0) ) {
		wait1Msec(100);
	} resetGyro(Main_Gyro);
};
/*
task DataCollection (){
int loops = 0;
datalogClear();
while(true){
global_1 = getGyroHeading(Main_Gyro); //series 1
global_2 = gyroValue; // 2
global_3 = gyroError; // 3
global_4 =  //4
global_5 = getMotorSpeed(Right); //5
global_6 = getMotorSpeed(Left); //6
global_7 =  //7
global_8 =  //8

datalogDataGroupStart();
datalogAddValue( DATALOG_SERIES_0, global_1 );
datalogAddValue( DATALOG_SERIES_1, global_2 );
datalogAddValue( DATALOG_SERIES_2, global_3 );
datalogAddValue( DATALOG_SERIES_3, global_4 );
datalogAddValue( DATALOG_SERIES_4, global_5 );
datalogAddValue( DATALOG_SERIES_5, global_6 );
datalogAddValue( DATALOG_SERIES_6, global_7 );
datalogAddValue( DATALOG_SERIES_7, global_8 );
datalogDataGroupEnd();

wait1Msec(10);
datalogAddValueWithTimeStamp( DATALOG_SERIES_3, global_3++ );
wait1Msec(10);
datalogAddValueWithTimeStamp( DATALOG_SERIES_3, global_3++ );

// Repeat sequence every 360 loops
if(loops++ == 360)
loops = 0;

// loop delay
wait1Msec(10);
}
}
*/
task gyroTask() {
	long velocity;
	long angle, lastAngle;

	setGyroSensitivity(Main_Gyro, gyroNormalSensitivity);
	resetGyro(Main_Gyro);
	wait1Msec(1000);

	while (true) {
		velocity = getGyroRate(Main_Gyro);
		angle = getGyroHeading(Main_Gyro);
		if (abs(velocity )<2) {
			if (angle!=lastAngle)
				gyroError+=lastAngle-angle;
		}
		lastAngle = angle;
		gyroValue = angle+gyroError;
		wait1Msec(10);
	}
}

task displayControl () {
	while (true) {
		int delayforscroll = 2000;
		//Display Code
		displayTextLine(0,"Battery (MV) = %d", nImmediateBatteryLevel);
		displayTextLine(1,"Gyro Real = %d", getGyroDegrees(Main_Gyro)); // displays the gyro value in degrees
		displayTextLine(2,"Gyro Error = %d", gyroError);
		displayTextLine(3,"Gyro Actual = %d", gyroValue);
		displayTextLine(4,"blank");


		delay(delayforscroll);
	};

};

void load(int screenrate = 60) {
	delay(screenrate);	displayTextLine(3,"/");
	delay(screenrate);	displayTextLine(3,"//");
	delay(screenrate);	displayTextLine(3,"///");
	delay(screenrate);	displayTextLine(3,"////");
	delay(screenrate);	displayTextLine(3,"/////");
	delay(screenrate);	displayTextLine(3,"//////");
	delay(screenrate);	displayTextLine(3,"///////");
	delay(screenrate);	displayTextLine(3,"////////");
	delay(screenrate);	displayTextLine(3,"/////////");
	delay(screenrate);	displayTextLine(3,"//////////");
	delay(screenrate+500);	displayTextLine(3,"///////////");
	delay(screenrate);	displayTextLine(3,"////////////");
	delay(screenrate);	displayTextLine(3,"/////////////");
	delay(screenrate);	displayTextLine(3,"//////////////");
	delay(screenrate);	displayTextLine(3,"///////////////");
	delay(screenrate+300);	displayTextLine(3,"////////////////");
	delay(screenrate);	displayTextLine(3,"/////////////////");
	delay(screenrate);	displayTextLine(3,"//////////////////");
	delay(screenrate);	displayTextLine(3,"///////////////////");
	delay(screenrate);	displayTextLine(3,"////////////////////");
	delay(screenrate);	displayTextLine(3,"/////////////////////");
	delay(screenrate+300);	displayTextLine(3,"//////////////////////");
	delay(screenrate);	displayTextLine(3,"///////////////////////");
	delay(screenrate);	displayTextLine(3,"////////////////////////");
	delay(screenrate);	displayTextLine(3,"////////////////////////");
	delay(screenrate);	displayTextLine(3,"//////////////////////////");
	delay(screenrate);	displayTextLine(3,"///////////////////////////");
	delay(screenrate);	displayTextLine(3,"////////////////////////////");
	delay(screenrate);	displayTextLine(3,"/////////////////////////////");
	delay(screenrate);	displayTextLine(3,"//////////////////////////////");
	delay(screenrate);	displayTextLine(3,"///////////////////////////////");
}

void activate() {
	boot = true;
};

task bootup() {
	displayTextLine(0,"		2917X Worlds Code");
	displayTextLine(1,"		Setting Up Motors");
	setTouchLEDColor(LED, colorYellow);

	load();
	setMotorBrakeMode(Left, motorHold);
	setMotorBrakeMode(Intake, motorHold);
	setMotorBrakeMode(BallRelease, motorCoast);
	setMotorBrakeMode(BallLift, motorHold);
	setMotorBrakeMode(IntakeLift, motorHold);
	setMotorBrakeMode(Right, motorHold);
	resetMotorEncoder(Left);
	resetMotorEncoder(Intake);
	resetMotorEncoder(BallRelease);
	resetMotorEncoder(BallLift);
	resetMotorEncoder(IntakeLift);
	resetMotorEncoder(Right);
	displayTextLine(1,"		Calibrating Gyro");
	load();
	//GyroCustomCalibration(30);
	delay(10);
	displayTextLine(1,"		Starting Tasks");
	load();
	startTask(gyroTask);
	displayTextLine(0,"");
	displayTextLine(1,"");
	displayTextLine(2,"");
	displayTextLine(3,"			Done!");
	displayTextLine(4,"");
	startTask(displayControl);
	playSound(soundGasFillup);
	setTouchLEDBlinkTime(LED, 2, 2);
	while(!boot) {
		if (getJoystickValue(ChA)>80) {
			delay(250);
			setMotorBrakeMode(BallRelease, motorHold);
			resetMotorEncoder(BallRelease);
			activate();
			} else {
			displayTextLine(0,"");
			displayTextLine(1," Check Ball Release");
			displayTextLine(2,"  Move ChA Joystick");
			displayTextLine(3,"	    to continue");
			displayTextLine(4,"");
		};
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																			 				Main Task																									//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

task main() { // main program code
	setTouchLEDColor(LED, colorRed);
	startTask(Battery);
	sleep(500);
	startTask(bootup); //
	waitUntil(boot);	 //
	//activate();      //
	playSound(soundTada);
	setTouchLEDBlinkTime(LED, 0, 0);
	stopTask(bootup);
	setTouchLEDColor(LED, colorGreen);
	while(boot)
	{
		// Drive
		if (abs(getJoystickValue(ChA))>25 || abs(getJoystickValue(ChD))>25) {
			setTouchLEDColor(LED, colorGreen);
			setMotorSpeed(Left, getJoystickValue(ChA)); //set the value of the motor to the value of the controller joystick
			setMotorSpeed(Right, getJoystickValue(ChD)); //set the value of the motor to the value of the controller joystick
			} else {
			setMotorSpeed(Left, 0);	// if nothing is happening on the controller set the motor speed to 0
			setMotorSpeed(Right, 0); // if nothing is happening on the controller set the motor speed to 0
		};
		// Intake
		if (!intakeStarted && getJoystickValue(BtnFUp)) {
			//waitUntil();
			intakeStarted = true;
		}
		else if (!getJoystickValue(BtnFUp) && intakeStarted) {
			setMotorSpeed(Intake, 100);
			} else if(getJoystickValue(BtnFUp) && intakeStarted){
			setMotorSpeed(Intake, -100);
			setTouchLEDColor(LED, colorGreen);

		};
		// Ball Release
		if(!getMotorMoving(BallRelease)) {
			if (getJoystickValue(BtnLUp) && !BallReleased) {
				setMotorTarget(BallRelease,180,67);
				BallReleased = true;
				setTouchLEDColor(LED, colorBlue);
				sleep(100);
				} else if (getJoystickValue(BtnLUp) && BallReleased) {
				setMotorTarget(BallRelease,0,67);
				BallReleased = false;
				setTouchLEDColor(LED, colorBlue);
				sleep(100);
			}
		}
		// Ball Lift
		if(!getMotorMoving(BallLift)) {
			if (getJoystickValue(BtnLDown) && !BallLiftToggle) {
				setMotorTarget(BallLift,-90,67);
				BallLiftToggle = true;
				setTouchLEDColor(LED, colorBlue);
				sleep(100);
				} else if (getJoystickValue(BtnLDown) && BallLiftToggle) {
				setMotorTarget(BallLift,0,67);
				BallLiftToggle = false;
				setTouchLEDColor(LED, colorBlue);
				sleep(100);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
	};                                                                                                      //
};																								                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																			 				End of Code																								//
//																   		Last Compile: 22/02/2020, 1515														   		  //
//																			  		By Joseph Greening																					//
//																			2917X	/ 3428B VIQC Squared Away																		//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
