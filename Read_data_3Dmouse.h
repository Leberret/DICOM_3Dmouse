#pragma once

//Includes
#include <atlstr.h>
#include "si.h"      
#include "siapp.h"
#include <iostream>
#include <QtWidgets>

using namespace std;

//LRESULT  WINAPI HandleNTEvent(HWND hWnd, unsigned msg, WPARAM wParam, LPARAM lParam);

int DispatchLoopNT();
int SbInit();
void SbMotionEvent(SiSpwEvent* pEvent);
void BoutonsEvent(SiSpwEvent* pEvent);
