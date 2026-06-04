#include "MainWindow.h"
#include "Character.h"

#include <QApplication>
#include <QMetaType>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<Character>("Character");

    MainWindow window;
    window.show();

    return app.exec();
}
