#include "welcomewindow.h"
#include "mainwindow.h"
#include "ClickableWidget.h"
#include "ui_welcomewindow.h"
#include <QPixmap>
#include <iostream>

WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Welcome");

    QPixmap add_song(":/resources/images/plus_PNG119.png");
    QPixmap record_song(":/resources/images/mic_red.png");

    ui->addSong_img->setPixmap(add_song.scaled(40, 40, Qt::KeepAspectRatio));
    ui->record_img->setPixmap(record_song.scaled(60, 60, Qt::KeepAspectRatio));

    connect(ui->pickSong_widget, &ClickableWidget::clicked,
        this, &WelcomeWindow::pickASong);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::on_selectSongButton_clicked()
{
    // open the playback main window
    mainWindow = new MainWindow();

    mainWindow->show();

    this->hide(); // hide the welcome window
}

void WelcomeWindow::pickASong(){
    std::cout << "Pick a song clicked\n";
}

