#pragma once

#include <QtWidgets/QWidget>
#include "ui_Screenshot.h"

#include "windows.h"

class CaptureScreen;

class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot(QWidget *parent = Q_NULLPTR);
	~Screenshot();

private:
	void getScreenShotImage(const char* filename, HWND hWnd);

private slots:
	void sltScreenShot();
	void sltOnCompleteCature(QPixmap);

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::ScreenshotClass ui;
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//截图工具类
	QSharedPointer<CaptureScreen> m_captureHelper = nullptr;
};
