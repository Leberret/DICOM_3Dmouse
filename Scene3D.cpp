#include "Scene3D.h"
#include "DICOM_3Dmouse.h"

//------------------------------------------------------------------------------
//--- Private methods ----------------------------------------------------------
//------------------------------------------------------------------------------
// Create the 3D objects of the scene.
void My3DScene::createObjects()
{
    int Coupe = QInputDialog::getInt(new Interface,"Saisir valeur", "Coupe");
    string coupe = to_string(Coupe);
    int Min = QInputDialog::getInt(new Interface, "Saisir valeur", "Image Min");
    int Max = QInputDialog::getInt(new Interface, "Saisir valeur", "Image Max");
    Qt3DRender::QTextureLoader* loader;
    //-----------------------------------------------------------------------------------------------------------

    for (int i = Min; i <Max; i++)
    {
        for (int k = 0; k < 3; k += 1)
        {
            this->planeEntity = new Qt3DCore::QEntity(&this->scene);
            this->planeMesh = new Qt3DExtras::QPlaneMesh(this->planeEntity);
            this->planeMesh->setWidth(6);
            this->planeMesh->setHeight(5);
            this->planeTransform = new  Qt3DCore::QTransform(this->planeMesh);
            this->planeTransform->setTranslation(QVector3D(0, 0.02 * i + 0.005 * k, 0));


            // Add texture to 3D plane.

            loader = new Qt3DRender::QTextureLoader(this->planeMesh);
            this->planeTexture = new Qt3DExtras::QTextureMaterial(this->planeMesh);
            string cheminimage;
            string format = ".PNG";
            string numero = to_string(i);
            
            cheminimage = "Coupe"+coupe+"_" + numero + format;
            loader->setSource(QUrl::fromLocalFile(QString::fromStdString(cheminimage)));
            this->planeTexture->setTexture(loader);
            loader->setMirrored(false);
            this->planeTexture->setAlphaBlendingEnabled(true);
            this->planeEntity->addComponent(this->planeMesh);
            this->planeEntity->addComponent(this->planeTexture);
            this->planeEntity->addComponent(this->planeTransform);

            this->planeEntity = new Qt3DCore::QEntity(&this->scene);
            this->planeTransform = new  Qt3DCore::QTransform(this->planeMesh);
            this->planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 180.0f));
            this->planeTransform->setTranslation(QVector3D(0, 0.02 * i + 0.005 * k, 0));

            loader = new Qt3DRender::QTextureLoader(this->planeMesh);
            this->planeTexture = new Qt3DExtras::QTextureMaterial(this->planeMesh);
            loader->setSource(QUrl::fromLocalFile(QString::fromStdString(cheminimage)));
            this->planeTexture->setTexture(loader);

            this->planeTexture->setAlphaBlendingEnabled(true);
            this->planeEntity->addComponent(this->planeMesh);
            this->planeEntity->addComponent(this->planeTexture);
            this->planeEntity->addComponent(this->planeTransform);
        }
    }
}


//------------------------------------------------------------------------------
// Create the light of the scene.
void My3DScene::createLight()
{
    this->lightEntity = new Qt3DCore::QEntity(&this->scene);
    this->light = new Qt3DRender::QPointLight(this->lightEntity);
    this->light->setColor("white");
    this->light->setIntensity(1);
    this->lightEntity->addComponent(this->light);
    this->lightTransform = new Qt3DCore::QTransform(this->lightEntity);
    this->lightTransform->setTranslation(this->camera()->position());
    this->lightEntity->addComponent(this->lightTransform);
}

//------------------------------------------------------------------------------
// Create the 3D camera of the scene.
void My3DScene::createCamera()
{
    this->camera()->lens()->setPerspectiveProjection(10.0f, 1.0f, 0.1f, 1000.0f);
    this->camera()->setPosition(QVector3D(0, 70.0f, 0));
    this->camera()->setViewCenter(QVector3D(0, 0, 0));
    this->camera()->setUpVector(QVector3D(0, 0,1));
    // For camera controls
    this->cameraController = new Qt3DExtras::QOrbitCameraController(&this->scene);
    this->cameraController->setLinearSpeed(20.0f);
    this->cameraController->setLookSpeed(100.0f);
    this->cameraController->setCamera(this->camera());
    this->camera()->translate(QVector3D(-pTx / 10.0, pTy / 10.0, 0));

}

//------------------------------------------------------------------------------
//--- Constructors -------------------------------------------------------------
//------------------------------------------------------------------------------
// Initialize the vector singelton and create the window elements.
My3DScene::My3DScene()
{
    hWndMain3D = (HWND)this->winId();

    this->createObjects();
    this->createCamera();
    this->createLight();
    this->setRootEntity(&this->scene);
}