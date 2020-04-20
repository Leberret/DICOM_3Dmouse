#include "Read_data_3Dmouse.h"

//D�finition des variables globales
HDC          hdc;
SiHdl        devHdl;
TCHAR        devicename[100] = _T("");
HWND         hWndMain;
HWND         hWnd3D;
INT          pTx, pTy, pTz, pRx, pRy, pRz;
INT          Intensite=0;
INT          prevInt;
INT          clicD=0, clicG=0;

/*--------------------------------------------------------------------------
* Fonction : SbInit()
*
* Description : Initialisation de la souris 3D
*
* Arguments : aucun
*
* Valeur retourn�e : res : r�sultat de SiOpen, =0 si erreur =1 si �a marche
*--------------------------------------------------------------------------*/
int SbInit()
{
    int res; //r�sultat de SiOpen, � retourner
    SiOpenDataEx oData; //Donn�es ind�pendantes du syst�me d'exploitation

    //Initialisation de la biblioth�que d'entr�es SpaceWare
    if (SiInitialize() == SPW_DLL_LOAD_ERROR)
    {
        //Message d'erreur
        QMessageBox error;
        error.setText("Erreur : Impossible de charger la librairie SiAppDll"); //Ajout � la boite QMessageBox
        error.exec(); //Affichage boite de dialogue
    }

    //Initialisation des donn�es sp�cifiques � la platfomre Windows
    SiOpenWinInitEx(&oData, hWndMain); 

    //Dit au pilote que nous voulons recevoir des V3DCMD au lieu de V3DKeys
    SiOpenWinAddHintBoolEnum(&oData, SI_HINT_USESV3DCMDS, SPW_TRUE);

    //Dit au driver que nous avons besoin d'une version minimale du driver de 17.5.5.
    //Cela pourrait �tre utilis� pour indiquer au driver qu'il doit �tre mis � niveau avant de pouvoir ex�cuter correctement cette application.
    SiOpenWinAddHintStringEnum(&oData, SI_HINT_DRIVERVERSION, L"17.5.5");

    //Ouverture des donn�es
    if ((devHdl = SiOpenEx(L"Logiciel de navigation 3D dans les images IRM", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData)) == NULL)
    {
        SiTerminate();  //Appel� pour fermer la biblioth�que d'entr�e SpaceWare
        
        //Impossible d'ouvrir l'appareil
        res = 0;
        return res;

        //Message d'erreur
        QMessageBox error;
        error.setText("Erreur : Probl�me avec l'initialisation de la souris 3D"); //Ajout � la boite QMessageBox
        error.exec(); //Affichage boite de dialogue
    }
    else
    {
        SiDeviceName devName;
        SiGetDeviceName(devHdl, &devName);
        
        //Appareil ouvert avec succ�s
        res = 1;
        return res;
    }

}

/*--------------------------------------------------------------------------
* Fonction : DispatchLoopNT()
*
* Description : Contient la boucle de message principale qui v�rifie 
* constamment les �v�nements de souris 3D et les g�re de mani�re appropri�e
*
* Arguments : aucun
*
* Valeur retourn�e :  msg.wparam : �v�nement pass� � la fen�tre
*--------------------------------------------------------------------------*/
int DispatchLoopNT()
{
    MSG            msg; //Message entrant � �valuer
    BOOL           handled; //Le message est-il encore g�r� ?
    SiSpwEvent     Event; //�v�nement SpaceWare
    SiGetEventData EData; //Donn�es d'�v�nement SpaceWare
    
    //Initialisation du handled
    handled = SPW_FALSE; 

    //Boucle de message
    while (GetMessage(&msg, NULL, 0, 0))
    {
        handled = SPW_FALSE;

        //Init des donn�es sp�cifiques � la plateforme Windows pour un appel � SiGetEvent
        SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);

        //V�rification si le msg �tait un �v�nement de souris 3D et le traiter
        if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT)
        {
            if (Event.type == SI_MOTION_EVENT)
            {
                //Processus �v�nement de mouvements de la souris 3D
                SbMotionEvent(&Event);
            }
            else if (Event.type == SI_CMD_EVENT)
            {
                //Processus �v�nement des boutons de la souris 3D
                BoutonsEvent(&Event); //V3DCMD_* events
            }

            //�v�nement de souris 3D g�r�
            handled = SPW_TRUE;
        }

        //Si c'est pas un �v�nement de souris 3D, laissez Windows le g�rer
        if (handled == SPW_FALSE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return((int)msg.wParam);
}

