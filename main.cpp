#include "Read_data_3Dmouse.h"
#include "DICOM_3Dmouse.h"
#include "Scene3D.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	Interface w;
	w.show();
    w.setStyleSheet("color: white;"
        "background-color:black;"
        "selection-color: white;"
        "selection-background-color: darkGrey;");
    
    int res;
    /* Initialize 3D mouse */
    res = SbInit();

    /* if 3D mouse was not detected then print error, close win., exit prog. */
    if (res < 1)
    {
        QMessageBox error;
        error.setText("Sorry - No supported 3Dconnexion device available."); //Ajout à la boite QMessageBox
        error.exec();//affichage boite de dialogue
        

        ExitProcess(1);                /* exit program */
    }
    else {
        DispatchLoopNT();
    }

    return a.exec();
}