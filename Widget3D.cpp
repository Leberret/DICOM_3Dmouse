#include "DICOM_3Dmouse.h"
#include "Scene3D.h"
#include "Widget3D.h"
//#include "Read_data_3Dmouse.h"


/*--------------------------------------------------------------------------
* Fonctions : DoubleClics()
*
* Description : Appel SaveAs si les 2 boutons de la souris 3D sont
* pressé enssemble ou non
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::DoubleClics() {
    //Condition de double clics
    if ((clicD == 1) && (clicG == 1)) {
        SaveAs3D();
        clicD = 0;
        clicG = 0;
    }
    else
        return;
}

/*--------------------------------------------------------------------------
* Fonctions : SaveAs()
*
* Description : Permet de screenshoter l'interafce de reconstruction 3D
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::SaveAs3D() {
    qApp->beep(); // Signal the screenshoot

        //Initialisation de la hauteur des images
    int tailleLimite_Y;


    //Récupération des coordonées du container contenant la Scene 3D
    QPoint coord = container->pos();
    int container_x = coord.x();
    int container_y = coord.y();


    // Prise du screenshoot
    QPixmap pixmap3D = QPixmap::grabWindow(this->winId(), container_x, container_y, -1, -1);

    //Fenêtre d'enregistrement
    QString filePath3D = QFileDialog::getSaveFileName(this, tr("Enregistrer sous"), "../Screenshot3D_1.png", tr("Images (*.png *.xpm *.jpg)"));

    //Sauvegarde de l'image
    pixmap3D.save(filePath3D);

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
    this->deleteLater();
    event->accept();

}

/*--------------------------------------------------------------------------
* Fonctions : centrage()
*
* Description : Appel de la fonction de recentrage de l'objet 3D dans la 
* classe My3DScene
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::centrage() {
    Visualisation3D->Recentrer();
}

void Widget3D::Actu3D() {
    Visualisation3D->mouse3DMove();
}


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
    Fichier = new QMenu("&Fichier");//Init menu fichier
    Affichage = new QMenu("&Affichage");//Init menu affichage

    //Actions liées aux différents menus
    Fichier->addAction("Enregistrer sous", this, SLOT(SaveAs3D()));
    Affichage->addAction("Recentrer", this, SLOT(centrage()));

    //Conversion de la Scene3d en QWidget et fixation de sa taille
    container = createWindowContainer(Visualisation3D);
    container->setFixedSize(999, 725);

    //Ajout des composants à leurs entités
    menu->addMenu(Fichier);
    menu->addMenu(Affichage);
    Layout->setMenuBar(menu);
    Layout->addWidget(container, 1, 0, Qt::AlignJustify);

    //Appel de la fonction mouse3DMove toutes les 10ms
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(Actu3D()));
    timer->start(10);

    //paramètres du widget
    setLayout(Layout);
    setFixedSize(1000, 800);
    setWindowTitle("Interface de reconstruction 3D des IRM");//titre fenetre

    show();
}
