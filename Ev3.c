/*
 \file		Ev3.c
 \author	${user}
 \date		${date}
 \brief		Simple Hello World! for the Ev3
*/

// Includes
#include <ev3.h>
#include <unistd.h>

/* Notes
 *
 * 	Decreasing gyroscope angle --> Robot is rotating to the right
 * 	Increasing gyroscope angle --> Robot is rotating to the left
 *
 * 	Forwards rotation is actually backwards
 *
 *	Each tank tread consists of 27 segments
 *	Each wheel has 10 notches
 *
 *	360 / 10 = 36
 *
 *	One segments = 36 degrees
 *
 *	36 * 27 = 972
 *
 *	If the drivewheels rotate 972 degrees, the tank treads complete a full rotation
 *
 *	Each tank tread is 32 cm long
 *
 */

// Macros. Basically, "shortcuts". When the program is compiled, it replaces all shortcuts with it's "true" value. Eg '#define ret return' : Now we can write ret instead of return

#define bool _Bool

#define True 1

#define False 0

// Actual code

// - - - - - - - - - - - - - - //
//   Simple utility functions  //
// - - - - - - - - - - - - - - //

int GetRotation(){
	// Returns the value current value of the gyroscope
	int currentRotation = readSensor(IN_4);
	return currentRotation;
}

int GetDistance(){
	// Returns the value current value of the gyroscope
	int currentDistance = readSensor(IN_1);
	return currentDistance;
}

int CalculateAngleDifference(int angleArgument1, int angleArgument2){
	// Calculates the actual difference between two angles
	int difference = abs(angleArgument1 - angleArgument2);
	if(difference > 180){ // Ensures that the smallest possible difference is the one returned
		difference = 360 - difference;
	}
	return difference;
}

bool IsSmaller(int FirstValue, int SecondValue){
	// Compares both arguments to each other. If the first argument is smaller than the second, the function returns true (1). Otherwise it returns false (0)
	if(FirstValue < SecondValue){
		return True;
	}
	else{
		return False;
	}
}

// - - - - - - - - - - - - - - //
// Simple navigation functions //
// - - - - - - - - - - - - - - //

void StartRotateLeft(int speed){
	// Starts rotating the motors in opposite direction, resulting in the robot turning left
	if(speed > 100){
		speed = 100;
	}

	OnFwdReg(OUT_A, speed);
	OnRevReg(OUT_D, speed);
}

void StartRotateRight(int speed){
	// Starts rotating the motors in opposite direction, resulting in the robot turning right
	if(speed > 100){
		speed = 100;
	}

	OnFwdReg(OUT_D, speed);
	OnRevReg(OUT_A, speed);
}

void StartDriveForward(int speed){
	// Starts driving forward
	if(speed > 100){
		speed = 100;
	}
	OnRevReg(OUT_AD, speed);
}

void StartDriveBackward(int speed){
	// Starts driving backward
	if(speed > 100){
		speed = 100;
	}
	OnFwdReg(OUT_AD, speed);
}

void Stop(){
	// Stops movement motors
	Off(OUT_AD);
}

// - - - - - - - - - - - - - - - - - //
// More complex navigation functions //
// - - - - - - - - - - - - - - - - - //

void RotateLeft(int RotationDegrees){
    // Rotates the robot. Accomplished by rotating both drive engines in opposite direction
	int startRotationAngle;
	int currentAngle;
	int AngleDifference;
	while(True){
		AngleDifference = CalculateAngleDifference(startRotationAngle, currentAngle);
		if(AngleDifference == RotationDegrees){ // We have achieved the desired rotation
			break;
		}
		else{
			if(IsSmaller(AngleDifference, RotationDegrees)){ // If we haven't rotated enough
				StartRotateLeft(25);
			}

			else{ // We have rotated to much, we need to rotate back
				StartRotateRight(25);
			}
		}
	}
}

void RotateRight(int RotationDegrees){
    // Rotates the robot. Accomplished by rotating both drive engines in opposite direction
	int startRotationAngle;
	int currentAngle;
	int AngleDifference;
	while(True){
		AngleDifference = CalculateAngleDifference(startRotationAngle, currentAngle);
		if(AngleDifference == RotationDegrees){ // We have achieved the desired rotation
			break;
		}
		else{

			if(IsSmaller(AngleDifference, RotationDegrees)){ // If we haven't rotated enough
				StartRotateRight(25);
			}

			else{ // We have rotated to much, we need to rotate back
				StartRotateLeft(25);
			}
		}
	}
}

