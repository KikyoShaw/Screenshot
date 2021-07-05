#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QPainter>
#include <QImage>
#include "ui_controlwidget.h"

//仿照QQ截图的控制面板
class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    ControlWidget(QWidget *parent = 0);
    ~ControlWidget();

public slots:
    void cancelBtn_slot();
    void finishBtn_slot();

signals:
	void sigCancelPixmap();
	void sigFinishPixmap();

private:
    Ui::ControlWidget ui;

};

#endif // CONTROLWIDGET_H
