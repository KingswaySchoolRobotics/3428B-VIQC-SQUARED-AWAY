#pragma config(Sensor, port3,  Main_Gyro,      sensorVexIQ_Gyro)

bool Finished = false;
bool Started = false;
int StartDegrees = 0;
int FinishedDegrees;
int Drift;

task main() {
	while (true) {
		if (getJoystickValue(BtnRUp) && !Started) {
			Started = true;
			resetTimer(timer1);
		};
		while (Started) {
			if (getTimer(timer1, seconds)>=60) {
				FinishedDegrees = getGyroHeadingFloat(Main_Gyro);
				delay(10);
				Started = false;
				Finished = true;
			};
		};

		if (Finished) {
			Drift = StartDegrees - FinishedDegrees;
			Finished = false;
		};
		displayVariableValues(line1,Finished);
		displayVariableValues(line2,Started);
		displayVariableValues(line3,StartDegrees);
		displayVariableValues(line4,FinishedDegrees);
		displayVariableValues(line5,Drift);
	};
};
