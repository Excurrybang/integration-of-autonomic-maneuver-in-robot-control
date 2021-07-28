/*
 * PosEstimation.cpp
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */
#include "PosEstimation.h"
#include <cmath>


void PosEstimation::Reset(){
	x[0] = 0.0;
	x[1] = 0.0;
	x[2] = 0.0;
}

void PosEstimation::PredicPosition(double dSpeedR, double dSpeedL, double dTimestep){
	x[0] += dVelAvearage * dTimestep * cos(x[2]);
	x[1] += dVelAvearage * dTimestep * sin(x[2]);
	x[2] += dTimestep*(dSpeedR - dSpeedL)/0.23;
	x[2] = fmod(x[2], 2*M_PI);
	if (x[2] > M_PI) x[2] -= 2*M_PI;
	else if (x[2] < -M_PI) x[2] += 2*M_PI;
	dVelAvearage = (dSpeedR + dSpeedL)/2;
}

double* PosEstimation::GetPosition(){
	return x;
}
