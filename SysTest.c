#pragma config(Sensor, port2,  ArmBottomBumper, sensorVexIQ_Touch)
#pragma config(Sensor, port3,  Main_Gyro,      sensorVexIQ_Gyro)
#pragma config(Sensor, port4,  LED,            sensorVexIQ_LED)
#pragma config(Sensor, port8,  BallColor,      sensorVexIQ_ColorHue)
#pragma config(Sensor, port12, AutonStart,     sensorVexIQ_Touch)
#pragma config(Motor,  motor1,          Left,          tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor5,          Intake,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor6,          Right,         tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor9,          CubeClaw,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor10,         ArmRight,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         ArmLeft,       tmotorVexIQ, PIDControl, reversed, encoder)

#define   DATALOG_SERIES_0    0 //Gyro
#define   DATALOG_SERIES_1    1 //Left
#define   DATALOG_SERIES_2    2 //Right
#define   DATALOG_SERIES_3    3 //AL
#define   DATALOG_SERIES_4    4 //AR
#define   DATALOG_SERIES_5    5 //IN
#define   DATALOG_SERIES_6    6 //Claw
#define   DATALOG_SERIES_7    7 // Claw Str
#define   DATALOG_SERIES_8    8 //

int   global_1 = 0;
int   global_2 = 0;
int   global_3 = 0;
int   global_4 = 0;
int   global_5 = 0;
int   global_6 = 0;
int   global_7 = 0;
int   global_8 = 0;

void DataCollection () /* ignore values on bottom of screen only graph values are valid /
Exp1 ArmPresetValue (fix negs)
Black = ArmPresetValue
Exp2 Motors
Drk-Green = Right
Purple = Left
Lime-Green = Intake
Maroon = CubeClaw

Exp3 Gyro Readings
blue = with drift
yellow = drift
red = Main_Gyro with out Drift
*/
{
	int loops = 0;
	datalogClear();
	while(true)
	{
		global_1 = getMotorCurrentLimit(Left);
		global_2 = getMotorSpeed(Left);
		global_3 = getMotorSpeed(Right);
		global_4 = getMotorCurrent(Left);
		global_5 = getMotorCurrent(Right);
		global_6 = getMotorSpeed(Intake);
		global_7 = getMotorSpeed(CubeClaw);
		global_8 = getMotorCurrent(CubeClaw);

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
		datalogAddValueWithTimeStamp( DATALOG_SERIES_3, global_4++ );
		wait1Msec(10);
		datalogAddValueWithTimeStamp( DATALOG_SERIES_3, global_4++ );

		// Repeat sequence every 360 loops
		if(loops++ == 360)
			loops = 0;

		// loop delay
		wait1Msec(10);
	}
}
task data(){
	while (true) {
		DataCollection();
}}

task main() {
	startTask(data);
	while (true) {
		setTouchLEDColor(LED, colorGreen);
		waitUntil(getJoystickValue(BtnEUp)&&getJoystickValue(BtnFUp)&&getJoystickValue(BtnLUp)&&getJoystickValue(BtnRUp));
		setMotorSpeed(Left,100);
		setMotorSpeed(Right,100);
		setMotorSpeed(ArmLeft,100);
		setMotorSpeed(ArmRight,100);
		setMotorSpeed(Intake,100);
		setMotorSpeed(CubeClaw,100);
		delay(2000);
		setMotorSpeed(Left,0);
		setMotorSpeed(Right,0);
		setMotorSpeed(ArmLeft,0);
		setMotorSpeed(ArmRight,0);
		setMotorSpeed(Intake,0);
		setMotorSpeed(CubeClaw,0);
		delay(2000);
		setMotorSpeed(Left,-100);
		setMotorSpeed(Right,-100);
		setMotorSpeed(ArmLeft,-100);
		setMotorSpeed(ArmRight,-100);
		setMotorSpeed(Intake,-100);
		setMotorSpeed(CubeClaw,-100);
		delay(2000);
		setMotorSpeed(Left,0);
		setMotorSpeed(Right,0);
		setMotorSpeed(ArmLeft,0);
		setMotorSpeed(ArmRight,0);
		setMotorSpeed(Intake,0);
		setMotorSpeed(CubeClaw,0);
		delay(2000);
		setMotorSpeed(Left,100);
		setMotorSpeed(Right,100);
		setMotorSpeed(ArmLeft,100);
		setMotorSpeed(ArmRight,100);
		setMotorSpeed(Intake,100);
		setMotorSpeed(CubeClaw,100);
		delay(2000);
		setMotorSpeed(Left,0);
		setMotorSpeed(Right,0);
		setMotorSpeed(ArmLeft,0);
		setMotorSpeed(ArmRight,0);
		setMotorSpeed(Intake,0);
		setMotorSpeed(CubeClaw,0);
		delay(2000);
		setTouchLEDColor(LED, colorGreen);
	}
}