/*--------------------------------------------------------------------------
* Fonction : HandleNTEvent()
*
* Description : Gestion des divers �v�nements de la fen�tre
*
* Arguments : hWnd : handle de la fen�tre   
*             msg : message � traiter
*             wParam : Param�tre msg 32 bits
*             lParam : Param�tre msg 32 bits
*
* Valeur retourn�e :  msg.wparam : programme termin�
*--------------------------------------------------------------------------*/
/*LRESULT WINAPI HandleNTEvent(HWND hWnd, unsigned msg, WPARAM wParam,
    LPARAM lParam)
{
    PAINTSTRUCT ps; //Utilis� pour peindre la zone client d'une fen�tre
    LONG addr; //Adresse de la fen�tre

    //Obtenir l'adresse de la fen�tre
    addr = GetClassLong(hWnd, 0);

    switch (msg)
    {
    case WM_ACTIVATEAPP:
        hdc = GetDC(hWnd);

        //Lib�ration du handle de la fen�tre
        ReleaseDC(hWnd, hdc);

    case WM_KEYDOWN:

    case WM_KEYUP:
        //L'utilisateur a appuy� sur une touche pour fermer le programme
        if (wParam == VK_ESCAPE)
        {
            SendMessage(hWndMain, WM_CLOSE, 0, 0l);
        }
        break;

    case WM_PAINT:
        //Temps de peindre la fen�tre
        if (addr)
        {
            hdc = BeginPaint(hWndMain, &ps);
            EndPaint(hWndMain, &ps);
        }

        break;

    case WM_CLOSE:
        //Nettoyer les informations d'objet cr��es

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return (0);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);

}*/

/*--------------------------------------------------------------------------
* Fonction : SbMotionEvent()
*
* Description : R�cup�ration des informations de mouvement et stokage dans 
* des variables globales
*
* Arguments : *pEvent : Contient des donn�es d'un �v�nement de souris 3D
*
* Valeur retourn�e :  aucune
*--------------------------------------------------------------------------*/
void SbMotionEvent(SiSpwEvent* pEvent)
{
    pTx = pEvent->u.spwData.mData[SI_TX];
    pTy = pEvent->u.spwData.mData[SI_TY];
    pTz = pEvent->u.spwData.mData[SI_TZ];
    pRx = pEvent->u.spwData.mData[SI_RX];
    pRy = pEvent->u.spwData.mData[SI_RY];
    pRz = pEvent->u.spwData.mData[SI_RZ];
}

/*--------------------------------------------------------------------------
* Fonction : BoutonsEvent()
*
* Description : R�cup�ration des �v�nement li�s aux boutons de la souris 3D
*
* Arguments : *pEvent : Contient des donn�es d'un �v�nement de souris 3D
*
* Valeur retourn�e :  aucune
*--------------------------------------------------------------------------*/
void BoutonsEvent(SiSpwEvent* pEvent)
{
    
    hdc = GetDC(hWndMain);
    switch (pEvent->u.cmdEventData.functionNumber)
    {
    //Bouton de gauche
    case V3DCMD_KEY_F1:
        clicG= pEvent->u.cmdEventData.pressed;
        break;

    //Bouton de droite
    case V3DCMD_KEY_F2:
        clicD = pEvent->u.cmdEventData.pressed;
        if (pEvent->u.cmdEventData.pressed == 1) {
            int i = prevInt;
            if ((i == 0)&&(clicG==0)) {
                Intensite = 1;
            }
            else {
                Intensite = 0;
            }
            prevInt = Intensite;
        }
        break;
    default:
        break;
    }
}
