#include "DICOM_3Dmouse.h"
#include "Scene3D.h"

QVector<int>* ALLPixels(vector<unsigned short>* pixels, QVector<int>* allpixels) //Stocker tous les pixels de chaque image dans un seul vecteur
{
    for (auto pixel2 : *pixels)
        allpixels->push_back(pixel2); //Remplissage du vecteur avec les valeurs des pixels
    return allpixels;
}
void Interface::passer3D()
{
    My3DScene *scene= new My3DScene;
    scene->show();
}
void Interface::Supprimer()
{
    int Coupe = QInputDialog::getInt(new Interface, "Saisir valeur", "Coupe");
    int Min = QInputDialog::getInt(new Interface, "Saisir valeur", "Image de d�part");
    int Max = QInputDialog::getInt(new Interface, "Saisir valeur", "Image Finale");
    *enregistre = 2;
    switch (Coupe)
    {
    case 1:
        for (Min; Min < Max; Min++)
            ImageDICOM(Min);
        break;
    case 2:
        for (Min; Min < Max; Min++)
            ImageDICOM2(Min);
        break;
    case 3:
        for (Min; Min < Max; Min++)
            ImageDICOM3(Min);
        break;
    }
    *enregistre = 0;
    ImageDICOM(slider->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());
}
void Interface::Enregistre()
{
    int Coupe = QInputDialog::getInt(new Interface, "Saisir valeur", "Coupe");
    int Min = QInputDialog::getInt(new Interface, "Saisir valeur", "Image de d�part");
    int Max = QInputDialog::getInt(new Interface, "Saisir valeur", "Image Finale");
    *enregistre = 1;
    *NbCouleurs = 0;
    switch (Coupe)
    {
    case 1:
        for (Min; Min < Max; Min++)
            ImageDICOM(Min);
        break;
    case 2:
        for (Min; Min < Max; Min++)
            ImageDICOM2(Min);
        break;
    case 3:
        for (Min; Min < Max; Min++)
            ImageDICOM3(Min);
        break;
    }
    *enregistre = 0;
    ImageDICOM(slider->value());
    ImageDICOM2(slider2->value());
    ImageDICOM3(slider3->value());

}
void Interface::InfoCoupes()
{
    if (*NbFichiers == 0)
        return;
    QStringList Listchemin = *Listechemin;
    *FichierImage2 = Listchemin[*NbFichiers / 2];
    QByteArray b = FichierImage2->toLocal8Bit(); //Convertir le QString* en const char * 
    const char* chemin = b.data();
    dcm::DicomFile* data;
    vector<unsigned short>* pixels;
    // Read file.
    data = readFile(chemin);//Lecture du fichier
    if (data == NULL)
        return;
    int nombre1, nombre2, nombre3, nombre4, nombre5, nombre6, nombre7;
    bool ok1, ok2, ok3, ok4, ok5, ok6, ok7;
    QString orientation1, orientation2, orientation3, orientation4, orientation5, orientation6, orientation7;
    string orientationGLOBAL = getStringTagValue(0x00200037, data);
    QString global = QString::fromStdString(orientationGLOBAL);
    QMessageBox cc;

    cc.setText(global);
    cc.exec();
    global.remove("0");
    global.remove(".");
    global.remove("-");
    orientation1 = global[0];
    nombre1 = orientation1.toInt(&ok1);
    orientation2 = global[1];
    nombre2 = orientation2.toInt(&ok2);
    orientation6 = global[5];
    nombre6 = orientation6.toInt(&ok6);
    orientation7 = global[6];
    nombre7 = orientation7.toInt(&ok7);

    
    if (nombre2 == 1 && nombre7 == 1)
    {
        *coupe = 1;
        SpinBox1->setPrefix("Coupe Sagittale : ");
        SpinBox2->setPrefix("Coupe Transversale : ");
        SpinBox3->setPrefix("Coupe Coronale : ");
    }

    if (nombre1 == 1 && nombre6 == 1)
    {
        *coupe = 2;
        SpinBox1->setPrefix("Coupe Transversale : ");
        SpinBox2->setPrefix("Coupe Coronale : ");
        SpinBox3->setPrefix("Coupe Sagittale : ");
    }

    if (nombre1 == 1 && nombre7 == 1)
    {
        *coupe = 3;
        SpinBox1->setPrefix("Coupe Coronale : ");
        SpinBox2->setPrefix("Coupe Transversale : ");
        SpinBox3->setPrefix("Coupe Sagittale : ");
    }


}
void Interface::displayTags() //Fonction ressortant les informations du patient dans une boite de dialogue
{
    if (*NbFichiers == 0)
        return;
    QByteArray b = FichierImage2->toLocal8Bit(); //Convertir le QString* en const char *
    const char* chemin = b.data();
    dcm::DicomFile data(chemin);
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    QString nom, genre, Date, type, photo, Datetude;
    if (!data.Load()) {
        msg.setText("ERREUR DE FICHIER");//Verfification d'existance du fichier
    }
    std::string Name;
    if (data.GetString(dcm::tags::kPatientName, &Name))
        nom = QString::fromStdString(Name); //Nom patient
    std::string DateEtude;
    if (data.GetString(dcm::tags::kStudyDate, &DateEtude))
        Datetude = QString::fromStdString(DateEtude); //Date etude patient
    std::string Sexe;
    if (data.GetString(dcm::tags::kPatientSex, &Sexe))
        genre = QString::fromStdString(Sexe); //Sexe patient
    std::string Birth;
    if (data.GetString(dcm::tags::kPatientBirthDate, &Birth))
        Date = QString::fromStdString(Birth);//date de naissance du patient
    std::string Type;
    if (data.GetString(dcm::tags::kImageType, &Type))
        type = QString::fromStdString(Type);//Type d'image
    std::string Photo;
    if (data.GetString(dcm::tags::kPhotometricInterpretation, &Photo))
        photo = QString::fromStdString(Photo);//Phototmetric
    nom.replace("^", " ");
    msg.setText("Nom du patient : " + nom + "\n" +
        "Sexe : " + genre + "\n" +
        "Date de naissance : " + Date[6] + Date[7] + "/" + Date[4] + Date[5] + "/" + Date[0] + Date[1] + Date[2] + Date[3] + "\n" +
        "Date de l'etude : " + Datetude[6] + Datetude[7] + "/" + Datetude[4] + Datetude[5] + "/" + Datetude[0] + Datetude[1] + Datetude[2] + Datetude[3] + "\n" +
        "Type d'images : " + type + "\n" +
        "Interpretation photometrique : " + photo + "\n"); //Ajout � la boite QMessageBox
    msg.setWindowTitle("Informations patient");
    msg.setWindowIcon(QIcon("patient.png"));
    msg.exec();//affichage boite de dialogue
}
void Interface::ouvrirFichier() //Ouvrir le dossier l'image en fonction du positionnement du curseur
{
    FichierImage = new QString();
    *FichierImage = QFileDialog::getExistingDirectory(this, "Explorateur de fichiers");//Boite de dialogue s�lection dossier
    if (FichierImage->isEmpty() || FichierImage->isNull())
        return;
    FichierImage2 = FichierImage;//Pour eviter de lire un dossier vide dans les infos patients en cas d'annulation de l'ouverture.
    ValMaxA = new qint16;//Init du nb de fichier
    ValeurMaxA = new qint16;//Init du nb de fichier
    ValMaxB = new qint16;//Init du nb de fichier
    ValeurMaxB = new qint16;//Init du nb de fichier
    ValMaxC = new qint16;//Init du nb de fichier
    ValeurMaxC = new qint16;//Init du nb de fichier
    *NbCouleurs = 0;//variable globale de l'indice de colormap
    *ValeurMaxA = 0;//Variable globale de intensite max
    *ValeurMaxB = 0;//Variable globale de intensite max
    *ValeurMaxC = 0;//Variable globale de intensite max
    Listechemin = new QStringList();//init list QString
    allpixels = new QVector<int>; //vecteur pour tous les pixels
    tailleImage = new qint16;//Taille image en globale init ici

    
    QDirIterator direction(*FichierImage, QDir::Files | QDir::NoSymLinks);// Obtenir l'arborescence des fichiers
    while (direction.hasNext()) //tant qu'il reste des dossier dans le fichier
        *Listechemin << direction.next(); //Ajout de tous les chemins dans une liste

    QStringList Listchemin = *Listechemin;
    *NbFichiers = Listchemin.length();//Nombre total de fichiers dans le dossier

    for (int NbFichier = 0; NbFichier < *NbFichiers; NbFichier++) //pour tous les fichiers du dossier
    {
        *FichierImage = Listchemin[NbFichier]; //selection du chemin selon la boucle
        QByteArray b = FichierImage->toLocal8Bit(); //Convertir le QString* en const char * 
        const char* chemin = b.data();
        dcm::DicomFile* data;
        vector<unsigned short>* pixels;
        // Read file.
        data = readFile(chemin);//Lecture du fichier
        if (data == NULL)
            return;
        *rows = getUShortTagValue(0x00280010, data);//Rows
        *cols = getUShortTagValue(0x00280011, data);//Cols
        pixels = readPixels(data); //Lecture des pixesls
        allpixels = ALLPixels(pixels, allpixels); //Tous les pixels stock�s dans un vecteur
        delete(data);
        delete(pixels);

    }
    SpinBox1->setButtonSymbols(QSpinBox::NoButtons);
    SpinBox2->setButtonSymbols(QSpinBox::NoButtons);
    SpinBox3->setButtonSymbols(QSpinBox::NoButtons);
    SpinBox1->setRange(0, *NbFichiers - 1);
    SpinBox2->setRange(0, *rows - 1);
    SpinBox3->setRange(0, *cols - 1);


    SpinBox1->setStyleSheet("QSpinBox { border: 0px solid grey; border-radius: 4px; background-color: rgb(230,230,230); color: black }");
    SpinBox2->setStyleSheet("QSpinBox { border: 0px solid grey; border-radius: 4px; background-color: rgb(230,230,230); color: black }");
    SpinBox3->setStyleSheet("QSpinBox { border: 0px solid grey; border-radius: 4px; background-color: rgb(230,230,230); color: black }");

    layout->addWidget(SpinBox1, 0, 0, Qt::AlignCenter);
    layout->addWidget(SpinBox2, 0, 1, Qt::AlignCenter);
    layout->addWidget(SpinBox3, 0, 2, Qt::AlignCenter);
    slider->setRange(0, *NbFichiers - 1);
    slider2->setRange(1, *rows - 1); //Valeurs du slider selon nb de fichiers
    slider3->setRange(1, *cols); //Valeurs du slider selon nb de fichiers

    *precValue = 0;
    *compteur = *NbFichiers /2;
    *precValue2 = 0;
    *compteur2 = *rows/2;
    *precValue3 = 0;
    *compteur3 = *cols/2;
    *precValue4 = 0;
    *compteur4 = 0;

    slider->setValue(*NbFichiers / 2);//Positionnement du cuseur a la moiti�
    slider2->setValue(*rows / 2);//Positionnement du cuseur a la moiti�
    slider3->setValue(*cols / 2);//Positionnement du cuseur a la moiti�

    InfoCoupes();

    ImageDICOM(slider->value()); //Affiche image selon curseur
    ImageDICOM2(slider2->value());//Affiche image selon curseur
    ImageDICOM3(slider3->value());//Affiche image selon curseur

    layout->removeWidget(slider4);//Pas de widgets intensit�
    slider4->setVisible(false);//Non visible
    layout->addWidget(slider, 2, 0,Qt::AlignBottom);//Position
    layout->addWidget(slider2, 2, 1, Qt::AlignBottom);//Position
    layout->addWidget(slider3, 2, 2, Qt::AlignBottom);//Position
    //layout->addWidget(imageLabel4, 0, 0, Qt::AlignHCenter);//Labels vides pour centrer
    //layout->addWidget(imageLabel5, 0, 1, Qt::AlignHCenter);//Labels vides pour centrer
    //layout->addWidget(imageLabel6, 0, 2, Qt::AlignHCenter);//Labels vides pour centrer

    *souris3D = 0;


}
/*
void Interface::SaveAs() {
    
}*/
void Interface::ChangerIntensite()
{
    if (*NbFichiers == 0) //S'il n'y a pas de fichier on se casse
        return;
    slider4->setRange(-1500, 3000); //Nuances d'intensit�
    slider4->setValue(0);//Init a 0 -> valMax r�elle
    slider4->setVisible(true);//rendre visible le curseur
    layout->addWidget(slider4, 3, 0, 1, 3);//Position
}

