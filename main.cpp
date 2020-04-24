#include "Read_data_3Dmouse.h"
#include "DICOM_3Dmouse.h"
#include "Scene3D.h"


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	Interface w;
	w.show();
    w.setStyleSheet("color: white;"
        "background-color:rgb(30,30,30);"
        "selection-color: white;"
        "selection-background-color: darkGrey;");
    Init3DMouse();
    

    return a.exec();
}