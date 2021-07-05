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
		Amplification,//�Ŵ�
		Shrink,//��С
		Lift,//��
		Right,//��
		Up,//����
		Down,//����
		Move//�ƶ�
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
	//��
	QPoint m_preDotPos;
	QPoint m_offset;        //�����ƶ�һ�ε�ͼƬƫ��ֵ
	int m_action;          //����(�Ŵ�,��С,�ƶ�...)
	QPoint m_Alloffset;           //��ƫ��
};

#endif // IMAGEVIEW_H
