#include "LabelGIF.h"
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>

LabelGIF::LabelGIF(QWidget *parent)
	: QLabel(parent)
{
	setAlignment(Qt::AlignCenter);
	//setFrameStyle(QFrame::Panel | QFrame::Sunken);
	setMovie(&m_movie);
	//setScaledContents(true);
}

LabelGIF::~LabelGIF()
{
}

void LabelGIF::setResource(const QString & path)
{
	if (m_movie.state() == QMovie::Running) {
		return;
	}
	m_movie.setFileName(path);
	m_movie.start();
}

void LabelGIF::setScaledSize(const QSize & size)
{
	setFixedSize(size);
	m_movie.setScaledSize(size);
	m_pSize = size;
}

void LabelGIF::setScaledSize(int w, int h)
{
	QSize size(w, h);
	setFixedSize(size);
	m_movie.setScaledSize(size);
	m_pSize = size;
}

void LabelGIF::mouseDoubleClickEvent(QMouseEvent *e)
{
	emit sigMouseDoubleClick();
}

void LabelGIF::getPsize(int w, int h)
{
	posWidth = w;
	posHeight = h;
}

void LabelGIF::wheelEvent(QWheelEvent * event)
{
	int value = event->delta();
	double poxWidth = m_pSize.width();
	double poxHeight = m_pSize.height();

	if (value > 0)
	{
		if (m_pSize.width() > m_pSize.height()) {
			poxWidth = m_pSize.width() + 10;
			if (posWidth <= m_pSize.width())
				poxWidth = posWidth;
			poxHeight = poxWidth * ((float)m_pSize.height() / (float)m_pSize.width());
		}
		else
		{
			poxHeight = m_pSize.height() + 10;
			if (posHeight <= m_pSize.height())
				poxHeight = posHeight;
			poxWidth = poxHeight * ((float)m_pSize.width() / (float)m_pSize.height());
		}
	//	qInfo() << QStringLiteral("放大操作") << "poxHeight = " << poxHeight << "poxWidth = " << poxWidth;
	}
	else
	{	
		if (m_pSize.width() > m_pSize.height()) {
			poxWidth = m_pSize.width() - 10;
			if (120 >= m_pSize.width())
				poxWidth = 120;
			poxHeight = poxWidth * ((float)m_pSize.height() / (float)m_pSize.width());
		}
		else
		{
			poxHeight = m_pSize.height() - 10;
			if (120 >= m_pSize.height())
				poxHeight = 120;
			poxWidth = poxHeight * ((float)m_pSize.width() / (float)m_pSize.height());
		}
	//	qInfo() << QStringLiteral("缩小操作") << "poxHeight = " << poxHeight << "poxWidth = " << poxWidth;
	}

	m_pSize = QSize(poxWidth, poxHeight);
	setScaledSize(m_pSize);
}

void LabelGIF::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->pos();
	}
	QWidget::mousePressEvent(event);
}

void LabelGIF::mouseMoveEvent(QMouseEvent * event)
{
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		setCursor(Qt::SizeAllCursor);
		if (parentWidget()) 
		{
			QSize parentSize = parentWidget()->size();
			QPoint movePos = event->pos() - m_point + pos();
			if (movePos.x() < 0) {//left
				movePos.setX(0);
			}
			if (movePos.y() < 0) {//top
				movePos.setY(0);
			}
			if ((movePos.x() + size().width()) > parentSize.width()) {//right
				movePos.setX(parentSize.width() - size().width());
			}
			if ((movePos.y() + size().height()) > parentSize.height()) {//bottom
				movePos.setY(parentSize.height() - size().height());
			}
			move(movePos);
			repaint();
		}
	}
	QWidget::mouseMoveEvent(event);
}

void LabelGIF::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	setCursor(Qt::ArrowCursor);
	QWidget::mouseReleaseEvent(event);
}

//void LabelGIF::paintEvent(QPaintEvent * event)
//{
//	QStyleOption opt;
//	opt.init(this);
//	QPainter painter(this);
//	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
//
//	painter.translate(this->width() / 2 + m_XPtInterval, this->height() / 2 + m_YPtInterval);
//
//	painter.scale(m_ZoomValue, m_ZoomValue);
//
//	move(this->width() / 2 + m_XPtInterval, this->height() / 2 + m_YPtInterval);
//	
//}
