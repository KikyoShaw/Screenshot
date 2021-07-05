#include "controlwidget.h"
#include <QPoint>
#include "capturescreen.h"
#include <QDebug>
#include <QApplication>
#include <QClipboard>

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
	connect(ui.cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtn_slot()));
	connect(ui.finishBtn, SIGNAL(clicked()), this, SLOT(finishBtn_slot()));
}

ControlWidget::~ControlWidget()
{
}

//������ͼ��ť
void ControlWidget::cancelBtn_slot()
{
	emit sigCancelPixmap();
}

//��ɰ�ť����ͼ���浽������
void ControlWidget::finishBtn_slot()
{
	emit sigFinishPixmap();
}
