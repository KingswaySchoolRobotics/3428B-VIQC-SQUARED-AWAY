#pragma config(UserModel, "E:/VIQC Code/Lib/Header.h")

bool overTemp; // if any of the motors are overtemp set this value to true
bool currentLimitFlag; // if any of the motors are using current above the default value set this value to true
int PickupBonusSequenceState;
int PlaceBonusSequenceState;

float Height0 = 0; //Floor
float Height1 = -100; //Place HighScored Hub and Move MultiPusher Over Field Lines
float Height2 = -200; //Position HighScored Hub
float Height3 = -300; //Static Height / Moving height for balance
float Height4 = -500; //Bonus Hub Position
float Height5 = -680; //Bonus Hub PickUp
int ArmPresetValue = 0; // the preset number that tells the preset code how high to move the arm


void driveDistance(float distance) {
	float MoveDistanceRotations=distance/200*360;
	moveMotorTarget(Left, MoveDistanceRotations, 100);
	moveMotorTarget(Right, MoveDistanceRotations, 100);
};

bool TurnDegrees (float varTurnDegrees) {

	static bool InProgressTask;
	if (!InProgressTask) {
		resetGyro(Main_Gyro);
		if (varTurnDegrees>0) {
			setMotorSpeed(Left, 50);
			setMotorSpeed(Right, -50);
			} else {
			setMotorSpeed(Left, -50);
			setMotorSpeed(Right, 50);
		};
	};
	InProgressTask = true;
	if ((getGyroDegreesFloat(Main_Gyro)>varTurnDegrees && varTurnDegrees>0) || (getGyroDegreesFloat(Main_Gyro)<varTurnDegrees && varTurnDegrees<0)) {
		InProgressTask = false;
		setMotorSpeed(Left, 0);
		setMotorSpeed(Right, 0);
		return true;

	};
	return false;
};

void ArmHeightMove() {
	switch(ArmPresetValue) {  //moves the arm by reading the variable (ArmPresetValue)
	case 0://Floor
		setMotorTarget(ArmLeft, Height0, 100);
		setMotorTarget(ArmRight, Height0, 100);
		setTouchLEDColor(LED,colorOrange);
		setTouchLEDColor(LED,colorNone);
		break;

	case 1://Place HighScored Hub and Move MultiPusher Over Field Lines
		setMotorTarget(ArmLeft, Height1, 100);
		setMotorTarget(ArmRight, Height1, 100);
		setTouchLEDColor(LED,colorOrange);
		setTouchLEDColor(LED,colorNone);
		break;

	case 2://Position HighScored Hub
		setMotorTarget(ArmLeft, Height2, 100);
		setMotorTarget(ArmRight, Height2, 100);
		setTouchLEDColor(LED,colorOrange);
		setTouchLEDColor(LED,colorNone);
		break;

	case 3://Static Height / Moving height for balance
		setMotorTarget(ArmLeft, Height3, 100);
		setMotorTarget(ArmRight, Height3, 100);
		setTouchLEDColor(LED,colorOrange);
		setTouchLEDColor(LED,colorNone);
		break;

	case 4://Bonus Hub Align Position
		setMotorTarget(ArmLeft, Height4, 100);
		setMotorTarget(ArmRight, Height4, 100);
		setTouchLEDColor(LED,colorOrange);
		setTouchLEDColor(LED,colorNone);
		break;

	case 5://Bonus Hub Pickup Position
		setMotorTarget(ArmLeft, Height5, 100);
		setMotorTarget(ArmRight, Height5, 100);
		setTouchLEDColor(LED,colorOrange);
		setTouchLEDColor(LED,colorNone);
		break;

	};
};

