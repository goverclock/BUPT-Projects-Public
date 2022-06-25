#include <windows.h>

#include <QApplication>

#include "primary.h"

int main(int argc, char *argv[]) {
    FreeConsole();
    QApplication a(argc, argv);
    Primary primary;

    return a.exec();
}
