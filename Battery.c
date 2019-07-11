
task main()
{
	while (true) {
		if (getJoystickValue(BtnLUp)) {
			playRepetitiveSound(soundSiren2, 100);
			} else if (getJoystickValue(BtnLDown)) {
			playRepetitiveSound(soundCarAlarm4, 100);
			} else if (getJoystickValue(BtnRUp)) {
			playRepetitiveSound(soundWrongWay, 100);
		};
	};
}
