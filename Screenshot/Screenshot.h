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
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//��ͼ������
	QSharedPointer<CaptureScreen> m_captureHelper = nullptr;
};
