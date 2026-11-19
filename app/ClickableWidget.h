#ifndef CLICKABLEWIDGET_H
#define CLICKABLEWIDGET_H
#include <QWidget>
#include <QMouseEvent>

class ClickableWidget : public QWidget{
    Q_OBJECT

public:
    ClickableWidget(QWidget* parent = nullptr)
        : QWidget(parent){
        setAttribute(Qt::WA_StyledBackground, true);
    }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            emit clicked();

        QWidget::mousePressEvent(event);
    }
};

#endif // CLICKABLEWIDGET_H
