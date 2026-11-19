#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

private slots:
    void on_selectSongButton_clicked();

private:
    Ui::WelcomeWindow *ui;
    MainWindow* mainWindow = nullptr;

    void pickASong();
};

#endif // WELCOMEWINDOW_H
