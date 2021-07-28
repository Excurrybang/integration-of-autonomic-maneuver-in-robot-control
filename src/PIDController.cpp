/*
 * PIDController.cpp
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */
#include "PIDController.h"

PIDController::PIDController(double Kp_, double Ki_, double Kd_, double Ta_){
	dKp = Kp_;
	dKi = Ki_;
	dKd = Kd_;
	Ta = Ta_;
	dEsum = 0;
	deold = 0;
	dU = 0;
}

PIDController::PIDController(){
	dKp = 500.0;
	dKi = 100.0;
	dKd = 0.0;
	Ta = 0.04;
	dEsum = 0;
	deold = 0;
	dU = 0;
}

void PIDController::CalculateU(double dW_, double dY_){
	dEsum += (dW_ - dY_);
	dU = dKp*(dW_ - dY_) + dKi*Ta*dEsum + dKd*((dW_ - dY_) - deold)/Ta;
	deold = dW_ - dY_;
}

double PIDController::GetU(){
	return dU;
}
