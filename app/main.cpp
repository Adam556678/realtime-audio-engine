#include "welcomewindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/resources/images/app_icon.png"));

    WelcomeWindow welcome;
    welcome.show();

    return a.exec();
}