void Interface::UtiliserSouris3D() {
    if (*NbFichiers == 0)
        return;
    int value = *souris3D;
    if (value == 0)
        *souris3D = 1;
    
    else
        *souris3D = 0;
}
void Interface::affichetruc(QMouseEvent* e)
{
    if (*NbFichiers == 0)
        return;
    QPoint bla = e->pos();
    int posi_x = bla.x();
    int posi_y = bla.y();
    QPoint coord = imageLabel1->pos();
    int label_x = coord.x();
    int label_y = coord.y();
    int tailleLimite_X;
    int tailleLimite_Y;

    int nouveau;
    int nouveau2;

    if (e->button() == Qt::LeftButton) {
        if (*rows < 400 && *cols < 400) //Si image de petite taille
        {
            tailleLimite_X = (label_x + *cols * 1.75);
            tailleLimite_Y = (label_y + *rows * 1.75);
            nouveau = (posi_x - label_x) / 1.75;
            nouveau2 = (posi_y - label_y) / 1.75;
        }
        else {
            tailleLimite_X = (label_x + *cols);
            tailleLimite_Y = (label_y + *rows);
            nouveau = (posi_x - label_x);
            nouveau2 = (posi_y - label_y);
        }

        if (posi_x > label_x && posi_x< tailleLimite_X && posi_y>label_y && posi_y < tailleLimite_Y) {

            if (*souris3D == 0) {
                ImageDICOM3(nouveau);
                ImageDICOM2(nouveau2);
                slider3->setValue(nouveau);
                slider2->setValue(nouveau2);
            }
            else if (*souris3D == 1) {
                *compteur2 = nouveau2;
                *compteur3 = nouveau;
                valueMouse2();
                valueMouse3();
            }
            
        }

    }

}
void Interface::affichetruc2(QMouseEvent* e)
{
    if (*NbFichiers == 0)
        return;
    QPoint bla = e->pos();
    int posi_x = bla.x();
    int posi_y = bla.y();
    QPoint coord = imageLabel2->pos();
    int label_x = coord.x();
    int label_y = coord.y();
    int tailleLimite_X;
    int tailleLimite_Y;

    int nouveau;
    int nouveau2;

    if (e->button() == Qt::LeftButton) {
        if (*rows < 400 && *cols < 400) //Si image de petite taille
        {
            tailleLimite_X = (label_x + *NbFichiers * 1.75);
            tailleLimite_Y = (label_y + *rows * 1.75);
            nouveau = (posi_x - label_x) / 1.75;
            nouveau2 = (posi_y - label_y) / 1.75;
        }
        else {
            tailleLimite_X = (label_x + *NbFichiers);
            tailleLimite_Y = (label_y + *rows);
            nouveau = (posi_x - label_x);
            nouveau2 = (posi_y - label_y);
        }
        if (posi_x > label_x && posi_x< tailleLimite_X && posi_y>label_y && posi_y < tailleLimite_Y) {
            if (*souris3D == 0) {
                ImageDICOM3(nouveau2);
                ImageDICOM(*NbFichiers - nouveau);
                slider->setValue(*NbFichiers - nouveau);
                slider3->setValue(nouveau2);
            }
            else if (*souris3D == 1) {
                *compteur = *NbFichiers - nouveau;
                *compteur3 = nouveau2;
                valueMouse();
                valueMouse3();
            }

        }

    }

}
void Interface::affichetruc3(QMouseEvent* e)
{
    if (*NbFichiers == 0)
        return;
    QPoint bla = e->pos();
    int posi_x = bla.x();
    int posi_y = bla.y();
    QPoint coord = imageLabel3->pos();
    int label_x = coord.x();
    int label_y = coord.y();
    int tailleLimite_X;
    int tailleLimite_Y;

    int nouveau;
    int nouveau2;

    if (e->button() == Qt::LeftButton) {
        if (*rows < 400 && *cols < 400) //Si image de petite taille
        {
            tailleLimite_X = (label_x + *NbFichiers * 1.75);
            tailleLimite_Y = (label_y + *rows * 1.75);
            nouveau = (posi_x - label_x) / 1.75;
            nouveau2 = (posi_y - label_y) / 1.75;
        }
        else {
            tailleLimite_X = (label_x + *NbFichiers);
            tailleLimite_Y = (label_y + *rows);
            nouveau = (posi_x - label_x);
            nouveau2 = (posi_y - label_y);
        }
        if (posi_x > label_x && posi_x< tailleLimite_X && posi_y>label_y && posi_y < tailleLimite_Y) {
            if (*souris3D == 0) {
                ImageDICOM(*NbFichiers - nouveau);
                ImageDICOM2(nouveau2);
                slider->setValue(*NbFichiers - nouveau);
                slider2->setValue(nouveau2);
            }
            else if (*souris3D == 1) {
                *compteur = *NbFichiers - nouveau;
                *compteur2 = nouveau2;
                valueMouse();
                valueMouse2();
            }
        }

    }

}

