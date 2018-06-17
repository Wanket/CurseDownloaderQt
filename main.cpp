#include <QApplication>
#include <Window/MainWindow.h>
#include <iostream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return QApplication::exec();
}
