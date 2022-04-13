#include "nasaimagevkpost.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NASAImageVKPost w;
    w.setFixedSize(274, 393);
    w.show();
    return a.exec();
}
