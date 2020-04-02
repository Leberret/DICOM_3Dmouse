#pragma once
#include <atlstr.h>
#include "si.h"      
#include "siapp.h"
#include <iostream>
#include <QtWidgets>

using namespace std;



LRESULT  WINAPI HandleNTEvent(HWND hWnd, unsigned msg, WPARAM wParam, LPARAM lParam);
void   DispatchLoopNT();
int   SbInit();
void  SbMotionEvent(SiSpwEvent* pEvent);
void HandleV3DCMDEvent(SiSpwEvent* pEvent);
