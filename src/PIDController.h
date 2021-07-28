/*
 * PIDController.h
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class PIDController{
public:
	PIDController(double Kp_, double Ki_, double Kd_, double Ta_);
	PIDController();
	void CalculateU(double dW_, double dY_);//dw_ soll_gesch, dy_ ist_gesch
	double GetU();
	~PIDController(){}

private:
	double dKp, dKi, dKd, Ta;
	double dEsum, deold;
	double dU;
};


#endif /* PIDCONTROLLER_H_ */