void Interface::changeAffichage() //Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0) //S'il n'y a pas de fichier on se casse
        return;
    if (*visible % 2 == 0) //Une fois sur deux on affiche
    {
        layout->removeWidget(slider4);
        slider4->setVisible(false);
    }
    slider4->setValue(0);
    *ValeurMaxA, * ValeurMaxB, * ValeurMaxC = 0;
    *NbCouleurs = 0; //valeur de condition couleur
    ImageDICOM(slider->value()); //Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
    *visible += 1;
}
void Interface::changeAffichage1() //Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0)//S'il n'y a pas de fichier on se casse
        return;
    *NbCouleurs = 1;//valeur de condition couleur
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::changeAffichage2()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0)//S'il n'y a pas de fichier on se casse
        return;
    *NbCouleurs = 2;//valeur de condition couleur
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::changeAffichage3()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0)//S'il n'y a pas de fichier on se casse
        return;
    *NbCouleurs = 3;//valeur de condition couleur
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::changeAffichage4()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0)//S'il n'y a pas de fichier on se casse
        return;
    *NbCouleurs = 4;//valeur de condition couleur
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::changeAffichage5()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0)//S'il n'y a pas de fichier on se casse
        return;
    *NbCouleurs = 5;//valeur de condition couleur
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::changeAffichage6()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0)//S'il n'y a pas de fichier on se casse
        return;
    *NbCouleurs = 6;//valeur de condition couleur
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::changeAffichage7()//Affectation de la valeur correspondant a la couleur
{
    if (*NbFichiers == 0)//S'il n'y a pas de fichier on se casse
        return;
    *NbCouleurs = 7;//valeur de condition couleur
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}

