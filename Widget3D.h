#pragma once
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

class Widget3D : public QWidget
{
	Q_OBJECT

public:
	Widget3D();
	My3DScene* Visualisation3D;
	void closeEvent(QCloseEvent* event);
signals:

public slots:
	void centrage();
	void Actu3D();
	void SaveAs3D();
	void DoubleClics();
public:
	QGridLayout* Layout;
	QWidget* container;
	QMenuBar* menu;
	QMenu* Fichier;
	QMenu* Affichage;
	QTimer* timer;
};