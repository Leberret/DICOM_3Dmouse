#include "Read_data_3Dmouse.h"
#include "DICOM_3Dmouse.h"


int main(int argc, char* argv[])
{

	QApplication a(argc, argv);
	Interface w;
	w.show();
    int res;
    /* Initialize 3D mouse */
    res = SbInit();

    /* if 3D mouse was not detected then print error, close win., exit prog. */
    if (res < 1)
    {
        cout << "Sorry - No supported 3Dconnexion device available.\n" << endl;

        ExitProcess(1);                /* exit program */
    }


    /* Function To be Repeated */
    return(DispatchLoopNT());
	return a.exec();
}

