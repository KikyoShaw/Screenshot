#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QImage>

class ImageView : public QWidget
{
	Q_OBJECT

public:
	enum  Type {
		None = 0,
		Amplification,//放大
		Shrink,//缩小
		Lift,//左
		Right,//右
		Up,//向上
		Down,//向下
		Move//移动
	};

	ImageView(QWidget *parent = nullptr);
	~ImageView();

	void toSetImageValue(QImage image);

protected:
	void paintEvent(QPaintEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QPixmap m_pixmap;
	QImage m_image;
	QSize m_imageSize;
	QSize m_minImageSize;
	QSize m_maxImageSize;
	int m_sourceWidth;
	int m_sourceHeight;
	bool m_bPressed;
	int xPoint;
	int yPoint;
	//新
	QPoint m_preDotPos;
	QPoint m_offset;        //单次移动一次的图片偏移值
	int m_action;          //动作(放大,缩小,移动...)
	QPoint m_Alloffset;           //总偏移
};

#endif // IMAGEVIEW_H
