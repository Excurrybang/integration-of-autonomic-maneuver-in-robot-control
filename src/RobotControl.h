/*
 * RobotControl.h
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */

#ifndef ROBOTCONTROL_H_
#define ROBOTCONTROL_H_

#include "InterfaceSIM.h"
#include "Maneuver.h"
#include "PIDController.h"

class RobotControl{
public:
	RobotControl();
	void Communicate();
	void Step();
	bool isActive();
	static void transferFunction(){transferPointer->Step();}

	static RobotControl* transferPointer;

private:
	InterfaceSIM interface;
	Maneuver maneuver;
	PosEstimation posEstimation;
	PIDController motorR;
	PIDController motorL;
	bool bIsActive;
};



#endif /* ROBOTCONTROL_H_ */
