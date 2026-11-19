#include "clickablewidget.h"
#include "ui_clickablewidget.h"

ClickableWidget::ClickableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClickableWidget)
{
    ui->setupUi(this);
}

ClickableWidget::~ClickableWidget()
{
    delete ui;
}
