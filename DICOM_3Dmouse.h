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
#include <direct.h>


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
	void Initialisation();
	void ouvrirFichiers(); //Ouvrir le dossier l'image en fonction du positionnement du curseur
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
	void AfficherCurseurIntensite();
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
	void MajClicCoupe1(QMouseEvent* e);
	void affichetruc2(QMouseEvent* e);
	void affichetruc3(QMouseEvent* e);
	void InfoCoupes();
	void Enregistre();

	// 3D
	void FirstImage(QString k);
	void LastImage(QString k);
	void AfficheIntensiteTransparence();
	void SelectCoupes(int k);
	void AppercuVisualisation3D();
	void Supprimer();

private:
	QLabel* imageLabel1;
	QLabel* imageLabel2;
	QLabel* imageLabel3;
	QLabel* imageLabel4;
	QLabel* imageLabel5;
	QLabel* imageLabel6;
	QString* pathFolder;
	QString* pathFolderSave;
	QGridLayout* layout;
	QSlider* slider;
	QSlider* slider2;
	QSlider* slider3;
	QSlider* sliderIntensite;
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
	qint16* cols;
	qint16* rows;
	qint16* NbFichiers;
	qint16* NbCouleurs;
	qint16* IntensiteMaxInitCoupe1;
	qint16* IntensiteVariableCoupe1;
	qint16* IntensiteMaxInitCoupe2;
	qint16* IntensiteVariableCoupe2;
	qint16* IntensiteMaxInitCoupe3;
	qint16* IntensiteVariableCoupe3;
	qint16* visible;
	qint16* lastTxValue;
	qint16* NumImageTx;
	qint16* lastTyValue;
	qint16* NumImageTy;
	qint16* lastTzValue;
	qint16* NumImageTz;
	qint16* lastRyValue;
	qint16* variationIntensite;
	qint16* souris3D;
	qint16* coupe;
	qint16* Mode;

	//-----------------interface AppercuVisualisation3D
	QWidget* WidgetAppercu3D;
	QLabel* LabelVisuImage;
	QLabel* LabelSaisieMin;
	QLabel* LabelSaisieMax;
	QLabel* LabelSliderIntensite;
	QLabel* LabelSliderTransparence;

	QGridLayout* LayoutVisuImage;
	QSlider* SliderVisuIntensite;
	QSlider* SliderVisuTransparence;
	QLineEdit* LineEditSaisieMin;
	QLineEdit* LineEditSaisieMax;
	qint16* imageMax;
	qint16* imageMin;
	qint16* CoupeVisu;
	QPushButton* validerVisu;
	QComboBox* comboBoxVisu;
	My3DScene* Visualisation3D;
};