#pragma once
#include <atlstr.h>
#include "si.h"      
#include "siapp.h"
#include <iostream>
using namespace std;

HDC          hdc;
SiHdl        devHdl;
TCHAR devicename[100] = _T("");
HWND         hWndMain;

LRESULT  WINAPI HandleNTEvent(HWND hWnd, unsigned msg, WPARAM wParam, LPARAM lParam);
int   DispatchLoopNT();
int   SbInit();
void  SbMotionEvent(SiSpwEvent* pEvent);

