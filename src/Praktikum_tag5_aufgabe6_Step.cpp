//============================================================================
// Name        : Praktikum_tag5_aufgabe6_Step.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "RobotControl.h"
using namespace std;

RobotControl *RobotControl::transferPointer;

int main() {
	RobotControl robot1;
	robot1.Communicate();
	if(robot1.isActive() == false) cout << "Programm wurde nicht benutzt."<<endl;
	return 0;
}
