#include "Read_data_3Dmouse.h"
HDC          hdc;
SiHdl        devHdl;
TCHAR devicename[100] = _T("");
HWND         hWndMain;

TCHAR buff0[30];                            /* text buffer for TX */
TCHAR buff1[30];                            /* text buffer for TY */
TCHAR buff2[30];                            /* text buffer for TZ */
TCHAR buff3[30];                            /* text buffer for RX */
TCHAR buff4[30];                            /* text buffer for RY */
TCHAR buff5[30];                            /* text buffer for RZ */


int SbInit()
{
    int res;                             /* result of SiOpen, to be returned  */
    SiOpenDataEx oData;                    /* OS Independent data to open ball  */

    /*init the SpaceWare input library */
    if (SiInitialize() == SPW_DLL_LOAD_ERROR)
    {
        cout << "Error: Could not load SiAppDll dll files" << endl;
    }

    SiOpenWinInitEx(&oData, hWndMain);    /* init Win. platform specific data  */

    // Tell the driver we want to receive V3DCMDs instead of V3DKeys
    SiOpenWinAddHintBoolEnum(&oData, SI_HINT_USESV3DCMDS, SPW_TRUE);

    // Tell the driver we need a min driver version of 17.5.5.  
    // This could be used to tell the driver that it needs to be upgraded before it can run this application correctly.
    SiOpenWinAddHintStringEnum(&oData, SI_HINT_DRIVERVERSION, L"17.5.5");

    /* open data, which will check for device type and return the device handle
    to be used by this function */
    if ((devHdl = SiOpenEx(L"3DxTest", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData)) == NULL)
    {
        SiTerminate();  /* called to shut down the SpaceWare input library */
        res = 0;        /* could not open device */
        return res;
        cout << "Sorry - pb SbInit.\n" << endl;
    }
    else
    {
        SiDeviceName devName;
        SiGetDeviceName(devHdl, &devName);
        _stprintf_s(devicename, SPW_NUM_ELEMENTS_IN(devicename), _T("%S"), devName.name);

        res = 1;        /* opened device succesfully */
        return res;
    }

}

int DispatchLoopNT()
{
    MSG            msg;      /* incoming message to be evaluated */
    BOOL           handled;  /* is message handled yet */
    SiSpwEvent     Event;    /* SpaceWare Event */
    SiGetEventData EData;    /* SpaceWare Event Data */

    handled = SPW_FALSE;     /* init handled */

    /* start message loop */
    while (GetMessage(&msg, NULL, 0, 0))
    {
        handled = SPW_FALSE;

        /* init Window platform specific data for a call to SiGetEvent */
        SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);

        /* check whether msg was a 3D mouse event and process it */
        if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT)
        {
            if (Event.type == SI_MOTION_EVENT)
            {
                SbMotionEvent(&Event);        /* process 3D mouse motion event */
            }


            handled = SPW_TRUE;              /* 3D mouse event handled */
        }

        /* not a 3D mouse event, let windows handle it */
        if (handled == SPW_FALSE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return((int)msg.wParam);
}


LRESULT WINAPI HandleNTEvent(HWND hWnd, unsigned msg, WPARAM wParam,
    LPARAM lParam)
{
    PAINTSTRUCT ps;           /* used to paint the client area of a window */
    LONG addr;                /* address of our window */

    addr = GetClassLong(hWnd, 0);  /* get address */

    switch (msg)
    {
    case WM_ACTIVATEAPP:
        hdc = GetDC(hWnd);


        /*release our window handle */
        ReleaseDC(hWnd, hdc);
    case WM_KEYDOWN:
    case WM_KEYUP:
        /* user hit a key to close program */
        if (wParam == VK_ESCAPE)
        {
            SendMessage(hWndMain, WM_CLOSE, 0, 0l);
        }
        break;

    case WM_PAINT:
        /* time to paint the window */
        if (addr)
        {
            hdc = BeginPaint(hWndMain, &ps);
            EndPaint(hWndMain, &ps);
        }

        break;

    case WM_CLOSE:
        /* cleanup the object info created */

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return (0);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);

}



void SbMotionEvent(SiSpwEvent* pEvent)
{
   
    int len0, len1, len2, len3, len4, len5;	   /* length of each buffer */

    /* put the actual ball data into the buffers */
    len0 = _stprintf_s(buff0, 30, _T("TX: %d         "), pEvent->u.spwData.mData[SI_TX]);
    len1 = _stprintf_s(buff1, 30, _T("TY: %d         "), pEvent->u.spwData.mData[SI_TY]);
    len2 = _stprintf_s(buff2, 30, _T("TZ: %d         "), pEvent->u.spwData.mData[SI_TZ]);
    len3 = _stprintf_s(buff3, 30, _T("RX: %d         "), pEvent->u.spwData.mData[SI_RX]);
    len4 = _stprintf_s(buff4, 30, _T("RY: %d         "), pEvent->u.spwData.mData[SI_RY]);
    len5 = _stprintf_s(buff5, 30, _T("RZ: %d         "), pEvent->u.spwData.mData[SI_RZ]);
    
    /* get handle of our window to draw on */
    hdc = GetDC(hWndMain);

    /* Dump to debugger output buffer to get a running log */
    _RPT3(_CRT_WARN, "%S %S %S", buff0, buff1, buff2);
    _RPT3(_CRT_WARN, " %S %S %S\n", buff3, buff4, buff5);

    
    /*release our window handle */
    ReleaseDC(hWndMain, hdc);

}