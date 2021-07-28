/*
 * PosEstimation.h
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */

#ifndef POSESTIMATION_H_
#define POSESTIMATION_H_

class PosEstimation{
public:
	PosEstimation(){x[3] = {0.0}; dVelAvearage = 0.0;}
	void Reset();
	void PredicPosition(double dSpeedR, double dSpeedL, double dTimestep);
	double* GetPosition();
	~PosEstimation(){}

private:
	double x[3];//x,y,w
	double dVelAvearage;
};




#endif /* POSESTIMATION_H_ */
