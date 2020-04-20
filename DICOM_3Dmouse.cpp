#include "DICOM_3Dmouse.h"
#include "Scene3D.h"

//Initialisation des variables globales
INT Coupe, Min, Max;


/*--------------------------------------------------------------------------
* Fonction : ALLPixels()
*
* Description : Stockages de tous les pixels de chaque image dans un seul vecteur
*
* Arguments : pixels : vecteur contenant la valeur de tous les pixels d'une image
*             allpixels : vecteur contenant la valeur de tous les pixels de toutes images à la suite
*
* Valeur retournée : allpixels
*--------------------------------------------------------------------------*/
QVector<int>* ALLPixels(vector<unsigned short>* pixels, QVector<int>* allpixels)
{
    for (auto pixel2 : *pixels)
        allpixels->push_back(pixel2); //Remplissage du vecteur avec les valeurs des pixels
    return allpixels;
}

/*--------------------------------------------------------------------------
* Fonction : Supprimer()
*
* Description : Suppression des images et du dossier
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::Supprimer()
{
    if (*NbFichiers == 0) //Condition d'existance des images
        return;
    QDirIterator direction("Images", QDir::Files | QDir::NoSymLinks);// Obtenir l'arborescence des fichiers
    while (direction.hasNext()) //tant qu'il reste des dossier dans le fichier
        remove(direction.next().toStdString().c_str()); //Ajout de tous les chemins dans une liste
    _rmdir("Images");//Suppression du dossier
}

/*--------------------------------------------------------------------------
* Fonction : AppercuVisualisation3D()
*
* Description : Création d'un widget permettant l'appercu d'une coupe et contenant différents
* outils de parametrage (intensité, transparence et choix des coupes)
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::AppercuVisualisation3D()
{
    if (*NbFichiers == 0) //Condition d'existance des images
        return;

    delete Visualisation3D; //Libération de la mémoire

    Visualisation3D = new My3DScene(); //Création d'une scène 3D vide

    *souris3D = 0; //souris3D non active

    //Init du Widget
    WidgetAppercu3D = new QWidget;
    //Init des labels
    LabelVisuImage = new QLabel(); 
    LabelSaisieMin = new QLabel();
    LabelSaisieMax = new QLabel();
    LabelSliderIntensite = new QLabel();
    LabelSliderTransparence = new QLabel();
    //Init du layout
    LayoutVisuImage = new QGridLayout;
    //Init du menu déroulant de sélection des coupes
    comboBoxVisu = new QComboBox();
    //Init des curseurs
    SliderVisuIntensite = new QSlider(Qt::Horizontal);
    SliderVisuTransparence = new QSlider(Qt::Horizontal);
    //Init des zones de saisie
    LineEditSaisieMin = new QLineEdit();
    LineEditSaisieMax = new QLineEdit();
    //init du Bouton valider
    validerVisu = new QPushButton("Valider");

    *Mode = 3; //Mode 3 = Appercu

    //Appercu en fonction de la coupe selectionnée
    switch (*CoupeVisu)
    {
    case 0:
        ImageDICOM(slider1->value());
        break;
    case 1:
        ImageDICOM2(slider2->value());
        break;
    case 2:
        ImageDICOM3(slider3->value());
        break;
    }
    *Mode = 0; //Mode 0 = retour du mode à l'état initial

    //Attribution des paramètres
    SliderVisuIntensite->setRange(0, 255);
    SliderVisuTransparence->setRange(0, 255);
    LabelSaisieMin->setText("Image initiale :");
    LabelSaisieMax->setText("Image finale :");
    LabelSliderIntensite->setText("Reglage intensite :");
    LabelSliderTransparence->setText("Reglage transparence :");
    comboBoxVisu->addItem("Coupe 1");
    comboBoxVisu->addItem("Coupe 2");
    comboBoxVisu->addItem("Coupe 3");

    //Mise en place des connexions
    connect(comboBoxVisu, SIGNAL(activated(int)), this, SLOT(SelectCoupes(int)));
    connect(SliderVisuIntensite, SIGNAL(valueChanged(int)), this, SLOT(AfficheIntensiteTransparence()));
    connect(SliderVisuTransparence, SIGNAL(valueChanged(int)), this, SLOT(AfficheIntensiteTransparence()));
    connect(LineEditSaisieMin, SIGNAL(textChanged(QString)), this, SLOT(FirstImage(QString)));
    connect(LineEditSaisieMax, SIGNAL(textChanged(QString)), this, SLOT(LastImage(QString)));
    connect(validerVisu, SIGNAL(clicked()), this, SLOT(Enregistre()));

    //Ajout des outils au Layout
    LayoutVisuImage->addWidget(comboBoxVisu, 1, 1, 1, 1);
    LayoutVisuImage->addWidget(LabelSliderIntensite, 2, 0, 1, 1);
    LayoutVisuImage->addWidget(LabelSliderTransparence, 3, 0, 1, 1);
    LayoutVisuImage->addWidget(SliderVisuIntensite, 2, 1, 1, 2);
    LayoutVisuImage->addWidget(SliderVisuTransparence, 3, 1, 1, 2);
    LayoutVisuImage->addWidget(LabelSaisieMin, 4, 0, 1, 1);
    LayoutVisuImage->addWidget(LabelSaisieMax, 5, 0, 1, 1);
    LayoutVisuImage->addWidget(LineEditSaisieMin, 4, 1, 1, 2);
    LayoutVisuImage->addWidget(LineEditSaisieMax, 5, 1, 1, 2);
    LayoutVisuImage->addWidget(validerVisu, 6, 0, 1, 3);

    //Paramétrage du style du Widget
    WidgetAppercu3D->setWindowTitle("Apercu de visualisation pour reconstitution 3D");//titre fenetre
    WidgetAppercu3D->setWindowIcon(QIcon("icon.png"));//Mettre un Icon a la fenetre
    WidgetAppercu3D->setLayout(LayoutVisuImage);

    //Affichage fenêtre
    WidgetAppercu3D->show();

}

/*--------------------------------------------------------------------------
* Fonction : LastImage()
*
* Description : Récuperation de la dernière image choisie par l'utilisateur   
* et affiche de celle-ci en appercu
*
* Arguments : SaisieMax : Valeur max saisie  par l'utilisateur
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::LastImage(QString SaisieMax)
{
    //Verification si la valeur saisie est un entier
    if (!SaisieMax.toInt())
        return;

    //Mémorisation de l'entier saisi
    *imageMax = SaisieMax.toInt();

    *Mode = 3; //Mode 3 = Appercu

    //Init de la QMessageBox
    QMessageBox MessageErreur;
    MessageErreur.setText("Valeur saisie incorrecte");

    //Affichage de l'image saisie en fonction de la coupe
    switch (*CoupeVisu)
    {
    case 0:
        if (*imageMax < *NbFichiers && *imageMax >= 0)
            ImageDICOM(*imageMax);
        else {
            MessageErreur.exec();
            return;
        }
            
        break;
    case 1:
        if (*imageMax < *rows && *imageMax > 0)
            ImageDICOM2(*imageMax);
        else {
            MessageErreur.exec();
            return;
        }
        break;
    case 2:
        if (*imageMax < *cols && *imageMax > 0)
            ImageDICOM3(*imageMax);
        else {
            MessageErreur.exec();
            return;
        }
        break;
    }
    *Mode = 0; //Mode 0 = retour du mode à l'état initial 
}

/*--------------------------------------------------------------------------
* Fonction : FirstImage()
*
* Description : Récuperation de la première image choisie par l'utilisateur
* et affiche de celle-ci en appercu
*
* Arguments : SaisieMin : Valeur min saisie  par l'utilisateur
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::FirstImage(QString SaisieMin)
{
    //Verification si la valeur saisie est un entier
    if (!SaisieMin.toInt())
        return;

    //Mémorisation de l'entier saisi
    *imageMin = SaisieMin.toInt();

    *Mode = 3;//Mode 3 = Appercu

    //Init de la QMessageBox
    QMessageBox MessageErreur;
    MessageErreur.setText("Valeur saisie incorrecte");

    //Affichage de l'image saisie en fonction de la coupe
    switch (*CoupeVisu)
    {
    case 0:
        if (*imageMin >= 0 && *imageMin < *NbFichiers)
            ImageDICOM(*imageMin);
        else
            return;
        break;
    case 1:
        if (*imageMin >= 1 && *imageMin < *cols)
            ImageDICOM2(*imageMin);
        else
            return;
        break;
    case 2:
        if (*imageMin >= 1 && *imageMin < *rows)
            ImageDICOM3(*imageMin);
        else
            return;
        break;
    }
    *Mode = 0; //Mode 0 = retour du mode à l'état initial 

}

/*--------------------------------------------------------------------------
* Fonction : AfficheIntensiteTransparence()
*
* Description : Affichage de l'image saisie en fonction de la coupe et des
* valeurs d'intensité et de transparence choisies
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::AfficheIntensiteTransparence()
{
    *Mode = 3; //Mode 3 = Appercu

    //Affichage de l'image saisie en fonction de la coupe
    switch (*CoupeVisu)
    {
    case 0:
        ImageDICOM(slider1->value());
        break;
    case 1:
        ImageDICOM2(slider2->value());
        break;
    case 2:
        ImageDICOM3(slider3->value());
        break;
    }
    *Mode = 0; //Mode 0 = retour du mode à l'état initial 
}

/*--------------------------------------------------------------------------
* Fonction : SelectCoupes()
*
* Description : Affichage instantané de l'image saisie en fonction de la coupe 
* choisie
*
* Arguments : valeurCoupe : valeur renvoyée par le menu déroulant
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::SelectCoupes(int valeurCoupe)
{
    //Mémorisation de la valeur de la coupe
    *CoupeVisu = valeurCoupe;

    *Mode = 3;//Mode 3 = Appercu

    //Affichage de l'image saisie en fonction de la coupe
    switch (*CoupeVisu)
    {
    case 0:
        ImageDICOM(slider1->value());
        break;
    case 1:
        ImageDICOM2(slider2->value());
        break;
    case 2:
        ImageDICOM3(slider3->value());
        break;
    }
    *Mode = 0;

}

/*--------------------------------------------------------------------------
* Fonction : Enregistre()
*
* Description : Affichage instantané de l'image saisie en fonction de la coupe
* choisie
*
* Arguments : valeurCoupe : valeur renvoyée par le menu déroulant
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::Enregistre()
{
    //Fermeture de la fenêtre d'aperçue
    WidgetAppercu3D->close();

    Coupe = *CoupeVisu + 1;

    *Mode = 1;//Mode 1 = enregistrement

    *NbCouleurs = 0; //Image en nuances de gris

    //Association prmière et denière image à Scene3D
    Min = *imageMin;
    Max = *imageMax;

    //initilisation du NumImageTx
    int cpt = Min;

    //Barre de chargement
    QProgressDialog progress("Importation des images", "Cancel", Min, Max, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);
    progress.setMinimumDuration(0);

    //Enregistrement pour toutes les images entre la première et la dernière en fonction de la coupe
    switch (Coupe)
    {
    case 1:
        for (cpt; cpt <= Max; cpt++) {
            progress.setValue(cpt);
            ImageDICOM(cpt);
        }
        progress.setValue(Max);
        break;
    case 2:
        for (cpt; cpt <= Max; cpt++) {
            progress.setValue(cpt);
            ImageDICOM2(cpt);
        }
        progress.setValue(Max);
        break;
    case 3:
        for (cpt; cpt <= Max; cpt++) {
            progress.setValue(cpt);
            ImageDICOM3(cpt);
        }
        progress.setValue(Max);
        break;
    }

    *Mode = 0; //Mode 0 = retour du mode à l'état initial 

    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());

    //Libération de la mémoire
    delete WidgetAppercu3D;

    //Lancement interface 3D
    Visualisation3D = new My3DScene();
    Visualisation3D->show();
}

/*--------------------------------------------------------------------------
* Fonction : InfoCoupes()
*
* Description : Affichage du nom de la coupe correspondant à l'image
* 
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::InfoCoupes()
{
    if (*NbFichiers == 0) //Condition d'existance des images
        return;

    //Initialisation des varibles locales
    int nombre1, nombre2, nombre3, nombre4, nombre5, nombre6, nombre7;
    bool ok1, ok2, ok3, ok4, ok5, ok6, ok7;
    QString orientation1, orientation2, orientation6, orientation7;

    //Gestion de l'ouverture d'un fichier dcm
    QStringList Listchemin = *Listechemin;
    *pathFolderSave = Listchemin[*NbFichiers / 2]; //Fichier central
    QByteArray b = pathFolderSave->toLocal8Bit(); //Convertir le QString* en const char * 
    const char* chemin = b.data();
    dcm::DicomFile* data;
    vector<unsigned short>* pixels;

    //Lecture du fichier dcm
    data = readFile(chemin);
    if (data == NULL)
        return;
    
    //Récupération des valeurs d'orientation
    string orientationGLOBAL = getStringTagValue(0x00200037, data); //00200037->attribut d'orientation de l'image
    QString orientation = QString::fromStdString(orientationGLOBAL); //conversion en String
    
    //Supression des moins
    orientation.remove("-");

    //Séparation des différents éléments
    QStringList global2 = orientation.split("\\");

    //Récupération des éléments d'orientation et conversion en float
    float premier = global2[0].toFloat(&ok1);
    float deuxieme = global2[1].toFloat(&ok2);
    float troisieme = global2[2].toFloat(&ok3);
    float quatrieme = global2[3].toFloat(&ok4);
    float cinquieme = global2[4].toFloat(&ok5);
    float sixieme = global2[5].toFloat(&ok6);

    //Conditions si coupe Sagittale en première
    if (deuxieme < 1.2 && deuxieme > 0.8 && sixieme < 1.2 && sixieme > 0.8)
    {
        *coupe = 1; //Variable globale d'identification de la coupe

        //Affichage fixe du nom des coupes
        SpinBox1->setPrefix("Coupe Sagittale : ");
        SpinBox2->setPrefix("Coupe Transversale : ");
        SpinBox3->setPrefix("Coupe Coronale : ");
    }

    //Conditions si coupe Transversale en première
    if (premier < 1.2 && premier > 0.8 && cinquieme < 1.2 && cinquieme > 0.8)
    {
        *coupe = 2; //Variable globale d'identification de la coupe

        //Affichage fixe du nom des coupes
        SpinBox1->setPrefix("Coupe Transversale : ");
        SpinBox2->setPrefix("Coupe Coronale : ");
        SpinBox3->setPrefix("Coupe Sagittale : ");
    }

    //Conditions si coupe Coronale en première
    if (premier < 1.2 && premier > 0.8 && sixieme < 1.2 && sixieme > 0.8)
    {
        *coupe = 3; //Variable globale d'identification de la coupe

        //Affichage fixe du nom des coupes
        SpinBox1->setPrefix("Coupe Coronale : ");
        SpinBox2->setPrefix("Coupe Transversale : ");
        SpinBox3->setPrefix("Coupe Sagittale : ");
    }


}

/*--------------------------------------------------------------------------
* Fonction : displayTags()
*
* Description : Fonction ressortant les informations du patient dans une boite de dialogue
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::displayTags()
{
    if (*NbFichiers == 0) //Condition d'existance des images
        return;

    //Initialisation des variables gloables
    QString nom, genre, Date, type, photo, Datetude;

    //Gestion de l'ouverture d'un fichier dcm
    QByteArray b = pathFolderSave->toLocal8Bit(); //Convertir le QString* en const char *
    const char* chemin = b.data();
    dcm::DicomFile data(chemin);

    //Initialisation de la QMessageBox
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);

    //Verfification d'existance du fichier
    if (!data.Load()) {
        msg.setText("ERREUR DE FICHIER");
    }
    string Name;
    if (data.GetString(dcm::tags::kPatientName, &Name))
        nom = QString::fromStdString(Name); //Nom patient

    string DateEtude;
    if (data.GetString(dcm::tags::kStudyDate, &DateEtude))
        Datetude = QString::fromStdString(DateEtude); //Date etude patient

    string Sexe;
    if (data.GetString(dcm::tags::kPatientSex, &Sexe))
        genre = QString::fromStdString(Sexe); //Sexe patient

    string Birth;
    if (data.GetString(dcm::tags::kPatientBirthDate, &Birth))
        Date = QString::fromStdString(Birth);//date de naissance du patient

    string Type;
    if (data.GetString(dcm::tags::kImageType, &Type))
        type = QString::fromStdString(Type);//Type d'image

    string Photo;
    if (data.GetString(dcm::tags::kPhotometricInterpretation, &Photo))
        photo = QString::fromStdString(Photo);//Phototmetric

    //Supression du ^ entre le nom et le prénom
    nom.replace("^", " ");

    //Affichage des infomations (avec date en norme française)
    msg.setText("Nom du patient : " + nom + "\n" +
        "Sexe : " + genre + "\n" +
        "Date de naissance : " + Date[6] + Date[7] + "/" + Date[4] + Date[5] + "/" + Date[0] + Date[1] + Date[2] + Date[3] + "\n" +
        "Date de l'etude : " + Datetude[6] + Datetude[7] + "/" + Datetude[4] + Datetude[5] + "/" + Datetude[0] + Datetude[1] + Datetude[2] + Datetude[3] + "\n" +
        "Type d'images : " + type + "\n" +
        "Interpretation photometrique : " + photo + "\n"); //Ajout à la boite QMessageBox
    msg.setWindowTitle("Informations patient");
    msg.setWindowIcon(QIcon("patient.png"));

    //Affichage boite de dialogue
    msg.exec();
}

/*--------------------------------------------------------------------------
* Fonction : ouvrirFichiers()
*
* Description : Sélection d'un dossier, ouverture de chauqe fichiers du dossier
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::ouvrirFichiers() //Ouvrir le dossier l'image en fonction du positionnement du curseur
{
    pathFolder = new QString();
    *pathFolder = QFileDialog::getExistingDirectory(this, "Explorateur de fichiers");//Boite de dialogue sélection dossier
    
    //Condition d'existence du dossier
    if (pathFolder->isEmpty() || pathFolder->isNull())
        return;

    //Mémorisation
    pathFolderSave = pathFolder;//Pour eviter de lire un dossier vide dans les infos patients en cas d'annulation de l'ouverture.
    
    //Initialisation des variables globales de l'intensité max de chaque images
    IntensiteMaxInitCoupe1 = new qint16;
    IntensiteMaxInitCoupe2 = new qint16;
    IntensiteMaxInitCoupe3 = new qint16;

    //Initialisation des variables globales de l'intensité variables de chaque images
    IntensiteVariableCoupe1 = new qint16;
    IntensiteVariableCoupe2 = new qint16;
    IntensiteVariableCoupe3 = new qint16;
    *IntensiteVariableCoupe1 = 0;
    *IntensiteVariableCoupe2 = 0;
    *IntensiteVariableCoupe3 = 0;

    //Initialisation de la variable pour gerer le colormap
    NbCouleurs = new qint16;
    *NbCouleurs = 0; //O = valeur "nuance de gris"

    //Initialisation de la variables globale souris3D
    *souris3D = 0; //souris3D non active

    //Initialisation de la liste des chemins de fichier
    Listechemin = new QStringList();//Liste de QString

    //Initialisation du vecteur contenant tous les pixels
    allpixels = new QVector<int>;

    //Stockage des chemins de fichier
    QDirIterator direction(*pathFolder, QDir::Files | QDir::NoSymLinks);// Obtenir l'arborescence des fichiers
    while (direction.hasNext()) //tant qu'il reste des dossier dans le fichier
        *Listechemin << direction.next(); //Ajout de tous les chemins dans une liste

    //Mise en locale d'une variable globale
    QStringList Listchemin = *Listechemin;

    //Récupération du nombre total de fichiers dans le dossier
    *NbFichiers = Listchemin.length();

    //Gestion d'ouverture et de lecture de tous les fichiers du dossier
    for (int NbFichier = 0; NbFichier < *NbFichiers; NbFichier++)
    {
        *pathFolder = Listchemin[NbFichier]; //Selection du chemin selon la boucle

        //Convertion le QString* en const char * 
        QByteArray b = pathFolder->toLocal8Bit(); 
        const char* chemin = b.data();

        //Initialisation d'un fichier dcm
        dcm::DicomFile* dataDcm;

        //Initialisation du vecteur contenant les pixels
        vector<unsigned short>* pixels;

        //Lecture du fichier
        dataDcm = readFile(chemin);//Lecture du fichier

        if (dataDcm == NULL)//Condition d'existence du fichier
            return;

        *rows = getUShortTagValue(0x00280010, dataDcm);//Nombre de lignes
        *cols = getUShortTagValue(0x00280011, dataDcm);//Nombre de colonnes

        pixels = readPixels(dataDcm); //Lecture des pixesls
        allpixels = ALLPixels(pixels, allpixels); //Tous les pixels stockés dans un vecteur

        //Libération de la mémoire
        delete(dataDcm);
        delete(pixels);

    }

    //-----------------------Paramétrage et positionnement des outils------------------------
    SpinBox1->setButtonSymbols(QSpinBox::NoButtons);
    SpinBox2->setButtonSymbols(QSpinBox::NoButtons);
    SpinBox3->setButtonSymbols(QSpinBox::NoButtons);

    SpinBox1->setRange(0, *NbFichiers - 1);
    SpinBox2->setRange(0, *rows - 1);
    SpinBox3->setRange(0, *cols - 1);

    SpinBox1->setStyleSheet("QSpinBox { border: 0px solid grey; border-radius: 4px; background-color: rgb(230,230,230); color: black }");
    SpinBox2->setStyleSheet("QSpinBox { border: 0px solid grey; border-radius: 4px; background-color: rgb(230,230,230); color: black }");
    SpinBox3->setStyleSheet("QSpinBox { border: 0px solid grey; border-radius: 4px; background-color: rgb(230,230,230); color: black }");
    
    slider1->setRange(0, *NbFichiers - 1);
    slider2->setRange(1, *rows - 1); //Valeurs du slider selon nb de fichiers
    slider3->setRange(1, *cols); //Valeurs du slider selon nb de fichiers
    slider1->setValue(*NbFichiers / 2);//Positionnement du cuseur a la moitié
    slider2->setValue(*rows / 2);//Positionnement du cuseur a la moitié
    slider3->setValue(*cols / 2);//Positionnement du cuseur a la moitié

    layout->addWidget(SpinBox1, 0, 0, Qt::AlignCenter);
    layout->addWidget(SpinBox2, 0, 1, Qt::AlignCenter);
    layout->addWidget(SpinBox3, 0, 2, Qt::AlignCenter);
    layout->addWidget(slider1, 2, 0,Qt::AlignBottom);
    layout->addWidget(slider2, 2, 1, Qt::AlignBottom);
    layout->addWidget(slider3, 2, 2, Qt::AlignBottom);
    //--------------------------------------------------------------------------------

    //Initialisation des paramètres de navigation avec la souris 3D
    *lastTxValue = 0;
    *NumImageTx = *NbFichiers / 2;
    *lastTyValue = 0;
    *NumImageTy = *rows / 2;
    *lastTzValue = 0;
    *NumImageTz = *cols / 2;
    *lastRyValue = 0;
    *variationIntensite = 0;// sliderIntensite->value();

    //Récuprération des infos des coupes
    InfoCoupes();

    //Affichage des images selon les curseurs
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());

    //En cas de réouverture de fichier slider d'intensité masqué
    layout->removeWidget(sliderIntensite);//Pas de widgets intensité
    sliderIntensite->setVisible(false);//Non visible
}

/*--------------------------------------------------------------------------
* Fonction : AfficherCurseurIntensite()
*
* Description : Affichage du curseur de variation d'intensité
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::AfficherCurseurIntensite()
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;
    sliderIntensite->setRange(-500, 500); //Nuances d'intensité
    sliderIntensite->setValue(0);//Init a 0 -> valMax réelle
    sliderIntensite->setVisible(true);//rendre visible le curseur
    layout->addWidget(sliderIntensite, 3, 0, 1, 3);//Position
}

/*--------------------------------------------------------------------------
* Fonction : UtiliserSouris3D()
*
* Description : Activation ou non de la souris 3D
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::UtiliserSouris3D() 
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Mise en locale d'une variable globale
    int value = *souris3D;

    //Condition On/Off
    if (value == 0)
        *souris3D = 1;
    else
        *souris3D = 0;
}

/*--------------------------------------------------------------------------
* Fonctions : MajClicCoupe1(),MajClicCoupe2(),MajClicCoupe3()

* Description : Mise à jour de 2 images en fonction du clic sur la troisième
*
* Arguments : e : évenement de la souris classique
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::MajClicCoupe1(QMouseEvent* e)
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Initialisation des tailles limites
    int tailleLimite_X;
    int tailleLimite_Y;

    //Initialisation des numéros des deux nouvelles images
    int NouvelleImageCoupe3;
    int NouvelleImageCoupe2;

    //Récupération des coordonées du clic
    QPoint PositionClic = e->pos();
    int posi_x = PositionClic.x();
    int posi_y = PositionClic.y();

    //Récupération des coordonées de la coupe 1
    QPoint coord = imageLabel1->pos();
    int label_x = coord.x();
    int label_y = coord.y();

    //Condition de clic sur le bouton gauche
    if (e->button() == Qt::LeftButton) {
        if (*rows < 400 && *cols < 400) //Si image de petite taille
        {
            //facteur 1.75 pour prendre en compte le zoom
            tailleLimite_X = (label_x + *cols * 1.75);
            tailleLimite_Y = (label_y + *rows * 1.75);
            NouvelleImageCoupe3 = (posi_x - label_x) / 1.75;
            NouvelleImageCoupe2 = (posi_y - label_y) / 1.75;
        }
        else { //Si grande image
            tailleLimite_X = (label_x + *cols);
            tailleLimite_Y = (label_y + *rows);
            NouvelleImageCoupe3 = (posi_x - label_x);
            NouvelleImageCoupe2 = (posi_y - label_y);
        }
        //Conditon tant qu'on clic sur l'image
        if (posi_x > label_x && posi_x< tailleLimite_X && posi_y>label_y && posi_y < tailleLimite_Y) {

            if (*souris3D == 0) { //Si souris 3D désactivée
                //Affichage des nouvelles images avec maj des sliders
                ImageDICOM3(NouvelleImageCoupe3);
                ImageDICOM2(NouvelleImageCoupe2);
                slider3->setValue(NouvelleImageCoupe3);
                slider2->setValue(NouvelleImageCoupe2);
            }
            else if (*souris3D == 1) { //Si souris 3D activée
                //Mémorisation des images sélectionnées au clic
                *NumImageTy = NouvelleImageCoupe2;
                *NumImageTz = NouvelleImageCoupe3;

                //Affichage des nouvelles images
                Action3DMouseTy();
                Action3DMouseTz();
            }
            
        }

    }

}
void Interface::MajClicCoupe2(QMouseEvent* e)
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;
    
    //Initialisation des tailles limites
    int tailleLimite_X;
    int tailleLimite_Y;

    //Initialisation des numéros des deux nouvelles images
    int NouvelleImageCoupe1;
    int NouvelleImageCoupe3;

    //Récupération des coordonées du clic
    QPoint PositionClic = e->pos();
    int posi_x = PositionClic.x();
    int posi_y = PositionClic.y();

    //Récupération des coordonées de la coupe 2
    QPoint coord = imageLabel2->pos();
    int label_x = coord.x();
    int label_y = coord.y();
    
    //Condition de clic sur le bouton gauche
    if (e->button() == Qt::LeftButton) {
        if (*rows < 400 && *cols < 400) //Si image de petite taille
        {
            //facteur 1.75 pour prendre en compte le zoom
            tailleLimite_X = (label_x + *NbFichiers * 1.75);
            tailleLimite_Y = (label_y + *rows * 1.75);
            NouvelleImageCoupe1 = (posi_x - label_x) / 1.75;
            NouvelleImageCoupe3 = (posi_y - label_y) / 1.75;
        }
        else { //Si grande image
            tailleLimite_X = (label_x + *NbFichiers);
            tailleLimite_Y = (label_y + *rows);
            NouvelleImageCoupe1 = (posi_x - label_x);
            NouvelleImageCoupe3 = (posi_y - label_y);
        }
        //Conditon tant qu'on clic sur l'image
        if (posi_x > label_x && posi_x< tailleLimite_X && posi_y>label_y && posi_y < tailleLimite_Y) {
            if (*souris3D == 0) { //Si souris 3D désactivée
                //Affichage des nouvelles images avec maj des sliders
                ImageDICOM3(NouvelleImageCoupe3);
                ImageDICOM(*NbFichiers - NouvelleImageCoupe1);
                slider1->setValue(*NbFichiers - NouvelleImageCoupe1);
                slider3->setValue(NouvelleImageCoupe3);
            }
            else if (*souris3D == 1) {//Si souris 3D activée
                //Mémorisation des images sélectionnées au clic
                *NumImageTx = *NbFichiers - NouvelleImageCoupe1;
                *NumImageTz = NouvelleImageCoupe3;

                //Affichage des nouvelles images
                Action3DMouseTx();
                Action3DMouseTz();
            }

        }

    }

}
void Interface::MajClicCoupe3(QMouseEvent* e)
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Initialisation des tailles limites
    int tailleLimite_X;
    int tailleLimite_Y;

    //Initialisation des numéros des deux nouvelles images
    int NouvelleImageCoupe1;
    int NouvelleImageCoupe2;

    //Récupération des coordonées du clic
    QPoint PositionClic = e->pos();
    int posi_x = PositionClic.x();
    int posi_y = PositionClic.y();

    //Récupération des coordonées de la coupe 3
    QPoint coord = imageLabel3->pos();
    int label_x = coord.x();
    int label_y = coord.y();

    //Condition de clic sur le bouton gauche
    if (e->button() == Qt::LeftButton) {
        if (*rows < 400 && *cols < 400) //Si image de petite taille
        {
            //facteur 1.75 pour prendre en compte le zoom
            tailleLimite_X = (label_x + *NbFichiers * 1.75);
            tailleLimite_Y = (label_y + *rows * 1.75);
            NouvelleImageCoupe1 = (posi_x - label_x) / 1.75;
            NouvelleImageCoupe2 = (posi_y - label_y) / 1.75;
        }
        else { //Si grande image
            tailleLimite_X = (label_x + *NbFichiers);
            tailleLimite_Y = (label_y + *rows);
            NouvelleImageCoupe1 = (posi_x - label_x);
            NouvelleImageCoupe2 = (posi_y - label_y);
        }
        //Conditon tant qu'on clic sur l'image
        if (posi_x > label_x && posi_x< tailleLimite_X && posi_y>label_y && posi_y < tailleLimite_Y) {
            if (*souris3D == 0) { //Si souris 3D désactivée
                //Affichage des nouvelles images avec maj des sliders
                ImageDICOM(*NbFichiers - NouvelleImageCoupe1);
                ImageDICOM2(NouvelleImageCoupe2);
                slider1->setValue(*NbFichiers - NouvelleImageCoupe1);
                slider2->setValue(NouvelleImageCoupe2);
            }
            else if (*souris3D == 1) { //Si souris 3D activée
                //Mémorisation des images sélectionnées au clic
                *NumImageTx = *NbFichiers - NouvelleImageCoupe1;
                *NumImageTy = NouvelleImageCoupe2;

                //Affichage des nouvelles images
                Action3DMouseTx();
                Action3DMouseTy();
            }
        }

    }

}

/*--------------------------------------------------------------------------
* Fonctions : AffichageOriginal(), AffichageJet(), AffichageBone(),
* AffichageCivids(), AffichageTurbo(), AffichageHot(), AffichageParula(),
* AffichageTwilightShifted()
*
* Description : Affectation de la valeur correspondant à la couleur
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::AffichageOriginal()
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    if (*visible % 2 == 0) //On affiche une fois sur deux le slider intensité
    {
        layout->removeWidget(sliderIntensite);
        sliderIntensite->setVisible(false);
    }
    //Incrémentation pour la visibilité ou non du slider
    *visible += 1;

    //Retour à l'intensité originale
    sliderIntensite->setValue(0);
    *IntensiteVariableCoupe1, * IntensiteVariableCoupe2, * IntensiteVariableCoupe3 = 0;

    //Valeur de condition couleur
    *NbCouleurs = 0; //0 = Nuance de gris

    //Affichage des images
    ImageDICOM(slider1->value()); 
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());

    
}
void Interface::AffichageJet()
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Valeur de condition couleur
    *NbCouleurs = 1; //1 = jet

    //Affichage des images
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}
void Interface::AffichageBone()
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Valeur de condition couleur
    *NbCouleurs = 2;//2 = Bone

    //Affichage des images
    ImageDICOM(slider1->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::AffichageCivids()
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Valeur de condition couleur
    *NbCouleurs = 3;//3 = Civids

    //Affichage des images
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}
void Interface::AffichageTurbo()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Valeur de condition couleur
    *NbCouleurs = 4;//4 = Turbo

    //Affichage des images
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}
void Interface::AffichageHot()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Valeur de condition couleur
    *NbCouleurs = 5;//5 = Hot

    //Affichage des images
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}
void Interface::AffichageParula()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Valeur de condition couleur
    *NbCouleurs = 6;//6 = Parula

    //Affichage des images
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}
void Interface::AffichageTwilightShifted()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0) //Condition d'existence du dossier
        return;

    //Valeur de condition couleur
    *NbCouleurs = 7;//7 = Twilight Shifted

    //Affichage des images
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}

/*--------------------------------------------------------------------------
* Fonctions : ChangementIntensite()
*
* Description : Prend en argument la valeur du slider quand il y a un changement
*
* Arguments : valueIntensite : valeur du slider intensité
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::ChangementIntensite(int valueIntensite)
{
    //Changement de la valeur max d'intensité de référence
    *IntensiteVariableCoupe1 = *IntensiteMaxInitCoupe1 + valueIntensite;
    *IntensiteVariableCoupe2 = *IntensiteMaxInitCoupe2 + valueIntensite;
    *IntensiteVariableCoupe3 = *IntensiteMaxInitCoupe3 + valueIntensite;

    //Affichage des images
    ImageDICOM(slider1->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}

/*--------------------------------------------------------------------------
* Fonctions : ActionSlider1(), ActionSlider2(), ActionSlider3()
*
* Description : Prend en argument la valeur du slider quand il y a un changement
* et affiche cette valeur dans le spinBox et affiche image correspondante
*
* Arguments : valueSlider1, valueSlider2, valueSlider3 : valeurs des sliders 
* position
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::ActionSlider1(int valueSlider1)
{
    //Appel de la fonction ImageDICOM pour afficher une image dicom
    ImageDICOM(valueSlider1);
    //Maj du SpinBox
    SpinBox1->setValue(slider1->value());
}
void Interface::ActionSlider2(int valueSlider2)
{
    //Appel de la fonction ImageDICOM pour afficher une image dicom
    ImageDICOM2(valueSlider2);
    //Maj du SpinBox
    SpinBox2->setValue(slider2->value());
}
void Interface::ActionSlider3(int valueSlider3)
{
    //Appel de la fonction ImageDICOM pour afficher une image dicom
    ImageDICOM3(valueSlider3);
    //Maj du SpinBox
    SpinBox3->setValue(slider3->value());
}

/*--------------------------------------------------------------------------
* Fonctions : ActionSpin1(), ActionSpin2(), ActionSpin3()
*
* Description : Affichage de l'image et mise à jour du slider en fonction de 
* la valeur indiquée dans le SpinBox
*
* Arguments : valueSpin1, valueSpin2, valueSpin3 : valeurs des saisies dans 
* les spinBoxs
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::ActionSpin1(int valueSpin1)
{
    //Appel de la fonction ImageDICOM pour afficher une image dicom
    ImageDICOM(valueSpin1); 
    //Maj du slider
    slider1->setValue(SpinBox1->value());
}
void Interface::ActionSpin2(int valueSpin2) //Récuperer la valeur du curseur lorsqu'il est déplacé
{
    //Appel de la fonction ImageDICOM pour afficher une image dicom
    ImageDICOM2(valueSpin2); 
    //Maj du slider
    slider2->setValue(SpinBox2->value());
}
void Interface::ActionSpin3(int valueSpin3) //Récuperer la valeur du curseur lorsqu'il est déplacé
{
    //Appel de la fonction ImageDICOM pour afficher une image dicom
    ImageDICOM3(valueSpin3); 
    //Maj du slider
    slider3->setValue(SpinBox3->value());
}

/*--------------------------------------------------------------------------
* Fonctions : Action3DMouseTx(), Action3DMouseTy(), Action3DMouseTz()
*
* Description : Navigation avec la souris 3D selon les axes x, y et z dans
* les 3 coupes
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::Action3DMouseTx() {
    //Condition d'utilisation de la souris
    int value = *souris3D;
    if (value == 0) //Si souris 3D off
        return;
    //Récupération du nombre d'images suivant la position de la coupe sagittale
    int NbImages;
    switch (*coupe)
    {
    case 1:
        NbImages = *NbFichiers;
        break;
    case 2:
        NbImages = *cols;
        break;
    case 3:
        NbImages = *cols;
        break;
    }

    //Récupération du numéro de l'image précédemment affichée
    int i = *NumImageTx;

    //Condition de navigation entre 0 et le nombre d'images
    if ((i > 0) && (i < NbImages)) {
        //Affichage des images selon position de la coupe sagittale
        switch (*coupe)
        {
        case 1:
            ImageDICOM(i);
            break;
        case 2:
            ImageDICOM3(i);
            break;
        case 3:
            ImageDICOM3(i);
            break;
        }

        //Conditions de navigation dans les images
        if ((pTx > 5) &&(pTx >= * lastTxValue) && (pTx<20)){
            i=i-1;
        }
        else if ((pTx >= 20) && (pTx >= * lastTxValue) && (pTx < 300)) {
            i = i-3;
        }
        else if ((pTx < -5) && (pTx <= * lastTxValue) && (pTx > -20)) {
            i=i+1;
        }
        else if ((pTx <= -20) && (pTx <= *lastTxValue) && (pTx > -300)) {
            i = i+3;
        }
    }
    
    else if(i <= 0 ){
        i = 1;
    }
    else if (i >= *NbFichiers) {
        i = *NbFichiers - 1;
    }

    //Mise à jour du slider en fonction de la postion de la coupe sagittale
    switch (*coupe)
    {
    case 1:
        slider1->setValue(i);
        break;
    case 2:
        slider3->setValue(i);
        break;
    case 3:
        slider3->setValue(i);
        break;
    }
    //Mémorisation du numéro d'image
    *NumImageTx = i;

    //Mémorisation de la dernière valeur TX prise par la souris 3D
    *lastTxValue = pTx;
}
void Interface::Action3DMouseTy() {
    //Condition d'utilisation de la souris
    int value = *souris3D;
    if (value == 0)
        return;

    //Récupération du nombre d'images suivant la position de la coupe transversale
    int NbImages;
    switch (*coupe)
    {
    case 1:
        NbImages=*rows;
        break;
    case 2:
        NbImages = *NbFichiers;
        break;
    case 3:
        NbImages = *rows;
        break;
    }

    //Récupération du numéro de l'image précédemment affichée
    int i = *NumImageTy;

    //Condition de navigation entre 0 et le nombre d'images
    if ((i > 0) && (i < NbImages)) {
        //Affichage des images selon position de la coupe tranversale
        switch (*coupe)
        {
        case 1:
            ImageDICOM2(i);
            break;
        case 2:
            ImageDICOM(i);
            break;
        case 3:
            ImageDICOM2(i);
            break;
        }

        //Conditions de navigation dans les images
        if ((pTy > 5) && (pTy >= * lastTyValue) && (pTy < 20)) {
            i = i - 1;
        }
        else if ((pTy >= 20) && (pTy >= * lastTyValue) && (pTy < 150)) {
            i = i - 2;
        }
        else if ((pTy < -5) && (pTy <= *lastTyValue) && (pTy > -20)) {
            i = i + 1;
        }
        else if ((pTy <= -20) && (pTy <= *lastTyValue) && (pTy > -150)) {
            i = i + 2;
        }
        
    }
    else if (i <= 0) {
        i = 1;
    }
    else if (i >= *rows) {
        i = *cols - 1;
    }
    else if (pTy == 0 && pTy < 50) {
        return;
    }

    //Mise à jour du slider en fonction de la postion de la coupe transversale
    switch (*coupe)
    {
    case 1:
        slider2->setValue(i);
        break;
    case 2:
        slider1->setValue(i);
        break;
    case 3:
        slider2->setValue(i);
        break;
    }

    //Mémorisation du numéro d'image
    *NumImageTy = i;

    //Mémorisation de la dernière valeur TY prise par la souris 3D
    *lastTyValue = pTy;
}
void Interface::Action3DMouseTz() {
    //Condition d'utilisation de la souris
    int value = *souris3D;
    if (value == 0)
        return;

    //Récupération du nombre d'images suivant la position de la coupe coronale
    int NbImages;
    switch (*coupe)
    {
    case 1:
        NbImages = *cols;
        break;
    case 2:
        NbImages = *rows;
        break;
    case 3:
        NbImages = *NbFichiers;
        break;
    }

    //Récupération du numéro de l'image précédemment affichée
    int i = *NumImageTz;

    //Condition de navigation entre 0 et le nombre d'images
    if ((i > 0) && (i < NbImages)) {
        //Affichage des images selon position de la coupe coronal
        switch (*coupe)
        {
        case 1:
            ImageDICOM3(i);
            break;
        case 2:
            ImageDICOM2(i);
            break;
        case 3:
            ImageDICOM(i);
            break;
        }

        //Conditions de navigation dans les images
        if ((pTz > 5) && (pTz >= * lastTzValue) && (pTz <20)) {
            i = i - 1;
        }
        else if ((pTz >= 20) && (pTz >= * lastTzValue) && (pTz < 250)) {
            i = i - 2;
        }
        else if ((pTz < -5) && (pTz <= *lastTzValue) && (pTz > -20)) {
            i = i + 1;
        }
        else if ((pTz <= -20) && (pTz <= *lastTzValue) && (pTz > -250)) {
            i = i + 2;
        }
        
    }

    else if (i <= 0) {
        i = 1;
    }
    else if (i >= *cols) {
        i = *cols - 1;
    }
    else if (pTz == 0 && pTy < 50) {
        return;
    }

    //Mise à jour du slider en fonction de la postion de la coupe coronale
    switch (*coupe)
    {
    case 1:
        slider3->setValue(i);
        break;
    case 2:
        slider2->setValue(i);
        break;
    case 3:
        slider1->setValue(i);
        break;
    }

    //Mémorisation du numéro d'image
    *NumImageTz = i;
    
    //Mémorisation de la dernière valeur TZ prise par la souris 3D
    *lastTzValue = pTz;
}

/*--------------------------------------------------------------------------
* Fonctions : Action3DMouseIntensite()
*
* Description : Modification de l'intensité des 3 coupes avec la souris 3D 
* selon l'axe de rotation y 
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Interface::Action3DMouseIntensite() {
    //Condition d'utilisation de la souris et si clic sur bouton droit de la souris 3D
    int value = *souris3D;
    int inte = Intensite; //vaut 1 au 1er clic et 0 au 2e clic sur bouton droite
    if ((value == 0) || (inte == 0)) {
        return;
    }

    //Affichage curseur d'intensité
    sliderIntensite->setVisible(true);//rendre visible le curseur

    //Récupération de la valeur d'intensité précédente
    int i = *variationIntensite;

    int lim = 500;//Limite de variation de l'intensité

    //Condition pour rester dans les limites de variation de l'intensité
    if ((i > -lim) && (i < lim)) {
        //Changement de la valeur max d'intensité de référence pour chaque coupe
        *IntensiteVariableCoupe1 = *IntensiteMaxInitCoupe1 + i;
        *IntensiteVariableCoupe2 = *IntensiteMaxInitCoupe2 + i;
        *IntensiteVariableCoupe3 = *IntensiteMaxInitCoupe2 + i;

        //Affichage des images et navigation avec souris 3D
        Action3DMouseTx();
        Action3DMouseTy();
        Action3DMouseTz();
        
        //Conditions de modification de l'intensité
        if ((pRy > 5) && (pRy >= *lastRyValue)&&(pRy<lim)) {
            i=i+50;
        }
        else if ((pRy < -5) && (pRy <= *lastRyValue) && (pRy> -lim)) {
            i = i-50;
        }
    }
    else if (i <= -lim) {
        i = 1-lim ;
    }
    else if (i >= lim) {
        i = lim-1;
    }

    //Mise à jour du slider intensité
    sliderIntensite->setValue(i);

    //Mémorisation de la valeur d'intensité
    *variationIntensite = i;

    //Mémorisation de la dernière valeur RY prise par la souris 3D
    *lastRyValue = pRy;
}

//----------Coupe coronale------------------
void Interface::ImageDICOM3(int v)
{
    Mat image = Mat::zeros(*NbFichiers - 1, *rows, CV_8UC1);//Image de la taille obtenue avec data
    int l = image.rows;
    int c = image.cols;
    *IntensiteMaxInitCoupe3 = 0;//globale
    int valMax = 0;//Locale
    int k = 0;
    vector<int>Valeurdefinitif2; //Nouveau vecteur contenant les pixels de l'image
    for (int nb = v; nb < (*allpixels).size(); nb += *cols)//Condition pour avoir l'image selon la bonne coupe 
    {
        Valeurdefinitif2.push_back((*allpixels)[nb]);//Vecteur definitif avec valeurs de l'image a afficher (tel colonnes de chaque image)
    }
    for (k = 0; k < Valeurdefinitif2.size(); k++) //Boucle pour avoir val max intensité
    {
        if (Valeurdefinitif2[k] > * IntensiteMaxInitCoupe3 && Valeurdefinitif2[k] < 2500)
            *IntensiteMaxInitCoupe3 = Valeurdefinitif2[k];//Isolement de l'intensité max
    }

    QImage dest; //Init QImage a ajouter a la fenetre Qt
    k = 0;
    if (*IntensiteVariableCoupe3 == 0)//Si il n'y a pas de changement de l'intensité
        valMax = *IntensiteMaxInitCoupe3; //Val max conservée
    else
        valMax = *IntensiteVariableCoupe3;//Sinon modifiée
    for (int i = 0; i < l; i++)
        for (int j = 0; j < c; j++)
        {
            if (valMax == 0)
                image.at<unsigned char>(i, j) = Valeurdefinitif2[k]; //Association de la valeur au bon endroit de l'image
            else
                image.at<unsigned char>(i, j) = (Valeurdefinitif2[k] * 255) / valMax; //Association de la valeur au bon endroit de l'image            k++;
            k++;
        }
    rotate(image, image, cv::ROTATE_90_CLOCKWISE); //Rotation de 90° de l'image
    switch (*NbCouleurs)
    {
    case 0:
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8); //Conversion d'un MAT en QImage
        break;
    case 1:
        applyColorMap(image, image, COLORMAP_JET);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 2:
        applyColorMap(image, image, COLORMAP_BONE);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 3:
        applyColorMap(image, image, COLORMAP_CIVIDIS);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 4:
        applyColorMap(image, image, COLORMAP_TURBO);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 5:
        applyColorMap(image, image, COLORMAP_HOT);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 6:
        applyColorMap(image, image, COLORMAP_PARULA);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 7:
        applyColorMap(image, image, COLORMAP_TWILIGHT_SHIFTED);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    }
    if (l < 400 && c < 400) //Si image de petite taille
    {
        l = 1.75 * l;//Coeff de 1.75
        c = 1.75 * c;//Coeff de 1.75
    }
    if (*Mode == 1 && *NbCouleurs == 0)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);

        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < SliderVisuTransparence->value())
                {
                    // set alpha to zero:
                    pixel[3] = 0;
                }
                else {
                    pixel[0] -= SliderVisuIntensite->value();
                    pixel[1] -= SliderVisuIntensite->value();
                    pixel[2] -= SliderVisuIntensite->value();
                    pixel[3] = 1;
                }
            }
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Images/Coupe3_" + numero + format;
        imwrite(cheminimage, input_bgra);
    }
    if (*Mode == 3 && *NbCouleurs == 0 && *CoupeVisu == 2)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);
        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < SliderVisuTransparence->value())
                {
                    // set alpha to zero:
                    pixel[0] = 255;
                    pixel[1] = 255;
                    pixel[2] = 255;
                    pixel[3] = 1;
                }
                else {
                    pixel[0] -= SliderVisuIntensite->value();
                    pixel[1] -= SliderVisuIntensite->value();
                    pixel[2] -= SliderVisuIntensite->value();
                    pixel[3] = 1;
                }
            }
        QImage visu = QImage((uchar*)input_bgra.data, input_bgra.cols, input_bgra.rows, input_bgra.step, QImage::Format_RGBX8888); //Conversion d'un MAT en QImage
        LabelVisuImage->setPixmap(QPixmap::fromImage(visu).scaled(QSize(l, c), Qt::IgnoreAspectRatio)); //Ajoute au layout
        LayoutVisuImage->addWidget(LabelVisuImage, 0, 0, 1, 3, Qt::AlignHCenter);//Ajout du layout à l'image
    }

    if (*Mode == 0)
    {
        //SpinBox3->setValue(slider3->value());

        imageLabel3->setPixmap(QPixmap::fromImage(dest).scaled(QSize(l, c), Qt::IgnoreAspectRatio)); //Ajoute au layout
        imageLabel3->setMaximumSize(l, c);
        layout->addWidget(imageLabel3, 1, 2, Qt::AlignHCenter);//Ajout du layout à l'image
    }
}
//-------------------Coupe Axiale------------------------
void Interface::ImageDICOM2(int v)
{

    Mat image = Mat::zeros(*NbFichiers - 1, *cols, CV_8UC1);//Image de la taille obtenue avec data
    int l = image.rows;
    int c = image.cols;
    *IntensiteMaxInitCoupe2 = 0;//globale
    int valMax = 0;//locale
    int k = 0;
    vector<int> Valeurdefinitif;
    for (int changementImag = 0; changementImag < (*allpixels).size(); changementImag += (*cols * *rows))
        for (int nb = changementImag + (v * *cols - 1); nb < changementImag + ((v + 1) * *cols - 1); nb++)//Condition pour avoir l'image selon la bonne coupe (tel ligne)
        {
            Valeurdefinitif.push_back((*allpixels)[nb]);
        }
    for (k = 0; k < Valeurdefinitif.size(); k++)
    {
        if (Valeurdefinitif[k] > * IntensiteMaxInitCoupe2 && Valeurdefinitif[k] < 2500)
            *IntensiteMaxInitCoupe2 = Valeurdefinitif[k];//Isolement de l'intensité max
    }

    QImage dest;//Init QImage a ajouter a la fenetre Qt
    k = 0;
    if (*IntensiteVariableCoupe2 == 0)//Si il n'y a pas de changement de l'intensité
        valMax = *IntensiteMaxInitCoupe2;//Val max conservée
    else
        valMax = *IntensiteVariableCoupe2; //Sinon modifiée

    for (int i = 0; i < l; i++)
        for (int j = 0; j < c; j++)
        {
            if (valMax==0)
                image.at<unsigned char>(i, j) = Valeurdefinitif[k]; //Association de la valeur au bon endroit de l'image
            else
                image.at<unsigned char>(i, j) = (Valeurdefinitif[k] * 255) / valMax; //Association de la valeur au bon endroit de l'image
            k++;
        }
    rotate(image, image, cv::ROTATE_90_CLOCKWISE);//Rotation de 90° de l'image
    switch (*NbCouleurs)
    {
    case 0:
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8); //Conversion d'un MAT en QImage
        break;
    case 1:
        applyColorMap(image, image, COLORMAP_JET);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 2:
        applyColorMap(image, image, COLORMAP_BONE);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 3:
        applyColorMap(image, image, COLORMAP_CIVIDIS);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 4:
        applyColorMap(image, image, COLORMAP_TURBO);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 5:
        applyColorMap(image, image, COLORMAP_HOT);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 6:
        applyColorMap(image, image, COLORMAP_PARULA);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 7:
        applyColorMap(image, image, COLORMAP_TWILIGHT_SHIFTED);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    }
    if (l < 400 && c < 400) //Si image de petite taille
    {
        l = 1.75 * l;//Coeff de 1.75
        c = 1.75 * c;//Coeff de 1.75
    }
    if (*Mode == 1 && *NbCouleurs == 0)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);

        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < SliderVisuTransparence->value())
                {
                    // set alpha to zero:
                    pixel[3] = 0;
                }
                else {
                    pixel[0] -= SliderVisuIntensite->value();
                    pixel[1] -= SliderVisuIntensite->value();
                    pixel[2] -= SliderVisuIntensite->value();
                    pixel[3] = 1;
                }
            }
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Images/Coupe2_" + numero + format;
        imwrite(cheminimage, input_bgra);
    }
    if (*Mode == 3 && *NbCouleurs == 0 && *CoupeVisu == 1)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);
        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < SliderVisuTransparence->value())
                {
                    // set alpha to zero:
                    pixel[0] = 255;
                    pixel[1] = 255;
                    pixel[2] = 255;
                    pixel[3] = 1;
                }
                else {
                    pixel[0] -= SliderVisuIntensite->value();
                    pixel[1] -= SliderVisuIntensite->value();
                    pixel[2] -= SliderVisuIntensite->value();
                    pixel[3] = 1;
                }
            }
        QImage visu = QImage((uchar*)input_bgra.data, input_bgra.cols, input_bgra.rows, input_bgra.step, QImage::Format_RGBX8888); //Conversion d'un MAT en QImage
        LabelVisuImage->setPixmap(QPixmap::fromImage(visu).scaled(QSize(l, c), Qt::IgnoreAspectRatio)); //Ajoute au layout
        LayoutVisuImage->addWidget(LabelVisuImage, 0, 0, 1, 3, Qt::AlignHCenter);//Ajout du layout à l'image
    }

    if (*Mode == 0)
    {
        //SpinBox2->setValue(slider2->value());

        imageLabel2->setPixmap(QPixmap::fromImage(dest).scaled(QSize(l, c), Qt::IgnoreAspectRatio)); //Ajoute au layout
        imageLabel2->setMaximumSize(l, c);
        layout->addWidget(imageLabel2, 1, 1, Qt::AlignHCenter);//Ajout du layout à l'image
    }

}
//------------------Coupe sagittale------------------------------
void Interface::ImageDICOM(int v)//Ouverture, lecture et affichage image "*.dcm"
{
    Mat image = Mat::zeros(*rows, *cols, CV_8UC1);//Image de la taille obtenue avec data
    int l = image.rows;
    int c = image.cols;
    int valMax = 0;
    *IntensiteMaxInitCoupe1 = 0;
    int k = 0;
    for (k = (*cols * *rows) * v; k < (*cols * *rows) * (v + 1); k++)
    {
        if ((*allpixels)[k] > * IntensiteMaxInitCoupe1 && (*allpixels)[k] < 6000)
            *IntensiteMaxInitCoupe1 = (*allpixels)[k];//Isolement de l'intensité max
    }
    QImage dest;//Init QImage a ajouter a la fenetre Qt
    k = (*cols * *rows) * v;
    if (*IntensiteVariableCoupe1 == 0)//Si il n'y a pas de changement de l'intensité
        valMax = *IntensiteMaxInitCoupe1;//Val max conservée
    else
        valMax = *IntensiteVariableCoupe1;//Sinon modifié

    for (int i = 0; i < l; i++)
        for (int j = 0; j < c; j++)
        {
            if (valMax==0)
                image.at<unsigned char>(i, j) = (*allpixels)[k]; //Association de la valeur au bon endroit de l'image
            else
                image.at<unsigned char>(i, j) = ((*allpixels)[k] * 255) / valMax; //Association de la valeur au bon endroit de l'image
            k++;
        }
    switch (*NbCouleurs)
    {
    case 0:
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8); //Conversion d'un MAT en QImage
        break;
    case 1:
        applyColorMap(image, image, COLORMAP_JET);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 2:
        applyColorMap(image, image, COLORMAP_BONE);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 3:
        applyColorMap(image, image, COLORMAP_CIVIDIS);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 4:
        applyColorMap(image, image, COLORMAP_TURBO);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 5:
        applyColorMap(image, image, COLORMAP_HOT);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 6:
        applyColorMap(image, image, COLORMAP_PARULA);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    case 7:
        applyColorMap(image, image, COLORMAP_TWILIGHT_SHIFTED);//Application de la couleur a l'image
        dest = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); //Conversion d'un MAT en QImage
        break;
    }
    if (l < 400 && c < 400) //Si image de petite taille
    {
        l = 1.75 * l;//Coeff de 1.75
        c = 1.75 * c;//Coeff de 1.75
    }
    if (*Mode == 1 && *NbCouleurs == 0)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);

        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < SliderVisuTransparence->value())
                {
                    // set alpha to zero:
                    pixel[3] = 0;
                }
                else {
                    pixel[0] -= SliderVisuIntensite->value();
                    pixel[1] -= SliderVisuIntensite->value();
                    pixel[2] -= SliderVisuIntensite->value();
                    pixel[3] = 1;
                }
            }
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Images/Coupe1_" + numero + format;
        imwrite(cheminimage, input_bgra);

    }
    if (*Mode == 3 && *NbCouleurs == 0 && *CoupeVisu == 0)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);
        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < SliderVisuTransparence->value())
                {
                    // set alpha to zero:
                    pixel[0] = 255;
                    pixel[1] = 255;
                    pixel[2] = 255;
                    pixel[3] = 1;
                }
                else {
                    pixel[0] -= SliderVisuIntensite->value();
                    pixel[1] -= SliderVisuIntensite->value();
                    pixel[2] -= SliderVisuIntensite->value();
                    pixel[3] = 1;
                }
            }
        QImage visu = QImage((uchar*)input_bgra.data, input_bgra.cols, input_bgra.rows, input_bgra.step, QImage::Format_RGBX8888); //Conversion d'un MAT en QImage
        LabelVisuImage->setPixmap(QPixmap::fromImage(visu).scaled(QSize(c, l), Qt::IgnoreAspectRatio)); //Ajoute au layout
        LayoutVisuImage->addWidget(LabelVisuImage, 0, 0, 1, 3, Qt::AlignHCenter);//Ajout du layout à l'image
    }
    if (*Mode == 0)
    {
        /*if (SpinBox1->value() != slider1->value()){
            slider1->setValue(SpinBox1->value());
        }
        else {
            SpinBox1->setValue(slider1->value());

        }*/

        imageLabel1->setPixmap(QPixmap::fromImage(dest).scaled(QSize(c, l), Qt::IgnoreAspectRatio)); //Ajoute au layout
        imageLabel1->setMaximumSize(c, l);
        layout->addWidget(imageLabel1, 1, 0, Qt::AlignHCenter);//Ajout du layout à l'image
    }

}

