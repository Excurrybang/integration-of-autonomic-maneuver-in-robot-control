/*
 * Maneuver.h
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */

#ifndef MANEUVER_H_
#define MANEUVER_H_

#include <iostream>
#include <cmath>
#include <list>
#include <string>
#include "PosEstimation.h"

using namespace std;

class Maneuver{
public:
	Maneuver();
	void CalcCircle(double dRadius, double dSpeed, double dTimestep);
	void CalcEight(double dRadius, double dSpeed, double dTimestep);
	void LogList(string sDatei);
	bool isRunning();
	void Start();
	void Stop();
	void Proceed();
	void CalcManeuverSpeed(double dX, double dY, double dW);
	double* GetManeuverSpeed(){return adWishSpeed;}
	//bool get_get_speed(){return get_speed;}

private:
	struct Coord{
		double dX;
		double dY;
		double dV;
		Coord (double m_dx, double m_dy, double m_dv):dX(m_dx), dY(m_dy),dV(m_dv){}
	};
	list<Coord> CoordList;
	list<Coord>::iterator iter;
	bool bIsRunning;
	double adWishSpeed[2];
	double dMaxSpeed;
	double dPosDifference;
	PosEstimation pos_akt;
	//bool get_speed;
};



#endif /* MANEUVER_H_ */
