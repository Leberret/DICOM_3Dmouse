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
#include "Scene3D.h"


extern HWND         hWndMain;
extern int Coupe, Min, Max;

extern INT pTx, pTy, pTz, pRx, pRy, pRz;
extern INT Intensite;
extern INT clicD, clicG;


class Interface : public QWidget
{
	Q_OBJECT

public:
	Interface();
	void ImageDICOM(int v);//Ouverture, lecture et affichage image "*.dcm"
	void ImageDICOM2(int v);//Ouverture, lecture et affichage image "*.dcm"
	void ImageDICOM3(int v);//Ouverture, lecture et affichage image "*.dcm"
	void mousePressEvent(QMouseEvent* e);
	void closeEvent(QCloseEvent* event);

signals:
	void clic(QMouseEvent* e);
public slots:
	void ouvrirFichier(); //Ouvrir le dossier l'image en fonction du positionnement du curseur
	bool DoubleClics();
	void SaveAs(WId winId);
	void UtiliserSouris3D();
	void changeAffichage();
	void changeAffichage1();
	void changeAffichage2();
	void changeAffichage3();
	void changeAffichage4();
	void changeAffichage5();
	void changeAffichage6();
	void changeAffichage7();
	void ChangerIntensite();
	void value(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void value2(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void value3(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void value4(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void valueSpin(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void valueSpin2(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void valueSpin3(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void displayTags();
	void valueMouse();
	void valueMouse2();
	void valueMouse3();
	void valueMouse_int();
	void affichetruc(QMouseEvent* e);
	void affichetruc2(QMouseEvent* e);
	void affichetruc3(QMouseEvent* e);
	void InfoCoupes();
	void Enregistre();

	// 3D
	void valueVisuMin(QString k);
	void valueVisuMax(QString k);
	void valueVisu1(int k);//R�cuperer la valeur du curseur lorsqu'il est d�plac�
	void SelectCoupes(int k);
	void Appercu();
	void QuitterSupprimer();

private:
	QLabel* imageLabel1;
	QLabel* imageLabel2;
	QLabel* imageLabel3;
	QLabel* imageLabel4;
	QLabel* imageLabel5;
	QLabel* imageLabel6;
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
	QMenu* Outils;
	QStringList* Listechemin;
	QVector<int>* allpixels;
	//--------------------------
	QSpinBox* SpinBox1;
	QSpinBox* SpinBox2;
	QSpinBox* SpinBox3;
	//-------------------------
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
	qint16* precValue3;
	qint16* compteur3;
	qint16* precValue4;
	qint16* compteur4;
	qint16* souris3D;
	qint16* coupe;
	qint16* enregistre;

	//-----------------interface appercu
	QWidget* bruh;
	QLabel* imageLabelVisu;
	QLabel* imageLabelVisuMin;
	QLabel* imageLabelVisuMax;
	QGridLayout* layoutVisu;
	QSlider* sliderVisuInt;
	QSlider* sliderVisuLim;
	QLineEdit* LineEditVisuMin;
	QLineEdit* LineEditVisuMax;
	qint16* imageMax;
	qint16* imageMin;
	qint16* CoupeVisu;
	QPushButton* validerVisu;
	QComboBox* comboBoxVisu;
	My3DScene* bla;
};