#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_DICOM_3Dmouse.h"
#include <QApplication>
#include <QtWidgets>
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <QFileDialog>
#include "utils.h"

#include <QMenuBar>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "dcm/dicom_file.h"
#include "dcm/logger.h"
#include "dcm/util.h"



extern HWND         hWndMain;


extern INT pTx,pTz;


class Interface : public QWidget
{
	Q_OBJECT

public:
	Interface();
	void ImageDICOM(int v);//Ouverture, lecture et affichage image "*.dcm"
	void ImageDICOM2(int v);//Ouverture, lecture et affichage image "*.dcm"
	void ImageDICOM3(int v);//Ouverture, lecture et affichage image "*.dcm"

public slots:
	void ouvrirFichier(); //Ouvrir le dossier l'image en fonction du positionnement du curseur
	void changeAffichage();
	void changeAffichage1();
	void changeAffichage2();
	void changeAffichage3();
	void changeAffichage4();
	void changeAffichage5();
	void changeAffichage6();
	void changeAffichage7();
	void ChangerIntensite();
	void value(int k);//Récuperer la valeur du curseur lorsqu'il est déplacé
	void value2(int k);//Récuperer la valeur du curseur lorsqu'il est déplacé
	void value3(int k);//Récuperer la valeur du curseur lorsqu'il est déplacé
	void value4(int k);//Récuperer la valeur du curseur lorsqu'il est déplacé
	void displayTags();
	void valueMouse();
	void valueMouse2();

private:
	QLabel* imageLabel1;
	QLabel* imageLabel2;
	QLabel* imageLabel3;
	QString* FichierImage;
	QString* FichierImage2;
	QGridLayout* layout;
	QSlider* slider;
	QSlider* slider2;
	QSlider* slider3;
	QSlider* slider4;
	QMenu* Affichage;
	QMenuBar* menu;
	QMenu* file;
	QMenu* Info;
	QStringList* Listechemin;
	QVector<int>* allpixels;
	qint16* tailleImage;
	qint16* cols;
	qint16* rows;
	qint16* NbFichiers;
	qint16* NbCouleurs;
	qint16* ValMaxA;
	qint16* ValeurMaxA;
	qint16* ValMaxB;
	qint16* ValeurMaxB;
	qint16* ValMaxC;
	qint16* ValeurMaxC;
	qint16* visible;
	qint16* precValue;
	qint16* compteur;
	qint16* precValue2;
	qint16* compteur2;
};