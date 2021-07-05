#ifndef SHOWIMAGE_H
#define SHOWIMAGE_H

#include <QWidget>
#include <QMouseEvent>
#include "ui_showImage.h"

enum E_ImageType
{
	E_Error = -1, //错误
	E_Image = 0, //正常图片
	E_GifImage = 1, //gif图片
};

class showImage : public QWidget
{
	Q_OBJECT

public:
	explicit showImage(QWidget *parent = 0);
	~showImage();

	void toShowImage(const QString &fileName);

protected:
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void showEvent(QShowEvent *e);

private:
	QSize getGifSize(const QString &fileName);
	int getImageFormat(const QString &fileName);

private:
	Ui::showImageWidget ui;
	int sysHeight = 0;
	int sysWidget = 0;
	QPoint m_point;
	bool m_bMove = false;
};


#endif // SHOWIMAGE_H
