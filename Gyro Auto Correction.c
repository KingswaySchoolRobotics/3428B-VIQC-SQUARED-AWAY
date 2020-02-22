//
Insert Setup code here
//
long gyroValue;
long gyroError;

void gyroturn(int degree)

{

if(gyroValue>degree)
{
setMotor(leftDrive,11);
setMotor(rightDrive,-11);
waitUntil(gyroValue<=degree+7);

}
if(gyroValue<degree)
{
setMotor(leftDrive,-11);
setMotor(rightDrive,11);
waitUntil(gyroValue>=degree-7);

}
stopMotor(leftDrive);
stopMotor(rightDrive);
}

task gyroTask()
{
 long rate;
 long angle, lastAngle;
 lastAngle = 0;
 gyroError=0;
 // Change sensitivity, this allows the rate reading to be higher
 setGyroSensitivity(Gyro, gyroNormalSensitivity);
 //Reset the gyro sensor to remove any previous data.
 resetGyro(Gyro);
 wait1Msec(1000);
 repeat (forever) {
  rate = getGyroRate(Gyro);
  angle = getGyroDegrees(Gyro);
  // If big rate then ignore gyro changes
  if( abs( rate ) < 2 )
  {
   if( angle != lastAngle )
    gyroError += lastAngle - angle;
  }
  lastAngle = angle;
  gyroValue = angle + gyroError;
  wait1Msec(10);
 }
}


task main()
{
  
 startTask( gyroTask );    
    while(true) {
    if (//Something Happens) {
	gyroturn(90);
     }
  }
}