void Interface::value(int v) //R�cuperer la valeur du curseur lorsqu'il est d�plac�
{
    
    ImageDICOM(v); //Appel de la fonction ImageDICOM pour afficher une image dicom
    SpinBox1->setValue(slider->value());
}
void Interface::value2(int v) //R�cuperer la valeur du curseur lorsqu'il est d�plac�
{
    ImageDICOM2(v);
    SpinBox2->setValue(slider2->value());
}
void Interface::value3(int v) //R�cuperer la valeur du curseur lorsqu'il est d�plac�
{
    ImageDICOM3(v);
    SpinBox3->setValue(slider3->value());
}
void Interface::value4(int v) //R�cuperer la valeur du curseur lorsqu'il est d�plac�
{
    *ValeurMaxA = *ValMaxA + v;//changement de la valeur max d'intensit� de r�f�rence
    *ValeurMaxB = *ValMaxB + v;//changement de la valeur max d'intensit� de r�f�rence
    *ValeurMaxC = *ValMaxC + v;//changement de la valeur max d'intensit� de r�f�rence
    ImageDICOM(slider->value());//Affichage de l'image
    ImageDICOM2(slider2->value());//Affichage de l'image
    ImageDICOM3(slider3->value());//Affichage de l'image
}
void Interface::valueSpin(int v) //R�cuperer la valeur du curseur lorsqu'il est d�plac�
{
    ImageDICOM(v); //Appel de la fonction ImageDICOM pour afficher une image dicom
    slider->setValue(SpinBox1->value());
}
void Interface::valueSpin2(int v) //R�cuperer la valeur du curseur lorsqu'il est d�plac�
{
    ImageDICOM2(v); //Appel de la fonction ImageDICOM pour afficher une image dicom
    slider2->setValue(SpinBox2->value());
}
void Interface::valueSpin3(int v) //R�cuperer la valeur du curseur lorsqu'il est d�plac�
{
    ImageDICOM3(v); //Appel de la fonction ImageDICOM pour afficher une image dicom
    slider3->setValue(SpinBox3->value());
}
void Interface::valueMouse() {

    
    int value = *souris3D;
    if (value == 0)
        return;
    int NbMaxImages;
    switch (*coupe)
    {
    case 1:
        NbMaxImages = *NbFichiers;
        break;
    case 2:
        NbMaxImages = *cols;
        break;
    case 3:
        NbMaxImages = *cols;
        break;
    }
    int i = *compteur;
    if ((i > 0) && (i < NbMaxImages)) {
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
            if ((pTx > 5) &&(pTx >= * precValue) && (pTx<20)){
                i=i-1;
            }
            else if ((pTx >= 20) && (pTx >= * precValue) && (pTx < 300)) {
                i = i-3;
            }
           
            else if ((pTx < -5) && (pTx <= * precValue) && (pTx > -20)) {
                i=i+1;
            }
            else if ((pTx <= -20) && (pTx <= *precValue) && (pTx > -300)) {
                i = i+3;
            }
    }
    
    else if(i <= 0 ){
        i = 1;
    }
    else if (i >= *NbFichiers) {
        i = *NbFichiers - 1;
    }
    else if (pTx >= 0 && pTx<50) {

        return;
    }
    switch (*coupe)
    {
    case 1:
        slider->setValue(i);
        break;
    case 2:
        slider3->setValue(i);
        break;
    case 3:
        slider3->setValue(i);
        break;
    }
    *compteur = i;
    *precValue = pTx;
}
void Interface::valueMouse2() {

    
    int value = *souris3D;
    if (value == 0)
        return;
    int NbMaxImages;
    switch (*coupe)
    {
    case 1:
        NbMaxImages=*rows;
        break;
    case 2:
        NbMaxImages = *NbFichiers;
        break;
    case 3:
        NbMaxImages = *rows;
        break;
    }
    int i = *compteur2;
    if ((i > 0) && (i < NbMaxImages)) {
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
        if ((pTy > 5) && (pTy >= * precValue2) && (pTy < 20)) {
            i = i - 1;
        }
        else if ((pTy >= 20) && (pTy >= * precValue2) && (pTy < 150)) {
            i = i - 2;
        }
        
        else if ((pTy < -5) && (pTy <= *precValue2) && (pTy > -20)) {
            i = i + 1;
        }
        else if ((pTy <= -20) && (pTy <= *precValue2) && (pTy > -150)) {
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
    switch (*coupe)
    {
    case 1:
        slider2->setValue(i);
        break;
    case 2:
        slider->setValue(i);
        break;
    case 3:
        slider2->setValue(i);
        break;
    }
    *compteur2 = i;
    *precValue2 = pTy;
}
void Interface::valueMouse3() {

    
    int value = *souris3D;
    if (value == 0)
        return;
    int NbMaxImages;
    switch (*coupe)
    {
    case 1:
        NbMaxImages = *cols;
        break;
    case 2:
        NbMaxImages = *rows;
        break;
    case 3:
        NbMaxImages = *NbFichiers;
        break;
    }
    int i = *compteur3;
    if ((i > 0) && (i < NbMaxImages)) {
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
        if ((pTz > 5) && (pTz >= * precValue3) && (pTz <20)) {
            i = i - 1;
        }
        else if ((pTz >= 20) && (pTz >= * precValue3) && (pTz < 250)) {
            i = i - 2;
        }
        
        else if ((pTz < -5) && (pTz <= *precValue3) && (pTz > -20)) {
            i = i + 1;
        }
        else if ((pTz <= -20) && (pTz <= *precValue3) && (pTz > -250)) {
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
    switch (*coupe)
    {
    case 1:
        slider3->setValue(i);
        break;
    case 2:
        slider2->setValue(i);
        break;
    case 3:
        slider->setValue(i);
        break;
    }    
    *compteur3 = i;
    *precValue3 = pTz;
}
void Interface::valueMouse_int() {
    int value = *souris3D;
    int inte = Intensite;
    if ((value == 0)||(inte==0))
        return;
    int v = pRy;
    int i = *compteur4;
    int lim = 500;
    if ((i > -lim) && (i < lim)) {
        *ValeurMaxA = *ValMaxA + i;//changement de la valeur max d'intensit� de r�f�rence
        *ValeurMaxB = *ValMaxB + i;//changement de la valeur max d'intensit� de r�f�rence
        *ValeurMaxC = *ValMaxB + i;//changement de la valeur max d'intensit� de r�f�rence

        valueMouse();
        valueMouse2();
        valueMouse3();
        if ((v > 5) && (v >= *precValue4)&&(v<lim)) {
            i=i+50;
        }

        else if ((v < -5) && (v <= *precValue4) && (v> -lim)) {
            i = i-50;
        }
    }
    else if (i <= -lim) {
        i = 1-lim ;
    }
    else if (i >= lim) {
        i = lim-1;
    }
    slider4->setValue(i);

    * compteur4 = i;
    *precValue4 = v;
}

//----------Coupe coronale------------------
void Interface::ImageDICOM3(int v)
{
    Mat image = Mat::zeros(*NbFichiers - 1, *rows, CV_8UC1);//Image de la taille obtenue avec data
    int l = image.rows;
    int c = image.cols;
    *ValMaxC = 0;//globale
    int valMax = 0;//Locale
    int k = 0;
    vector<int>Valeurdefinitif2; //Nouveau vecteur contenant les pixels de l'image
    for (int nb = v; nb < (*allpixels).size(); nb += *cols)//Condition pour avoir l'image selon la bonne coupe 
    {
        Valeurdefinitif2.push_back((*allpixels)[nb]);//Vecteur definitif avec valeurs de l'image a afficher (tel colonnes de chaque image)
    }
    for (k = 0; k < Valeurdefinitif2.size(); k++) //Boucle pour avoir val max intensit�
    {
        if (Valeurdefinitif2[k] > * ValMaxC && Valeurdefinitif2[k] < 2500)
            *ValMaxC = Valeurdefinitif2[k];//Isolement de l'intensit� max
    }

    QImage dest; //Init QImage a ajouter a la fenetre Qt
    k = 0;
    if (*ValeurMaxC == 0)//Si il n'y a pas de changement de l'intensit�
        valMax = *ValMaxC; //Val max conserv�e
    else
        valMax = *ValeurMaxC;//Sinon modifi�e
    for (int i = 0; i < l; i++)
        for (int j = 0; j < c; j++)
        {
            image.at<unsigned char>(i, j) = (Valeurdefinitif2[k] * 255) / valMax; //Association de la valeur au bon endroit de l'image
            k++;
        }
    rotate(image, image, cv::ROTATE_90_CLOCKWISE); //Rotation de 90� de l'image
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
    if (*enregistre == 1 && *NbCouleurs == 0)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);

        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < 15)
                {
                    // set alpha to zero:
                    pixel[3] = 0;
                }
                else {
                    pixel[0] -= 50;
                    pixel[1] -= 50;
                    pixel[2] -= 50;
                    pixel[3] = 1;
                }
            }
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Coupe3_" + numero + format;
        imwrite(cheminimage, input_bgra);
    }
    if (*enregistre == 2)
    {
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Coupe3_" + numero + format;
        remove(cheminimage.c_str());
    }
    imageLabel3->setPixmap(QPixmap::fromImage(dest).scaled(QSize(l, c), Qt::IgnoreAspectRatio)); //Ajoute au layout
    layout->addWidget(imageLabel3, 1, 2, Qt::AlignHCenter);//Ajout du layout � l'image
}
//-------------------Coupe Axiale------------------------
void Interface::ImageDICOM2(int v)
{

    Mat image = Mat::zeros(*NbFichiers - 1, *cols, CV_8UC1);//Image de la taille obtenue avec data
    int l = image.rows;
    int c = image.cols;
    *ValMaxB = 0;//globale
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
        if (Valeurdefinitif[k] > * ValMaxB && Valeurdefinitif[k] < 2500)
            *ValMaxB = Valeurdefinitif[k];//Isolement de l'intensit� max
    }

    QImage dest;//Init QImage a ajouter a la fenetre Qt
    k = 0;
    if (*ValeurMaxB == 0)//Si il n'y a pas de changement de l'intensit�
        valMax = *ValMaxB;//Val max conserv�e
    else
        valMax = *ValeurMaxB; //Sinon modifi�e

    for (int i = 0; i < l; i++)
        for (int j = 0; j < c; j++)
        {
            image.at<unsigned char>(i, j) = (Valeurdefinitif[k] * 255) / valMax; //Association de la valeur au bon endroit de l'image
            k++;
        }
    rotate(image, image, cv::ROTATE_90_CLOCKWISE);//Rotation de 90� de l'image
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
    if (*enregistre == 1 && *NbCouleurs == 0)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);

        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < 15)
                {
                    // set alpha to zero:
                    pixel[3] = 0;
                }
                else {
                    pixel[0] -= 50;
                    pixel[1] -= 50;
                    pixel[2] -= 50;
                    pixel[3] = 1;
                }
            }
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Coupe2_" + numero + format;
        imwrite(cheminimage, input_bgra);
    }
    if (*enregistre == 2)
    {
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Coupe2_" + numero + format;
        remove(cheminimage.c_str());
    }
    imageLabel2->setPixmap(QPixmap::fromImage(dest).scaled(QSize(l, c), Qt::IgnoreAspectRatio)); //Ajoute au layout
    layout->addWidget(imageLabel2, 1, 1, Qt::AlignHCenter);//Ajout du layout � l'image

}
//------------------Coupe sagittale------------------------------
void Interface::ImageDICOM(int v)//Ouverture, lecture et affichage image "*.dcm"
{
    Mat image = Mat::zeros(*rows, *cols, CV_8UC1);//Image de la taille obtenue avec data
    int l = image.rows;
    int c = image.cols;
    int valMax = 0;
    *ValMaxA = 0;
    int k = 0;
    for (k = (*cols * *rows) * v; k < (*cols * *rows) * (v + 1); k++)
    {
        if ((*allpixels)[k] > * ValMaxA && (*allpixels)[k] < 6000)
            *ValMaxA = (*allpixels)[k];//Isolement de l'intensit� max
    }
    QImage dest;//Init QImage a ajouter a la fenetre Qt
    k = (*cols * *rows) * v;
    if (*ValeurMaxA == 0)//Si il n'y a pas de changement de l'intensit�
        valMax = *ValMaxA;//Val max conserv�e
    else
        valMax = *ValeurMaxA;//Sinon modifi�

    for (int i = 0; i < l; i++)
        for (int j = 0; j < c; j++)
        {
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
    if (*enregistre == 1 && *NbCouleurs == 0)
    {
        Mat input_bgra;
        cvtColor(image, input_bgra, COLOR_GRAY2BGRA);

        for (int y = 0; y < input_bgra.rows; ++y)
            for (int x = 0; x < input_bgra.cols; ++x)
            {
                cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
                // if pixel is white
                if (image.at<unsigned char>(y, x) < 15)
                {
                    // set alpha to zero:
                    pixel[3] = 0;
                }
                else {
                    pixel[0] -= 50;
                    pixel[1] -= 50;
                    pixel[2] -= 50;
                    pixel[3] = 1;
                }
            }
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Coupe1_" + numero + format;
        imwrite(cheminimage, input_bgra);
    }

    if (*enregistre == 2)
    {
        string cheminimage;
        string format = ".PNG";
        string numero = to_string(v);
        cheminimage = "Coupe1_" + numero + format;
        remove(cheminimage.c_str());
    }
    imageLabel1->setPixmap(QPixmap::fromImage(dest).scaled(QSize(c, l), Qt::IgnoreAspectRatio)); //Ajoute au layout
    imageLabel1->setMaximumSize(c, l);//Taille du QLabel max = taille de l'image
    layout->addWidget(imageLabel1, 1, 0, Qt::AlignHCenter);//Ajout du layout � l'image

}

Interface::Interface() : QWidget() //Widget = fenetre principale
{
    

    hWndMain = (HWND)this->winId();
    QTimer* timer = new QTimer(this);

    FichierImage2 = new QString();
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
    slider = new QSlider(Qt::Horizontal);//init curseur
    slider2 = new QSlider(Qt::Horizontal);//init curseur
    slider3 = new QSlider(Qt::Horizontal);//init curseur
    slider4 = new QSlider(Qt::Horizontal);//init curseur
    NbCouleurs = new qint16;//Init de la variable pour gerer le colormap
    NbFichiers = new qint16;//Init du nb de fichier
    visible = new qint16;//Init du nb de fichier
    menu = new QMenuBar();//Init Bar de menus

    file = new QMenu("&Fichiers");//init menu fichiers
    Info = new QMenu("&Informations");//init menu infos
    Affichage = new QMenu("&Affichage");//Init menu affichage
    Outils = new QMenu("&Outils");//Init menu outils



    precValue = new qint16;
    compteur = new qint16;
    precValue2 = new qint16;
    compteur2 = new qint16;
    precValue3 = new qint16;
    compteur3 = new qint16;
    precValue4 = new qint16;
    compteur4 = new qint16;
    coupe = new qint16;
    enregistre = new qint16;

    cols = new qint16;//Colones en global init ici
    rows = new qint16;//lignes en global init ici
    souris3D = new qint16;
    Outils->addAction("Activer/Desactiver souris 3D", this, SLOT(UtiliserSouris3D()));
    Affichage->addAction("ORIGINAL", this, SLOT(changeAffichage()));//Action pour couleur
    Affichage->addAction("JET", this, SLOT(changeAffichage1()));//Action pour couleur
    Affichage->addAction("BONE", this, SLOT(changeAffichage2()));//Action pour couleur
    Affichage->addAction("CIVIDS", this, SLOT(changeAffichage3()));//Action pour couleur
    Affichage->addAction("TURBO", this, SLOT(changeAffichage4()));//Action pour couleur
    Affichage->addAction("HOT", this, SLOT(changeAffichage5()));//Action pour couleur
    Affichage->addAction("PARULA", this, SLOT(changeAffichage6()));//Action pour couleur
    Affichage->addAction("TWILIGHT SHIFTED", this, SLOT(changeAffichage7()));//Action pour couleur
    Affichage->addAction("Modifier Intensite", this, SLOT(ChangerIntensite()));//Action d'affichage slider
    Affichage->addAction("Passer en visualisation 3D", this, SLOT(passer3D()));//Action d'affichage slider
    Info->addAction("Informations patient", this, SLOT(displayTags()));//Connexion menu action
    file->addAction("Ouvrir", this, SLOT(ouvrirFichier()));//Connexion menu action
    file->addAction("Enregistrer images", this, SLOT(Enregistre()));//Connexion menu action
    file->addAction("Supprimer images", this, SLOT(Supprimer()));//Connexion menu action
    //file->addAction("Enregistrer sous", this, SLOT(SaveAs(this->winId(),imageLabel1)));//Connexion menu action

    *NbFichiers = 0; //Initialise a 0 pour ne pas avoir de pb de memoires
    *visible = 0;//compteur global pour savoir si affichage slider4
    *souris3D = 0;
    *enregistre = 0;

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
    

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(value(int)));// Connexion du slider a fonction
    connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(value2(int)));// Connexion du slider a fonction
    connect(slider3, SIGNAL(valueChanged(int)), this, SLOT(value3(int)));// Connexion du slider a fonction
    connect(slider4, SIGNAL(valueChanged(int)), this, SLOT(value4(int)));// Connexion du slider a fonction
   
    connect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(valueSpin(int)));// Connexion du slider a fonction
    connect(SpinBox2, SIGNAL(valueChanged(int)), this, SLOT(valueSpin2(int)));// Connexion du slider a fonction
    connect(SpinBox3, SIGNAL(valueChanged(int)), this, SLOT(valueSpin3(int)));// Connexion du slider a fonction

    connect(this, SIGNAL(clic(QMouseEvent*)), this, SLOT(affichetruc(QMouseEvent*)));
    connect(this, SIGNAL(clic(QMouseEvent*)), this, SLOT(affichetruc2(QMouseEvent*)));
    connect(this, SIGNAL(clic(QMouseEvent*)), this, SLOT(affichetruc3(QMouseEvent*)));
       
    connect(timer, SIGNAL(timeout()), this, SLOT(valueMouse()));
    connect(timer, SIGNAL(timeout()), this, SLOT(valueMouse2()));
    connect(timer, SIGNAL(timeout()), this, SLOT(valueMouse3()));
    connect(timer, SIGNAL(timeout()), this, SLOT(valueMouse_int()));
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
