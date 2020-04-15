#include "Scene3D.h"
#include "DICOM_3Dmouse.h"
extern int Coupe, Min, Max;
//------------------------------------------------------------------------------
//--- Private methods ----------------------------------------------------------
//------------------------------------------------------------------------------
// Create the 3D objects of the scene.

void My3DScene::init() {

    precValueTX = new qint16;
    compteurTX = new qint16;
    *precValueTX = 0;
    *compteurTX = 0;

    precValueTY = new qint16;
    compteurTY = new qint16;
    *precValueTY = 0;
    *compteurTY = 0;

    precValueTZ = new qint16;
    compteurTZ = new qint16;
    *precValueTZ = 0;
    *compteurTZ = 0;

    precValueRX = new qint16;
    compteurRX = new qint16;
    *precValueRX = 0;
    *compteurRX = 0;

    precValueRY = new qint16;
    compteurRY = new qint16;
    *precValueRY = 0;
    *compteurRY = 0;

    precValueRZ = new qint16;
    compteurRZ = new qint16;
    *precValueRZ = 0;
    *compteurRZ = 0;
}


void My3DScene::createObjects()
{
    string coupe = to_string(Coupe);
    Qt3DRender::QTextureLoader* loader;
    //-----------------------------------------------------------------------------------------------------------

    for (int i = Min; i <Max-1; i++)
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
            
            cheminimage = "Images/Coupe"+coupe+"_" + numero + format;
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
    this->camera()->setViewCenter(QVector3D(0, 0.02 * (Max - Min) / 2, 0));

}

void My3DScene::mouseMove()
{    
    this->SceneTransform = new Qt3DCore::QTransform();
    int h = *compteurTX;
    int l = *compteurTY;
    int m = *compteurTZ;

    this->SceneTransform->setTranslation(QVector3D((float)h/10.0, (float)l/2.0, (float)m / 10.0));

    if ((-pTx > 5) && (-pTx >= *precValueTX) && (-pTx < 20)) {
        h = h - 1;
    }
    else if ((-pTx >= 20) && (-pTx >= *precValueTX) && (-pTx < 600)) {
        h = h - 3;
    }

    else if ((-pTx < -5) && (-pTx <= *precValueTX) && (-pTx > -20)) {
        h = h + 1;
    }
    else if ((-pTx <= -20) && (-pTx <= *precValueTX) && (-pTx > -600)) {
        h = h + 3;
    }
    
    *compteurTX = h;
    *precValueTX = -pTx;

    if ((-pTy > 5) && (-pTy >= *precValueTY) && (-pTy < 20)) {
        l = l - 1;
    }
    else if ((-pTy >= 20) && (-pTy >= *precValueTY) && (-pTy < 600)) {
        l = l - 3;
    }

    else if ((-pTy < -5) && (-pTy <= *precValueTY) && (-pTy > -20)) {
        l = l + 1;
    }
    else if ((-pTy <= -20) && (-pTy <= *precValueTY) && (-pTy > -600)) {
        l = l + 3;
    }

    *compteurTY = l;
    *precValueTY = -pTy;

    if ((pTz > 5) && (pTz >= *precValueTZ) && (pTz < 20)) {
        m = m - 1;
    }
    else if ((pTz >= 20) && (pTz >= *precValueTZ) && (pTz < 600)) {
        m = m - 3;
    }

    else if ((pTz < -5) && (pTz <= *precValueTZ) && (pTz > -20)) {
        m = m + 1;
    }
    else if ((pTz <= -20) && (pTz <= *precValueTZ) && (pTz > -600)) {
        m = m + 3;
    }

    *compteurTZ = m;
    *precValueTZ = pTz;


    int i = *compteurRX;
    if ((i > -360) && (i < 360)) {
        this->SceneTransform->setRotationX(i);

        if ((-pRx > 5) && (-pRx >= *precValueRX) && (-pRx < 20)) {
            i = i - 1;
        }
        else if ((-pRx >= 20) && (-pRx >= *precValueRX) && (-pRx < 600)) {
            i = i - 3;
        }

        else if ((-pRx < -5) && (-pRx <= *precValueRX) && (-pRx > -20)) {
            i = i + 1;
        }
        else if ((-pRx <= -20) && (-pRx <= *precValueRX) && (-pRx > -600)) {
            i = i + 3;
        }
    }

    else if (i <= -360) {
        i = 0;
    }
    else if (i >= 360) {
        i = 0;
    }
    else if (-pRx >= 0 && -pRx < 50) {

        return;
    }
    *compteurRX = i;
    *precValueRX = -pRx;



    int j = *compteurRY;
    if ((j > -360) && (j < 360)) {
        this->SceneTransform->setRotationY(j);

        if ((-pRy > 5) && (-pRy >= *precValueRY) && (-pRy < 20)) {
            j = j - 1;
        }
        else if ((-pRy >= 20) && (-pRy >= *precValueRY) && (-pRy < 600)) {
            j = j - 3;
        }

        else if ((-pRy < -5) && (-pRy <= *precValueRY) && (-pRy > -20)) {
            j = j + 1;
        }
        else if ((-pRy <= -20) && (-pRy <= *precValueRY) && (-pRy > -600)) {
            j = j + 3;
        }
    }

    else if (j <= -360) {
        j = 0;
    }
    else if (j >= 360) {
        j = 0;
    }
    
    *compteurRY = j;
    *precValueRY = -pRy;

    int k = *compteurRZ;
    if ((k > -360) && (k < 360)) {
        this->SceneTransform->setRotationZ(k);

        if ((pRz > 5) && (pRz >= *precValueRZ) && (pRz < 20)) {
            k = k - 1;
        }
        else if ((pRz >= 20) && (pRz >= *precValueRZ) && (pRz < 600)) {
            k = k - 3;
        }

        else if ((pRz < -5) && (pRz <= *precValueRZ) && (pRz > -20)) {
            k = k + 1;
        }
        else if ((pRz <= -20) && (pRz <= *precValueRZ) && (pRz > -600)) {
            k = k + 3;
        }
    }

    else if (k <= -360) {
        k = 0;
    }
    else if (k >= 360) {
        k = 0;
    }
    else if (pRz >= 0 && pRz < 50) {

        return;
    }
    *compteurRZ = k;
    *precValueRZ = pRz;


    scene.addComponent(this->SceneTransform);
    
}

//------------------------------------------------------------------------------
//--- Constructors -------------------------------------------------------------
//------------------------------------------------------------------------------
// Initialize the vector singelton and create the window elements.
My3DScene::My3DScene()
{
    hWndMain3D = (HWND)this->winId();
    init();

    //this->SceneTransform1 = new Qt3DCore::QTransform(&this->scene);
    //this->SceneTransform2 = new Qt3DCore::QTransform(&this->scene);
    //this->SceneTransform3 = new Qt3DCore::QTransform(&this->scene);

    this->createObjects();
    this->createCamera();
    this->createLight();
    this->setRootEntity(&this->scene);
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &My3DScene::mouseMove);
    timer->start(10);
}