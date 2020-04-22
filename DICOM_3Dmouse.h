#pragma once

//Includes
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
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "dcm/dicom_file.h"
#include "dcm/logger.h"
#include "dcm/util.h"
#include "Scene3D.h"
#include <direct.h>

//Appel des variables globales externes
extern HWND         hWndMain;
extern INT			Coupe, Min, Max;
extern INT			pTx, pTy, pTz, pRx, pRy, pRz;
extern INT			Intensite, prevInt, OnOffSouris3D;
extern INT			clicD, clicG;

class Interface : public QWidget
{
	Q_OBJECT

public:
	Interface();
	void mousePressEvent(QMouseEvent* e);
	void closeEvent(QCloseEvent* event);

signals:
	void clic(QMouseEvent* e);

public slots:
	//------------------Interface 2D-------------------------
	void ouvrirFichiers();
	void GestionImages(int v);
	void GestionImagesLignes(int v);
	void GestionImagesColonnes(int v);
	void DoubleClics();
	void ClicGauche();
	void SaveAs();
	void UtiliserSouris3D();
	void AffichageOriginal();
	void AffichageJet();
	void AffichageBone();
	void AffichageCivids();
	void AffichageTurbo();
	void AffichageHot();
	void AffichageParula();
	void AffichageTwilightShifted();
	void UtiliserCurseurIntensite();
	void ChangementIntensite(int k);
	void ActionSlider1(int k);
	void ActionSlider2(int k);
	void ActionSlider3(int k);
	void ActionSpin1(int k);
	void ActionSpin2(int k);
	void ActionSpin3(int k);
	void displayTags();
	void Action3DMouseTx();
	void Action3DMouseTy();
	void Action3DMouseTz();
	void Action3DMouseIntensite();
	void MajClicCoupe1(QMouseEvent* e);
	void MajClicCoupe2(QMouseEvent* e);
	void MajClicCoupe3(QMouseEvent* e);
	void InfoCoupes();
	void Enregistre();

	//------------------Interface 3D-------------------------
	void FirstImage(QString k);
	void LastImage(QString k);
	void AfficheIntensiteTransparence();
	void SelectCoupes(int k);
	void AppercuVisualisation3D();
	void Supprimer();

private:
	//------------------Interface 2D-------------------------
	QLabel* imageLabel1;
	QLabel* imageLabel2;
	QLabel* imageLabel3;
	QLabel* imageLabel4;
	QLabel* imageLabel5;
	QLabel* imageLabel6;
	QString* pathFolder;
	QString* pathFolderSave;
	QGridLayout* layout;
	QSlider* slider1;
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
	QSpinBox* SpinBox1;
	QSpinBox* SpinBox2;
	QSpinBox* SpinBox3;
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
	qint16* MenuSouris3D;
	qint16* OnOffIntensite;
	qint16* MenuIntensite;
	qint16* coupe;
	qint16* Mode;
	qint16* IntensiteOnOff;

	//-----------------interface AppercuVisualisation3D--------------------------
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
};