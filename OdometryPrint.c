int ticksPerRev = 960;
float x;
float y;
float LeftDriveTraveled;
float RightDriveTraveled;
float diameter = 63.661977236758134307553505349006;
float DriveWidth = 19.5; //cm  B (base line distance)

task odometry () { // odometry task

	setMotorEncoderUnits(encoderCounts);
	while(true)
	{
		OdometryAngle = (LeftDriveTraveled - RightDriveTraveled)  / DriveWidth;
		LeftDriveTraveled = (((3.14159 * diameter) / ticksPerRev) * (getMotorEncoder(Left))); // in mm
		RightDriveTraveled = (((3.14159 * diameter) / ticksPerRev) * (getMotorEncoder(Right))); // in mm
		x = ((LeftDriveTraveled + RightDriveTraveled) / 2) * cos(gyroValue);
		y = ((LeftDriveTraveled + RightDriveTraveled) / 2) * sin(gyroValue);
		delay(100);
	};
};
