#pragma once

//Includes
#include <atlstr.h>
#include "si.h"      
#include "siapp.h"
#include <iostream>
#include <QtWidgets>

using namespace std;

//Appel des variables globales externes
extern INT			mode3D;


int DispatchLoopNT();
int SbInit();
void SbMotionEvent(SiSpwEvent* pEvent);
void BoutonsEvent(SiSpwEvent* pEvent);
void Init3DMouse();
