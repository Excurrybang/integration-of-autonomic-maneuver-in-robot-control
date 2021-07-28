/*
 * RobotControl.cpp
 *
 *  Created on: 01.04.2021
 *      Author: pi
 */
#include "RobotControl.h"
#include "InterfaceSIM.h"
#include "Maneuver.h"
#include "ncurses.h"
#include <iostream>

using namespace std;

RobotControl::RobotControl(){
	bIsActive = true;
	interface.Initialize(0.04,(*transferFunction));
	transferPointer = this;
}


void RobotControl::Communicate(){

	int input_num;
	double input_radius;
	double input_gesch;
	int input_wunsch;

	std::cout <<"Robot control system start.."<<std::endl<<endl;

	cout <<"Ob Sie ein Kreis oder eine Acht Manöver erzeugen möchten, bitte Nummer eingebn:(1 für Krei, 2 für Acht, 0 für nichts)"<<endl;
	cin >> input_num;

	if (input_num == 0){
		cout <<"System würde beenden."<<endl;
		bIsActive = false;
	}
	else if (input_num == 1 || input_num == 2){
		cout <<"Bitte den Radius eingeben: "<<endl;
		cin >> input_radius;

		cout <<"Bitte die Geschwindigkeit eingeben:[-0.5, 0.5]"<<endl;
		cin >> input_gesch;

		if (input_gesch <= 0.5 && input_gesch >= -0.5){
			if(input_num == 1){
				maneuver.CalcCircle(input_radius, input_gesch, 0.04);
			}
			else if (input_num == 2){
				maneuver.CalcEight(input_radius, input_gesch, 0.04);
			}
			initscr();
			nodelay(stdscr, TRUE);
			noecho();
			sigprocmask(SIG_UNBLOCK, &interface.mask, nullptr);

			printw("bitte Ihr Wunsch dürcken, (1 für Start, 2 für Stop, 3 für weiter laufen, q für Ende\n");
			while (1) {
					input_wunsch = getch();
					if ( input_wunsch != ERR) {
						clear();
						printw("bitte Ihr Wunsch dürcken, (1 für Start, 2 für Stop, 3 für weiter laufen, q für Ende\n");

						if(input_wunsch == 'q' ){
							maneuver.Stop();
							printw("Eingabe ist q\n");
							printw("Manöver würde kurz enden\n");
							break;
						}

						switch (input_wunsch) {
						case (49):
								maneuver.Start();
								printw("Eingabe ist 1\n");
								printw("Manöver würde straten\n");
								break;
						case (50):
								maneuver.Stop();
								printw("Eingabe ist 2\n");
								printw("Manöver würde stoppen\n");
								break;
						case (51):
								maneuver.Proceed();
								printw("Eingabe ist 3\n");
								printw("Manöver würde weiterlaufen\n");
								break;
						default:
							printw("Bitte nur 1, 2, 3 oder q eindrücken\n");
							break;
						}
					}
			}
			while(interface.GetInput()[0] != 0 || interface.GetInput()[1] != 0) continue;
			sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
			endwin();

			cout<<"Roboter und Programm stoppt"<<endl;
		}
		else cout << "bitte Geschwindigkeit in Bereich [-0.5, 0.5] eingeben"<<endl;
	}
	else cout << "bitte nur 0, 1 oder 2 eingeben"<<endl;
}

void RobotControl::Step(){
	double* ist_gesch = interface.GetInput();
	int iMicros[2] = {0};
	if (maneuver.isRunning() == true){
		//compute the future position
		posEstimation.PredicPosition(ist_gesch[0], ist_gesch[1], 0.04);
		double* soll_pos = posEstimation.GetPosition();

		//compute the speed of right and left
		maneuver.CalcManeuverSpeed(soll_pos[0], soll_pos[1], soll_pos[2]);

		//PID Regel
		double* ma_speed = maneuver.GetManeuverSpeed();
		motorR.CalculateU(ma_speed[0], ist_gesch[0]);
		motorL.CalculateU(ma_speed[1], ist_gesch[1]);

		//speed translate into signal
		iMicros[0] = int(motorR.GetU() + 1500);
		iMicros[1] = int(motorL.GetU() + 1500);

		//iMicros[0] = int(ma_speed[0]*1000 + 1500);
		//iMicros[1] = int(ma_speed[1]*1000 + 1500);

		//restrict signal between 1000 and 2000
		if (iMicros[0] > 2000){
			iMicros[0] = 2000;
			printw("Rechte Geschwindigkeit ist größer als 0.5, würde automatisch auf 0.5 stellen\n");
		}
		else if (iMicros[0] < 1000){
			iMicros[0] = 1000;
			printw("Rechte Geschwindigkeit ist kleiner als -0.5, würde automatisch auf 0.5 stellen\n");
		}
		if (iMicros[1] > 2000){
			iMicros[1] = 2000;
			printw("Linke Geschwindigkeit ist größer als 0.5, würde automatisch auf 0.5 stellen\n");
		}
		else if (iMicros[1] < 1000){
			iMicros[1] = 1000;
			printw("Linke Geschwindigkeit ist kleiner als -0.5, würde automatisch auf 0.5 stellen\n");
		}


	}
	else {
		iMicros[0] = 1500;
		iMicros[1] = 1500;
	}

	interface.SetOutputs(iMicros);
}

bool RobotControl::isActive(){
	return bIsActive;
}

