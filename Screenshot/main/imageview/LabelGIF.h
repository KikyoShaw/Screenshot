#pragma once

#include <QLabel>
#include <QMovie>

class LabelGIF : public QLabel
{
	Q_OBJECT

public:
	LabelGIF(QWidget *parent = Q_NULLPTR);
	~LabelGIF();

	void setResource(const QString &path);
	void setScaledSize(const QSize &size);
	void setScaledSize(int w, int h);

	void getPsize(int w, int h);

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	/*void paintEvent(QPaintEvent *event);*/

signals:
	void sigMouseDoubleClick();

private:
	QMovie m_movie;
	QSize m_pSize;
	int posWidth;
	int posHeight;
	//“∆∂Ø Ù–‘÷µ
	QPoint m_point;
	volatile bool m_bMove = false;
};