void ArmTopLimit() {
	if (getBumperValue(ArmTopBumper)==1 && getJoystickValue(BtnRUp)){
		stopMultipleMotors(ArmLeft, ArmRight);
	};
};
void PickupBonusSequence () {
	static int LastState;
	bool P1;

	if (PickupBonusSequenceState !=LastState) {
		P1 = true;
		resetTimer(T1);
		LastState = PickupBonusSequenceState;
	}
	else {
		P1 = false;
	};
	switch (PickupBonusSequenceState){
	case 1:
		if (getJoystickValue(BtnEUp)==1) {
			PickupBonusSequenceState = 2;
		};
		break;

	case 2:
		if (P1) {
			driveDistance(600);
			ArmPresetValue=4;
			ArmHeightMove();
			delay(100);
		};
		if (getMotorZeroVelocity(Left)) {
			PickupBonusSequenceState = 3;
		};
		break;

	case 3:
		if (P1) {
			ArmPresetValue=5;
			ArmHeightMove();
			delay(100);
		};
		if (getMotorZeroVelocity(ArmLeft) || (getTimerValue(T1)>3000)) {
			PickupBonusSequenceState = 4;
		};
		break;

	case 4:
		if (P1) {
			driveDistance(-600);
			ArmPresetValue=3;
			delay(100);
		};
		if(getTimerValue(T1)>1500) {
			ArmHeightMove();
		};
		if (getMotorZeroVelocity(Left) || (getTimerValue(T1)>3000)) {
			PickupBonusSequenceState = 5;
		};
		break;

	case 5:
		if (TurnDegrees(90.0)) {
			PickupBonusSequenceState =1;
		};
		break;

	default: PickupBonusSequenceState = 1;
	};
};

void PlaceBonusSequence () {
	static int LastState;
	bool P1;

	if (PlaceBonusSequenceState !=LastState) {
		P1 = true;
		resetTimer(T1);
		LastState = PlaceBonusSequenceState;
	}
	else {
		P1 = false;
	};
	switch (PlaceBonusSequenceState){
	case 1:
		if (getJoystickValue(BtnFUp)==1) {
			PlaceBonusSequenceState = 2;
		};
		break;

	case 2:
		if (P1) {
			driveDistance(500);
			ArmPresetValue=3;
			ArmHeightMove();
			delay(100);
		};
		if (getMotorZeroVelocity(Left) || (getTimerValue(T1)>3000)) {
			PlaceBonusSequenceState = 3;
		};
		break;

	case 3:
		if (P1) {
			driveDistance(-70);
			delay(100);
		};
		if (getMotorZeroVelocity(Left) || (getTimerValue(T1)>1500)) {
			PlaceBonusSequenceState = 4;
		};
		break;

	case 4:
		if (P1) {
			ArmPresetValue=1;
			ArmHeightMove();
		};
		if (getMotorZeroVelocity(ArmLeft) || (getTimerValue(T1)>500)) {
			PlaceBonusSequenceState = 5;
		};
		break;

	case 5:
		if (P1) {
			delay(500);
			driveDistance(-380);
			delay(100);

		};
		if (getMotorZeroVelocity(Left) || (getTimerValue(T1)>1500)) {
			PlaceBonusSequenceState = 1;
		};
		if(getTimerValue(T1)>1500) {
			ArmPresetValue = 0;
			ArmHeightMove();
		};
		break;

	default: PlaceBonusSequenceState = 1;
	};
};

void EMERGENCYSTOP (float delay) { // if emergency stop buttons are pushed stop all motor and blink red on touch led
	if ((getJoystickValue(BtnEDown)==1)&&(getJoystickValue(BtnFDown)==1)| (overTemp==true)|(currentLimitFlag==true)) {
		sleep(delay);
		repeatUntil(getTouchLEDValue(LED)==1 || (getJoystickValue(BtnEDown)==1) || (getJoystickValue(BtnFDown)==1)) {
			setTouchLEDColor(LED,colorRed);
			setTouchLEDBlinkTime(LED, 12.5, 7.5);
			stopAllMotors();
			playSound(soundCarAlarm2);
		};
		setTouchLEDColor(LED, colorNone);
	};
};

void MotorDiagnostics() { // checks for motor overtemp or currentLimitFlag and plays an alarm if they are true
	if ((overTemp==true)|(currentLimitFlag==true))
		playSound(soundCarAlarm2);
};

