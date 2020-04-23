#include "Read_data_3Dmouse.h"

//Définition des variables globales
//HDC          hdc;
SiHdl        devHdl;
//TCHAR        devicename[100] = _T("");
HWND         hWndMain;
INT          pTx, pTy, pTz, pRx, pRy, pRz;
INT          Intensite=0,prevInt;
INT          OnOffSouris3D=0,prevOnOffSouris3D;
INT          clicD = 0,clicG = 0;

/*--------------------------------------------------------------------------
* Fonction : Init3DMouse()
*
* Description : Appel SbInit et vérifie si l'initialisation de la souris 3D
* c'est bien passé
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Init3DMouse() {
    int res;

    // Initialisation de la souris 3D
    res = SbInit();
    //Condtiion de vérification d'erreur
    if (res < 1) //Si erreur
    {
        QMessageBox error;
        error.setText("Sorry - No supported 3Dconnexion device available."); //Ajout à la boite QMessageBox
        error.exec(); //Affichage boite de dialogue

        ExitProcess(1); //Fermeture du programme
    }
    else { //Si pas d'erreur
        //Appel de la boucle
        DispatchLoopNT();
    }
}

/*--------------------------------------------------------------------------
* Fonction : SbInit()
*
* Description : Initialisation de la souris 3D
*
* Arguments : aucun
*
* Valeur retournée : res : résultat de SiOpen, =0 si erreur =1 si ça marche
*--------------------------------------------------------------------------*/
int SbInit()
{
    int res; //résultat de SiOpen, à retourner
    SiOpenDataEx oData; //Données indépendantes du système d'exploitation

    //Initialisation de la bibliothèque d'entrées SpaceWare
    if (SiInitialize() == SPW_DLL_LOAD_ERROR)
    {
        //Message d'erreur
        QMessageBox error;
        error.setText("Erreur : Impossible de charger la librairie SiAppDll"); //Ajout à la boite QMessageBox
        error.exec(); //Affichage boite de dialogue
    }

    //Initialisation des données spécifiques à la platfomre Windows
    SiOpenWinInitEx(&oData, hWndMain); 

    //Dit au pilote que nous voulons recevoir des V3DCMD au lieu de V3DKeys
    SiOpenWinAddHintBoolEnum(&oData, SI_HINT_USESV3DCMDS, SPW_TRUE);

    //Dit au driver que nous avons besoin d'une version minimale du driver de 17.5.5.
    //Cela pourrait être utilisé pour indiquer au driver qu'il doit être mis à niveau avant de pouvoir exécuter correctement cette application.
    SiOpenWinAddHintStringEnum(&oData, SI_HINT_DRIVERVERSION, L"17.5.5");

    //Ouverture des données
    if ((devHdl = SiOpenEx(L"Interface de reconstruction 3D des IRM", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData)) == NULL)
    {
        SiTerminate();  //Appelé pour fermer la bibliothèque d'entrée SpaceWare
        
        //Impossible d'ouvrir l'appareil
        res = 0;
        return res;

        //Message d'erreur
        QMessageBox error;
        error.setText("Erreur : Problème avec l'initialisation de la souris 3D"); //Ajout à la boite QMessageBox
        error.exec(); //Affichage boite de dialogue
    }
    else
    {
        SiDeviceName devName;
        SiGetDeviceName(devHdl, &devName);
        
        //Appareil ouvert avec succès
        res = 1;
        return res;
    }
    
    

}

/*--------------------------------------------------------------------------
* Fonction : DispatchLoopNT()
*
* Description : Contient la boucle de message principale qui vérifie 
* constamment les événements de souris 3D et les gère de manière appropriée
*
* Arguments : aucun
*
* Valeur retournée :  msg.wparam : événement passé à la fenêtre
*--------------------------------------------------------------------------*/
int DispatchLoopNT()
{
    MSG            msg; //Message entrant à évaluer
    BOOL           handled; //Le message est-il encore géré ?
    SiSpwEvent     Event; //Événement SpaceWare
    SiGetEventData EData; //Données d'événement SpaceWare
    
    //Initialisation du handled
    handled = SPW_FALSE; 

    //Boucle de message
    while (GetMessage(&msg, NULL, 0, 0))
    {
        handled = SPW_FALSE;

        //Init des données spécifiques à la plateforme Windows pour un appel à SiGetEvent
        SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);

        //Vérification si le msg était un événement de souris 3D et le traiter
        if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT)
        {
            if (Event.type == SI_MOTION_EVENT)
            {
                //Processus événement de mouvements de la souris 3D
                SbMotionEvent(&Event);
            }
            else if (Event.type == SI_CMD_EVENT)
            {
                //Processus événement des boutons de la souris 3D
                BoutonsEvent(&Event); //V3DCMD_* events
            }

            //Événement de souris 3D géré
            handled = SPW_TRUE;
        }

        //Si c'est pas un événement de souris 3D, laissez Windows le gérer
        if (handled == SPW_FALSE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return((int)msg.wParam);
}

/*--------------------------------------------------------------------------
* Fonction : SbMotionEvent()
*
* Description : Récupération des informations de mouvement et stokage dans 
* des variables globales
*
* Arguments : *pEvent : Contient des données d'un événement de souris 3D
*
* Valeur retournée :  aucune
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
* Description : Récupération des événement liés aux boutons de la souris 3D
*
* Arguments : *pEvent : Contient des données d'un événement de souris 3D
*
* Valeur retournée :  aucune
*--------------------------------------------------------------------------*/
void BoutonsEvent(SiSpwEvent* pEvent)
{ 
    switch (pEvent->u.cmdEventData.functionNumber)
    {
    //Bouton de gauche
    case V3DCMD_KEY_F1:
        clicG = pEvent->u.cmdEventData.pressed;
        if ((pEvent->u.cmdEventData.pressed == 1) && (clicD == 0)) {
            int i = prevOnOffSouris3D;
            if (i == 0) {
                OnOffSouris3D = 1;
            }
            else {
                OnOffSouris3D = 0;
            }
            prevOnOffSouris3D = OnOffSouris3D;
        }

        break;

    //Bouton de droite
    case V3DCMD_KEY_F2:
        clicD = pEvent->u.cmdEventData.pressed;
        if ((pEvent->u.cmdEventData.pressed == 1) && (clicG==0)) {
            int i = prevInt;
            if (i == 0) {
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
