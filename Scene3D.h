#pragma once
// Includes.

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

// Namespaces.
using namespace std;

// Class My3DScene.
class My3DScene : public Qt3DExtras::Qt3DWindow
{
private:
	// Attributes.
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

	// Private methods.
	void createObjects();
	void createLight();
	void createCamera();
	void afficher();

public:
	// Constructors.
	My3DScene();
};