Interface::Interface() : QWidget() //Widget = fenetre principale
{
    _mkdir("Images");
    Visualisation3D = new My3DScene();
    //--------------interface visualisation
    imageMin = new qint16;
    imageMax = new qint16;
    CoupeVisu = new qint16;
    *CoupeVisu = 0;
    //------------------
    hWndMain = (HWND)this->winId();
    QTimer* timer = new QTimer(this);

    pathFolderSave = new QString();
    layout = new QGridLayout;//Init layout
    imageLabel1 = new QLabel(); //init label
    
    SpinBox1 = new QSpinBox();
    SpinBox2 = new QSpinBox();
    SpinBox3 = new QSpinBox();

    imageLabel2 = new QLabel();//init label
    imageLabel3 = new QLabel();//init label
    imageLabel4 = new QLabel(); //init label
    imageLabel5 = new QLabel();//init label
    imageLabel6 = new QLabel();//init label
    slider1 = new QSlider(Qt::Horizontal);//init curseur
    slider2 = new QSlider(Qt::Horizontal);//init curseur
    slider3 = new QSlider(Qt::Horizontal);//init curseur
    sliderIntensite = new QSlider(Qt::Horizontal);//init curseur
    NbFichiers = new qint16;//Init du nb de fichier
    visible = new qint16;//Init du nb de fichier
    menu = new QMenuBar();//Init Bar de menus

    file = new QMenu("&Fichiers");//init menu fichiers
    Info = new QMenu("&Informations");//init menu infos
    Affichage = new QMenu("&Affichage");//Init menu affichage
    Outils = new QMenu("&Outils");//Init menu outils



    lastTxValue = new qint16;
    NumImageTx = new qint16;
    lastTyValue = new qint16;
    NumImageTy = new qint16;
    lastTzValue = new qint16;
    NumImageTz = new qint16;
    lastRyValue = new qint16;
    variationIntensite = new qint16;
    coupe = new qint16;
    Mode = new qint16;

    cols = new qint16;//Colones en orientation init ici
    rows = new qint16;//lignes en orientation init ici
    souris3D = new qint16;
    Outils->addAction("Activer/Desactiver souris 3D", this, SLOT(UtiliserSouris3D()));
    Affichage->addAction("ORIGINAL", this, SLOT(AffichageOriginal()));//Action pour couleur
    Affichage->addAction("JET", this, SLOT(AffichageJet()));//Action pour couleur
    Affichage->addAction("BONE", this, SLOT(AffichageBone()));//Action pour couleur
    Affichage->addAction("CIVIDS", this, SLOT(AffichageCivids()));//Action pour couleur
    Affichage->addAction("TURBO", this, SLOT(AffichageTurbo()));//Action pour couleur
    Affichage->addAction("HOT", this, SLOT(AffichageHot()));//Action pour couleur
    Affichage->addAction("PARULA", this, SLOT(AffichageParula()));//Action pour couleur
    Affichage->addAction("TWILIGHT SHIFTED", this, SLOT(AffichageTwilightShifted()));//Action pour couleur
    Affichage->addAction("Modifier Intensite", this, SLOT(AfficherCurseurIntensite()));//Action d'affichage slider1
    Affichage->addAction("Passer en visualisation 3D", this, SLOT(AppercuVisualisation3D()));//Action d'affichage slider1
    Info->addAction("Informations patient", this, SLOT(displayTags()));//Connexion menu action
    file->addAction("Ouvrir", this, SLOT(ouvrirFichiers()));//Connexion menu action
    file->addAction("Supprimer", this, SLOT(Supprimer()));//Connexion menu action

    *NbFichiers = 0; //Initialise a 0 pour ne pas avoir de pb de memoires
    *visible = 0;//NumImageTx orientation pour savoir si affichage sliderIntensite
    *souris3D = 0;
    *Mode = 0;

    menu->addMenu(file);//Ajout menu a bar de menus
    menu->addMenu(Info);//Ajout menu a bar de menus
    menu->addMenu(Affichage);//Ajout menu a bar de menus
    menu->addMenu(Outils);//Ajout menu a bar de menus
    layout->setMenuBar(menu);//Ajout du menu au layout
    layout->setAlignment(Qt::AlignHCenter);
    setWindowState(windowState() | Qt::WindowMaximized);//Fenetre en plein ecran
    setLayout(layout);//Mise en place du layout
    setWindowTitle("Logiciel de navigation 3D dans les images IRM");//titre fenetre
    setWindowIcon(QIcon("icon.png"));//Mettre un Icon a la fenetre
    

   
    connect(sliderIntensite, SIGNAL(valueChanged(int)), this, SLOT(ChangementIntensite(int)));// Connexion du slider1 a fonction
    connect(slider1, SIGNAL(valueChanged(int)), this, SLOT(ActionSlider1(int)));// Connexion du slider1 a fonction
    connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(ActionSlider2(int)));// Connexion du slider1 a fonction
    connect(slider3, SIGNAL(valueChanged(int)), this, SLOT(ActionSlider3(int)));// Connexion du slider1 a fonction

    connect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(ActionSpin1(int)));// Connexion du slider1 a fonction
    connect(SpinBox2, SIGNAL(valueChanged(int)), this, SLOT(ActionSpin2(int)));// Connexion du slider1 a fonction
    connect(SpinBox3, SIGNAL(valueChanged(int)), this, SLOT(ActionSpin3(int)));// Connexion du slider1 a fonction

    connect(this, SIGNAL(clic(QMouseEvent*)), this, SLOT(MajClicCoupe1(QMouseEvent*)));
    connect(this, SIGNAL(clic(QMouseEvent*)), this, SLOT(MajClicCoupe2(QMouseEvent*)));
    connect(this, SIGNAL(clic(QMouseEvent*)), this, SLOT(MajClicCoupe3(QMouseEvent*)));
       
    connect(timer, SIGNAL(timeout()), this, SLOT(Action3DMouseTx()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Action3DMouseTy()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Action3DMouseTz()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Action3DMouseIntensite()));
    connect(timer, SIGNAL(timeout()), this, SLOT(DoubleClics()));


    timer->start(10);
    SaveAs(this->winId());

}


void Interface::mousePressEvent(QMouseEvent* e)//Definition du signal clic
{
    emit clic(e);
}
bool Interface::DoubleClics() {
    if ((clicD == 1) && (clicG == 1)) {
        
        return TRUE;
        //clicD = 0;
        //clicG = 0;
    }
    else
        return FALSE;
    
}

void Interface::SaveAs(WId winId) {
    if (DoubleClics() == TRUE) {
        
        QMessageBox cc;

        cc.setText("screenshot");
        cc.exec();
        //qApp->beep(); // Signal the screenshoot

    // Prise du screenshoot

        //QScreen* screen = qApp->primaryScreen();
    

        //QApplication::beep();

        //QPixmap pixmap = screen->grabWindow(winId,0,0,-1,-1);
        //QPixmap pixmap = QScreen::grabWindow(winId, 0, 0, -1, -1);


        /*QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            "",
            tr("JPEG (*.jpg *.jpeg);;PNG (*.png)")
            );*/

        QString filePath = "E:/Documents/Etudes_M1/Projet_M1/myscreen2.png";
        //pixmap.save(filePath);
        
    
    }
}
void Interface::closeEvent(QCloseEvent* event)
{
    Supprimer();
    event->accept();

}