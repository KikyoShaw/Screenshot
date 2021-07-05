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

//撤销截图按钮
void ControlWidget::cancelBtn_slot()
{
	emit sigCancelPixmap();
}

//完成按钮将截图保存到剪贴板
void ControlWidget::finishBtn_slot()
{
	emit sigFinishPixmap();
}
