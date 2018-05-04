#include "model/DBUtil.h"
#include "controller/macro_menu/MacroMenuController.h"
#include <QApplication>
//#include "controller/macro_editor/MacroEditorController.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DBUtil::init(); // Make sure database entities are initialized!

    MacroMenuController mainController;
    mainController.assumeControlFromParent(); // Activate main controller (which will initialize proper Main Menu view)!

    return a.exec();
}
