#pragma once

#include <QtWidgets/QWidget>
#include "ui_Screenshot.h"

#include "windows.h"

class CaptureScreen;
class showImage;

class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot(QWidget *parent = Q_NULLPTR);
	~Screenshot();

private:
	void testScreenShotImage();
	void getScreenShotImage(HWND hWnd);
	//����ͼƬ�ڵ�ǰĿ¼��
	void saveScreenShotBMP(const char* filename, HWND hWnd);
	//�鿴ͼƬ
	void lookImage(const QString &fileName);

private slots:
	void sltScreenShot();
	void sltOnCompleteCature(QPixmap);
	void sltOpenFileImage();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::ScreenshotClass ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//��ͼ������
	QSharedPointer<CaptureScreen> m_captureHelper = nullptr;
	//�鿴ͼƬ��
	QSharedPointer<showImage> m_imageViewer = nullptr;
};