void ArmReset() {
	if (getBumperValue(ArmBottomBumper)==1) {
		resetMotorEncoder(ArmLeft);
		resetMotorEncoder(ArmRight);
		setTouchLEDColor(LED,colorBlue);
		setTouchLEDColor(LED,colorNone);

	}
}

task main() { // main program code

	resetMotorEncoder(ArmLeft);	 //Resets Left Arm Motor Encoder to 0
	resetMotorEncoder(ArmRight); //Resets Right Arm Motor Encoder to 0
	bool armPressed;
	bool IndexArmPressed;
	while(true) //while the program is running do this:
	{
		ArmReset();
		//Display Code
		displayTextLine(0, "Arm=%d", ((getMotorEncoder(ArmLeft)) + (getMotorEncoder(ArmRight)))/2); //Displays Average of motor encoders position
		displayVariableValues(1,ArmPresetValue); // displays the preset value for the height of the arm
		displaySensorValues(2, Main_Gyro); // displays the gyro value in degrees
		displayVariableValues(3, PickupBonusSequenceState);
		//System Functions
		MotorDiagnostics(); // see MotorDiagnostics
		EMERGENCYSTOP(100); // see EMERGENCYSTOP
		//Sequences
		PickupBonusSequence();
		PlaceBonusSequence();
		//Preset Functions
		ArmTopLimit();

		//Arm Code
		if (getJoystickValue(BtnLDown)==1 && IndexArmPressed==false) {  //gets the reading of LUP & LDOWN and turns it into a variable(ArmPresetValue)
			ArmPresetValue = (ArmPresetValue-1);
			ArmHeightMove();
			IndexArmPressed=true;
		}
		else {
			if (getJoystickValue(BtnLUp)==1 && IndexArmPressed==false) {
				ArmPresetValue = (ArmPresetValue+1);
				ArmHeightMove();
				IndexArmPressed=true;
			}
		};
		if (getJoystickValue(BtnLDown)==0 && getJoystickValue(BtnLUp)==0) {
			IndexArmPressed=false;
		};
		if (0>ArmPresetValue){
			ArmPresetValue = 0;
		}
		else {
			if (4<ArmPresetValue) {
				ArmPresetValue = 5;
			};
		}

		//DriveCode
		if (PickupBonusSequenceState ==1 && PlaceBonusSequenceState == 1) {
			if(abs(getJoystickValue(ChA))>25 || abs(getJoystickValue(ChD))>25) {	//if the absoloute value of ChA is above 20 or the absoloute value of ChD is above 15 then allow the Motors to
				setMotorSpeed(Left, getJoystickValue(ChA)); //set the value of the motor to the value of the controller joystick
				setMotorSpeed(Right, getJoystickValue(ChD)); //set the value of the motor to the value of the controller joystick
			}
			else { setMotorSpeed(Left, 0);	// if nothing is happening on the controller set the motor speed to 0
				setMotorSpeed(Right, 0); // if nothing is happening on the controller set the motor speed to 0
			};

			if(getJoystickValue(BtnRUp)==1) //if the right up button is pushed move the arm up
			{
				setMotorSpeed(ArmLeft, -75);	//move the left arm motor up
				setMotorSpeed(ArmRight, -75); //move the right arm motor up
				armPressed = true;
			}
			else
			{
				if(getJoystickValue(BtnRDown)==1) //if the right down button is pushed move the arm down
				{
					setMotorSpeed(ArmLeft, 75);	 //move the left arm motor down
					setMotorSpeed(ArmRight, 75); //move the right arm motor down
					armPressed = true;
				}
			}
			if (getJoystickValue(BtnRDown)==0 && getJoystickValue(BtnRUp)==0 && armPressed==true) {
				setMotorSpeed(ArmLeft,0);
				setMotorSpeed(ArmRight,0);
				armPressed = false;
			};
		};
	};
}

//END
