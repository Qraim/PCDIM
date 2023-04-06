#include <QApplication>
#include "pcdim.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Commentez ou supprimez ces lignes
    pcdim widget;
    widget.show();


    return app.exec();
}

