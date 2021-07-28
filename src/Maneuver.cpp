/*
 * Maneuver.cpp
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */
#include <iostream>
#include <cmath>
#include <list>
#include <string>
#include <fstream>
#include "Maneuver.h"
#include "ncurses.h"

Maneuver::Maneuver() {
	dMaxSpeed = 0.5;
	bIsRunning = false;
	dPosDifference = 0.02;
	adWishSpeed[0] = 0.0;
	adWishSpeed[1] = 0.0;
}

void Maneuver::CalcCircle(double dRadius, double dSpeed, double dTimestep) {
	CoordList.clear();
	double x_, y_;
	for (int counter = 1;
			counter < (int) ((2 * M_PI) / ((dSpeed / dRadius) * dTimestep));
			++counter) {
		x_ = dRadius * sin(counter * dSpeed * dTimestep / dRadius);
		y_ = dRadius * (1 - cos(counter * dSpeed * dTimestep / dRadius));
		CoordList.push_back(Coord(x_, y_, dSpeed));
	}
	//LogList("LogFileCircle.txt");
}

void Maneuver::CalcEight(double dRadius, double dSpeed, double dTimestep) {
	CoordList.clear();
	double x_, y_;

	//1.teilkreis
	for (int counter = 1;
			counter < (int) ((2 * M_PI) / ((dSpeed / dRadius) * dTimestep));
			++counter) {
		x_ = dRadius * sin(counter * (dSpeed/ dRadius) * dTimestep);
		y_ = dRadius * (1 - cos(counter * (dSpeed / dRadius) * dTimestep));
		CoordList.push_back(Coord(x_, y_, dSpeed));
	}
	//2.teilkreis
	for (int counter = 1;
			counter < (int) (2 * M_PI) / ((dSpeed / dRadius) * dTimestep);
			++counter) {
		x_ = dRadius * sin(counter * dSpeed * dTimestep / dRadius);
		y_ = (-1) * dRadius * (1 - cos(counter * dSpeed * dTimestep / dRadius));
		CoordList.push_back(Coord(x_, y_, dSpeed));
	}
	//LogList("LogFileEight.txt");
}

/*void Maneuver::LogList(string sDatei){
 ofstream file;
 if (sDatei == "LogFileCircle.txt") file.open("/home/pi/Schreibtisch/workspace_vorlage/Praktikum_tag5_aufgabe6_Step/src/LogFileCircle.txt",ios::trunc);
 else if (sDatei == "LogFileEight.txt") file.open("/home/pi/Schreibtisch/workspace_vorlage/Praktikum_tag5_aufgabe6_Step/src/LogFileEight.txt",ios::trunc);
 if(file.is_open() == false) {
 cout<<"cannot open the file"<< endl;
 }
 else {
 for(iter = CoordList.begin(); iter != CoordList.end(); ++iter)
 file<<iter->dX<<"\t"<<iter->dY<<"\n";
 file.close();
 }
 }
 */
bool Maneuver::isRunning() {
	return bIsRunning;
}

void Maneuver::Start() {
	bIsRunning = true;
	iter = CoordList.begin();
}

void Maneuver::Stop() {
	bIsRunning = false;
	adWishSpeed[0] = 0.0;
	adWishSpeed[1] = 0.0;
}

void Maneuver::Proceed() {
	bIsRunning = true;
}

void Maneuver::CalcManeuverSpeed(double dX, double dY, double dW) {
	double winkel_delta;
	double speed_ro;
	double speed_trans;
	//1.Positionsvergleich
	if (fabs(iter->dX - dX) <= dPosDifference
			&& fabs(iter->dY - dY) <= dPosDifference)
		iter++;

	//2.überprüfen ob bis zum end
	if (iter == CoordList.end())
		Stop();
	else {
		//3.winkel zwischen soll und ist berechnen und 4.winkel differenz berechnen
		winkel_delta = atan2(iter->dY - dY, iter->dX - dX) - dW;

		//5.winkel differenz beschränken
		if (winkel_delta <= -M_PI)
			winkel_delta += 2 * M_PI;
		else if (winkel_delta > M_PI)
			winkel_delta -= 2 * M_PI;

		//6. rotationsanteil bestimmen
		speed_ro = 2 * winkel_delta;
		if (speed_ro > dMaxSpeed)
			speed_ro = dMaxSpeed;
		else if (speed_ro < -dMaxSpeed)
			speed_ro = -dMaxSpeed;

		//7. translationsanteil übernehmen
		speed_trans = iter->dV;

		//8. geschwindigkeitsanteile überprüfen
		if (speed_ro * speed_trans > 0) {
			if (speed_ro + speed_trans > dMaxSpeed)
				speed_trans = dMaxSpeed - speed_ro;
			else if (speed_ro + speed_trans < -dMaxSpeed)
				speed_trans = -dMaxSpeed - speed_ro;
		} else if (speed_ro * speed_trans < 0) {
			if (speed_trans - speed_ro > dMaxSpeed)
				speed_trans = dMaxSpeed + speed_ro;
			else if (speed_trans - speed_ro < -dMaxSpeed)
				speed_trans = -dMaxSpeed + speed_ro;
		}

		//9. geschwindigkeiten summieren
		adWishSpeed[0] = speed_trans + speed_ro;
		adWishSpeed[1] = speed_trans - speed_ro;
	}

}
