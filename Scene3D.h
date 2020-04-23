#pragma once

// Includes
#include <iostream>
#include <QGuiApplication>
#include <QEntity>
#include <QCamera>
#include <QTransform>
#include <QPlaneMesh>
#include <QTexture>
#include <QTextureMaterial>
#include <QPointLight>
#include <QPhongMaterial>
#include <QtWidgets>
#include "qt3dwindow.h"
#include "qorbitcameracontroller.h"


using namespace std;

//Variables globales externes
extern INT			Coupe, Min, Max;
extern INT			pTx, pTy, pTz, pRx, pRy, pRz;
//extern HWND			hWndMain;


//Class My3DScene.
class My3DScene : public Qt3DExtras::Qt3DWindow
{
private:
	Qt3DCore::QEntity scene;
	Qt3DCore::QEntity* planeEntity;
	Qt3DExtras::QPlaneMesh* planeMesh;
	Qt3DExtras::QPhongMaterial* planeMaterial;
	Qt3DCore::QTransform* planeTransform;
	Qt3DExtras::QTextureMaterial* planeTexture;
	Qt3DCore::QEntity* planeEntity2;
	Qt3DCore::QEntity* lightEntity;
	Qt3DRender::QPointLight* light;
	Qt3DCore::QTransform* lightTransform;
	Qt3DExtras::QOrbitCameraController* cameraController;
	Qt3DCore::QTransform* SceneTransform;

	//Variables globales
	qint16* precValueTX;
	qint16* compteurTX;
	qint16* precValueTY;
	qint16* compteurTY;
	qint16* precValueTZ;
	qint16* compteurTZ;
	qint16* precValueRX;
	qint16* compteurRX;
	qint16* precValueRY;
	qint16* compteurRY;
	qint16* precValueRZ;
	qint16* compteurRZ;
	
public slots:
	void mouse3DMove();
	void Recentrer();
	void BloquerScene();

public:
	My3DScene();
	void createObjects();
	void createCamera();
	void init();
};