void RotateTo(int TargetRotation){
	int StartRotation = GetRotation();
	if(CalculateAngleDifference(StartRotation, TargetRotation) < CalculateAngleDifference(StartRotation + 1, TargetRotation)){
		// Rotating to the left made the delta angle bigger --> Rotating to the right makes us reach the target angle fastest
		StartRotateRight(25);
		int angle;
		while(angle != TargetRotation);
			angle = GetRotation();
	}
	else{
		// Either leftwise rotation makes us reach the target angle faster, or they're both equal (pretty unlikely)
		StartRotateLeft(25);
		int angle = GetRotation();
		while(angle != TargetRotation){
			angle = GetRotation();
		}
	}
}

void EnsureDistanceToWallIsBetween(int Highest, int Smallest){
	int Distance = 0;
	while(1){
		Distance = GetDistance();
		if(Distance < Highest && Distance > Smallest){ // If the robot is in the accepted range
			Stop();
			break; // Then we're done
		}
		else if(Distance > Highest){ // Too far away
			StartDriveForward(50);
		}
		else if(Distance < Smallest){ // Too close
			StartDriveBackward(50);
		}

	}

}

void DriveForwardPreciseDistance(double Distance, int speed){ // argument is in cm

	double angle = (972 / 32) * Distance;

	RotateMotor(OUT_AD, speed, -angle);

}

void DropPayload(){

	// TODO : Drop the book

}

void ZeroToWall(){
	int BestRotation = GetRotation();
	int BestDistance = GetDistance();
	int StartingRotation = BestRotation;
	int Lap = 0;
	StartRotateRight(25);
	while(True){
		int rotationCont = GetRotation();
		int distanceCont = GetDistance();
		if(distanceCont > BestDistance && rotationCont != BestRotation){ // If the robot finds a new best distance and rotation
			BestDistance = distanceCont;
			BestRotation = rotationCont;
		}
		if(rotationCont == StartingRotation){
			Lap++;
		}
		if(rotationCont == StartingRotation && Lap > 1){
			Stop();
			break;
		}
	}
	RotateTo(BestRotation);
}

bool GoToDestination(int DestinationNumber){
	ZeroToWall();
    if(DestinationNumber == 1 || DestinationNumber == 3){
    	RotateRight(90);
    	DriveForwardPreciseDistance(250);
    	if(DestinationNumber == 1){
        	RotateLeft(90);
    	}
    	else if(DestinationNumber == 3){
        	RotateRight(90);
    	}
    	EnsureDistanceToWallIsBetween(10, 20);
    	DropPayload();
    }
    else if(DestinationNumber == 2 || DestinationNumber == 4){
    	RotateLeft(90);
    	DriveForwardPreciseDistance(250);
    	if(DestinationNumber == 2){
        	RotateRight(90);
    	}
    	else if(DestinationNumber == 4){
        	RotateLeft(90);
    	}
    	EnsureDistanceToWallIsBetween(10, 20);
    	DropPayload();
    }
    else{ // Provided destination number is not registered
    	return 0; // Should be impossible, but just in case...
    }
    return 1;
}

int main(void)
{

	InitEV3();
	setAllSensorMode(US_DIST_MM, NO_SEN, NO_SEN, GYRO_ANG);

	int selectednumber = 0;

	while(True){ // A loop that waits for one of the 4 buttons to be pressed, which designates which destination the robot should move to
		if(ButtonIsDown(BUTTONUP) || ButtonIsDown(BUTTONRIGHT) || ButtonIsDown(BUTTONDOWN) || ButtonIsDown(BUTTONLEFT)){
			if(ButtonIsDown(BUTTONUP)){
				selectednumber = 1;
			}
			else if(ButtonIsDown(BUTTONRIGHT)){
				selectednumber = 2;
			}
			else if(ButtonIsDown(BUTTONDOWN)){
				selectednumber = 3;
			}
			else if(ButtonIsDown(BUTTONLEFT)){
				selectednumber = 4;
			}
			else{
				return 0; // Should be impossible, but just in case...
			}
			break;
		}
	}

	// Notifies you that a destination has been selected and that the robot will start moving in 4 seconds
	PlaySound(SOUND_DOUBLE_BEEP);
	sleep(1);
	PlaySound(SOUND_CLICK);
	sleep(1);
	PlaySound(SOUND_CLICK);
	sleep(1);
	PlaySound(SOUND_CLICK);
	sleep(1);
	PlaySound(SOUND_DOUBLE_BEEP);

	// Starts moving
	GoToDestination(selectednumber);



	FreeEV3();
	return 0;
}
