/*
 \file		Ev3.c
 \author	${user}
 \date		${date}
 \brief		Simple Hello World! for the Ev3
*/

#include <ev3.h>

// Macros. Basically, "shortcuts". When the program is compiled, it replaces all shortcuts with it's "true" value. Eg '#define ret return' : Now we can write ret instead of return

#define bool _Bool

#define True 1

#define False 0

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

void RotateLeft(int RotationDegrees){ // TODO : Create an opposite function, that instead rotates right. The seemingly simple Rotate() function is split in to two different to increase readability.
    // Rotates the robot. Accomplished by rotating both drive engines in opposite direction
	int startRotationAngle;

	int currentAngle;

	int AngleDifference;

	while(True){

		AngleDifference = CalculateAngelDifference(startRotationAngle, currentAngle);

		if(CalculateAngelDifference(startRotationAngle, currentAngle) == RotationDegrees){

			break;

		}

		else{

			if(IsSmaller(AngleDifference, RotationDegrees)){ // If we haven't rotated enough

				// TODO : Rotate left

			}

			else{ // We have rotated to much

				// TODO : Rotate right

			}

		}

	}

}

void DriveStraight(int BiggestAllowedAngleDeviation){
    // Robot drives straight forward. If it deviates too much from it givens path, it stops and adjusts



}

void ZeroToWall(){
    // Makes the robot face the closest wall perpendicularly

	// First, the robot rotates in 90 degree increments, then 45 degree increments, and so on....
	int ZeroingDegrees[5] = {90, 45, 30, 15, 5};

	int ArrayLength = 5;

	// Two arrays to store the distance and the angle of the two "best" measuresments // array = {distance, angle}
	int ArrayClosest1[2];

	int ArrayClosest2[2];

	// The two following values are the one that are changed every position and compared to the previous values stored in the two previous arrays
	int LoopDistanceValue = 0;

	int LoopRotationValue = 0;

	// This value is used to keep track of which value the robot started rotating from
	int InnerLoopStartAngle = 0;

	for(int i = 0; i < ArrayLength; i++){

		for(int x = 0; ;x++){

			LoopDistanceValue = GetDistance();

			LoopRotationValue = GetRotation();

			if(i == 0 && (x == 0 || x == 1)){ // Ensures that the ArrayClosestValues aren't 0 to begin with, since that's technically the "best" value

				if(x == 0){

					ArrayClosest1[0] = LoopDistanceValue;

					ArrayClosest1[1] = LoopRotationValue;

				}

				else if(x == 1){

					ArrayClosest2[0] = LoopDistanceValue;

					ArrayClosest2[1] = LoopRotationValue;

				}

			}

			else{ // Most cases fall under this else-statement, since the previous if-statements are only meant to capture the very first attempts




			}

		}

		LoopRotationValue = GetRotation();

		LoopDistanceValue = GetDistance();

		if(){

		}

	};

}

bool GoToDestination(int DestinationNumber, bool Return){

	ZeroToWall();

    if(DestinationNumber == 1){

    }
    else if(DestinationNumber == 2){

    }
    else if(DestinationNumber == 3){

    }
    else if(DestinationNumber == 4){

    }
    else{
    	// Provided destination number is not registered
    }


    return 0;
}

int main(void)
{
	//TODO Place here your variables

	InitEV3();

	setAllSensorsMode(US_DIST_MM, NO_SEN, NO_SEN, GYRO_ANG);

	//TODO Place here your code



	FreeEV3();
	return 0;
}
