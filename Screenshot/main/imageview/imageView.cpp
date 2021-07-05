#include "imageView.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QStyleOption>
#include <QPainter>
#include <QFileDialog>

ImageView::ImageView(QWidget *parent)
{

}

ImageView::~ImageView()
{

}

void ImageView::toSetImageValue(QImage image)
{
	m_image = image;
	int height = qMax(image.height(), this->height());
	int width = qMax(image.width(), this->width());
	if (image.height() < image.width()) {
		height = this->height();
		width = image.width();
		//width = height * 1.0 * (image.width() * 1.0 / image.height());
	}
	else {
		width = this->width();
		height = image.height();
		//height = width * 1.0 / (image.width() * 1.0 / image.height());
	}

	m_image = image.scaled(width, height, Qt::KeepAspectRatio);
	m_pixmap = QPixmap::fromImage(m_image);
	m_imageSize = m_pixmap.size();

	m_sourceWidth = m_imageSize.width();
	m_sourceHeight = m_imageSize.height();
	m_minImageSize = QSize(m_imageSize.width() / 10, m_imageSize.height() / 10);;
	m_maxImageSize = QSize(m_imageSize.width() * 3, m_imageSize.height() * 3);
}

void ImageView::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	QSize widgetSize;
	widgetSize = this->size();

	int NowW = m_imageSize.width();//���ڵĿ�
	int NowH = m_imageSize.height();

	xPoint = widgetSize.width() / 2 - m_imageSize.width() / 2;
	yPoint = widgetSize.height() / 2 - m_imageSize.height() / 2;

	if (m_action == ImageView::Move)                    //�ƶ�
	{
		int offsetx = m_Alloffset.x() + m_offset.x();
		m_Alloffset.setX(offsetx);//��ƫ�� x

		int offsety = m_Alloffset.y() + m_offset.y();
		m_Alloffset.setY(offsety); //��ƫ�� y
		m_action = ImageView::None;
	}

	if (abs(m_Alloffset.x()) >= abs(xPoint))    //����Xƫ��ֵ
	{
		if (m_Alloffset.x() > 0)//����
			m_Alloffset.setX(abs(xPoint));//���ֵ
		else//����
			m_Alloffset.setX(-abs(xPoint));
	}

	if (abs(m_Alloffset.y()) >= abs(yPoint))    //����Xƫ��ֵ
	{
		if (m_Alloffset.y() > 0)//����
			m_Alloffset.setY(abs(yPoint));//���ֵ
		else//����
			m_Alloffset.setY(-abs(yPoint));
	}

	int x = widgetSize.width() / 2 + m_Alloffset.x() - NowW / 2;
	if (x < 0)x = 0;

	int y = widgetSize.height() / 2 + m_Alloffset.y() - NowH / 2;
	if (y < 0)y = 0;

	int  sx = NowW / 2 - widgetSize.width() / 2 - m_Alloffset.x();
	if (sx < 0)sx = 0;

	int  sy = NowH / 2 - widgetSize.height() / 2 - m_Alloffset.y();
	if (sy < 0)sy = 0;

	int w = (NowW - sx) > widgetSize.width() ? widgetSize.width() : (NowW - sx);
	if (w > (widgetSize.width() - x))
		w = widgetSize.width() - x;

	int h = (NowH - sy) > widgetSize.height() ? widgetSize.height() : (NowH - sy);
	if (h > (widgetSize.height() - y))
		h = widgetSize.height() - y;

	if ((xPoint > 0) && (yPoint > 0)) {
		//�������㣬���Ա�֤���Ŵ��Ժ��϶�������λ�ú���С������ı䴦������λ��.
		int offSetX = xPoint;
		int offSetY = yPoint;
		painter.drawPixmap(offSetX, offSetY, m_pixmap);
	}
	else {
		painter.drawPixmap(x, y, w, h, m_pixmap, sx, sy, w, h);             //�滭ͼ��
	}
}

void ImageView::wheelEvent(QWheelEvent *event)
{
	QImage imgScaled;
	//int numDegrees = event->delta() / 8;
	//int numSteps = numDegrees / 15;

	int numDegrees = event->delta();
	int numSteps = numDegrees;

	if (m_imageSize == QSize(0, 0)) {
		m_imageSize = m_image.size();
	}
	if ((m_imageSize.width() <= m_maxImageSize.width()) && \
		(m_imageSize.height() <= m_maxImageSize.height()) && \
		(m_imageSize.width() >= m_minImageSize.width()) && \
		(m_imageSize.height() >= m_minImageSize.height())) {
		if (numSteps > 0) {//���Ϲ���
			imgScaled = m_image.scaled(m_imageSize.width() * 12 / 11,
				m_imageSize.height() * 12 / 11,
				Qt::KeepAspectRatio);
		}
		else {
			imgScaled = m_image.scaled(m_imageSize.width() * 11 / 12,
				m_imageSize.height() * 11 / 12,
				Qt::KeepAspectRatio);
		}
		//��painter����ͼƬ
		m_pixmap = QPixmap::fromImage(imgScaled);
		m_imageSize = m_pixmap.size();
		this->update();
		//������� ֻ����С
	}
	else if ((m_imageSize.width() >= m_maxImageSize.width()) && \
		(m_imageSize.height() >= m_maxImageSize.height()) && \
		(numSteps < 0)) {
		if (numSteps > 0) {//���Ϲ���
			imgScaled = m_image.scaled(m_imageSize.width() * 12 / 11,
				m_imageSize.height() * 12 / 11,
				Qt::KeepAspectRatio);
		}
		else {
			imgScaled = m_image.scaled(m_imageSize.width() * 11 / 12,
				m_imageSize.height() * 11 / 12,
				Qt::KeepAspectRatio);
		}
		//��painter����ͼƬ
		m_pixmap = QPixmap::fromImage(imgScaled);
		m_imageSize = m_pixmap.size();
		this->update();
		//С����С��ֻ�ܷŴ�
	}
	else if ((m_imageSize.width() <= m_minImageSize.width()) && \
		(m_imageSize.height() <= m_minImageSize.height() && \
		(numSteps > 0))) {
		if (numSteps > 0) {//���Ϲ���
			imgScaled = m_image.scaled(m_imageSize.width() * 12 / 11,
				m_imageSize.height() * 12 / 11,
				Qt::KeepAspectRatio);
		}
		else {
			imgScaled = m_image.scaled(m_imageSize.width() * 11 / 12,
				m_imageSize.height() * 11 / 12,
				Qt::KeepAspectRatio);
		}
		//��painter����ͼƬ
		m_pixmap = QPixmap::fromImage(imgScaled);
		m_imageSize = m_pixmap.size();
		m_sourceWidth = m_imageSize.width();
		m_sourceHeight = m_imageSize.height();
		this->update();
	}
	//else {
	//	qInfo() << "��������";
	//}
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
	if (Qt::LeftButton == event->button()) {
		m_bPressed = true;
		m_preDotPos = event->pos();
	}
	QWidget::mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) && m_bPressed && ((yPoint < 0) || (xPoint < 0))) {
		this->setCursor(Qt::SizeAllCursor);
		m_offset.setX(event->pos().x() - m_preDotPos.x());//ƫ���� x
		m_offset.setY(event->pos().y() - m_preDotPos.y());//ƫ���� y
		m_preDotPos = event->pos();//�ı� ǰ��
		m_action = ImageView::Move;
		event->accept();
		this->update();
	}
	else {
		QWidget::mouseMoveEvent(event);
	}

}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
	m_bPressed = false;
	this->setCursor(Qt::ArrowCursor);
}