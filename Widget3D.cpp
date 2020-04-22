#include "DICOM_3Dmouse.h"
#include "Scene3D.h"
#include "Widget3D.h"

/*--------------------------------------------------------------------------
* Fonctions : Widget3D()
*
* Description : Construction de la fenêtre 3D
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
Widget3D::Widget3D()
{
    //Initialisation des éléments
    Visualisation3D = new My3DScene();
    Layout = new QGridLayout();
    menu = new QMenuBar();//Initialisation Barre de menus
    Affichage = new QMenu("&Affichage");//Init menu affichage

    //Actions liées aux différents menus
    Affichage->addAction("Recentrer", this, SLOT(centrage()));//Action pour couleur

    //Conversion de la Scene3d en QWidget et fixation de sa taille
    container = createWindowContainer(Visualisation3D);
    container->setFixedSize(999, 725);

    //Ajout des composants à leurs entités
    menu->addMenu(Affichage);
    Layout->setMenuBar(menu);
    Layout->addWidget(container, 1, 0,Qt::AlignBottom);

    //paramètres du widget
    setLayout(Layout);
    setFixedSize(1000, 800);

    //Exécution
    show();
}
/*--------------------------------------------------------------------------
* Fonctions : closeEvent()
*
* Description : Appel de la fonction Supprimer qui permet de supprimer le
* dossier image et son contenu lors de la fermeture de la fenêtre principale
*
* Arguments : event : évenement de fermeture de la fenêtre
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::closeEvent(QCloseEvent* event)
{
    delete Visualisation3D;
    event->accept();

}
/*--------------------------------------------------------------------------
* Fonctions : centrage()
*
* Description : Appel de la fonction de recentrage de l'objet 3D dans la 
* classse My3DScene
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::centrage() {
    Visualisation3D->Recentrer();
}