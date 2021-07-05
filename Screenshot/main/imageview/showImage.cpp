#include "showImage.h"
#include <QMessageBox>
#include <QImageReader>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QPushButton>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <QFileInfo>
#include <QApplication>
#include <QDesktopWidget>

showImage::showImage(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(ui.pushButton_close, &QPushButton::clicked, this, &QWidget::close);

	ui.widget_image->hide();
	ui.widget_gif->hide();

	sysHeight = QApplication::desktop()->height();
	sysWidget = QApplication::desktop()->width();

}

showImage::~showImage()
{
}

void showImage::toShowImage(const QString & fileName)
{
	int imageType = getImageFormat(fileName);

	if (E_ImageType::E_Image == imageType)
	{
		QImageReader reader(fileName);
		reader.setDecideFormatFromContent(true);
		if (!reader.canRead()) {
			qInfo() << QStringLiteral("[%1][file cant read]:").arg(__FUNCTION__) << fileName;
			ui.widget_image->setFixedSize(QSize(464, 522));
			return;
		}
		ui.widget_image->show();
		auto size = reader.size();
		if (size.height() < 464 && size.width() < 522) {
			ui.widget_image->setFixedSize(QSize(464, 522));
		}
		else if (size.height() > size.width() && size.height() > (sysHeight - 120)) {
			if (size.height() > 2 * size.width()) {
				float m_heght = sysHeight - 120;
				float m_width = m_heght * (1 - (float)size.width() / (float)size.height());
				ui.widget_image->setFixedSize(QSize(m_width, m_heght));
			}
			else {
				float m_heght = sysHeight - 120;
				float m_width = m_heght * (float)size.width() / (float)size.height();
				ui.widget_image->setFixedSize(QSize(m_width, m_heght));
			}
		}
		else if (size.height() < size.width() && size.width() > (sysWidget - 600)) {
			if (size.width() > 2 * size.height()) {
				float m_width = (sysWidget - 600);
				float m_heght = m_width * (1 - (float)size.height() / (float)size.width());
				if (size.height() > (sysHeight - 120))
					m_heght = sysHeight - 120;
				ui.widget_image->setFixedSize(QSize(m_width, m_heght));
			}
			else {
				float m_width = (sysWidget - 600);
				float m_heght = m_width * (float)size.height() / (float)size.width();
				if (size.height() > (sysHeight - 120))
					m_heght = sysHeight - 120;
				ui.widget_image->setFixedSize(QSize(m_width, m_heght));
			}
		}
		else if (size.height() == size.width() && size.width() > 500) {
			ui.widget_image->setFixedSize(QSize(500, 500));
		}
		else {
			ui.widget_image->setFixedSize(QSize(size.width(), size.height()));
		}
		ui.widget_image->toSetImageValue(reader.read());
	}
	else if (E_ImageType::E_GifImage == imageType)
	{
		ui.widget_gif->show();
		ui.widget_gif->setFixedSize(530, 530);
		QSize m_size = getGifSize(fileName);
		ui.label_gif->setResource(fileName);
		double poxWidth = m_size.width();
		double poxHeight = m_size.height();
		//ui.label_gif->setScaledSize(m_size);
		if (m_size.width() > ui.widget_gif->width() && m_size.width() >= m_size.height())
		{
			poxWidth = ui.widget_gif->width();
			poxHeight = poxWidth * ((float)m_size.height() / (float)m_size.width());
		}
		else if (m_size.height() > ui.widget_gif->height() && m_size.width() < m_size.height())
		{
			poxHeight = ui.widget_gif->height();
			poxWidth = poxHeight * ((float)m_size.width() / (float)m_size.height());
		}
		ui.label_gif->setScaledSize(poxWidth, poxHeight);
		ui.label_gif->getPsize(ui.widget_gif->width(), ui.widget_gif->height());
		/*if (width() < m_size.width() || height() < m_size.height())
			setFixedSize(m_size.width() + 8, m_size.height() + 48 + 8);
		else
			setFixedSize(538, 586);
		ui.label_gif->getPsize(width() - 8, height() - 48 - 8);*/
	}

}

void showImage::mouseMoveEvent(QMouseEvent * e)
{
	if ((e->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(e->globalPos() - m_point);//移动窗口
	}
}

void showImage::mousePressEvent(QMouseEvent * e)
{
	//鼠标事件中包含“按住的是左键”
	if ((e->button() == Qt::LeftButton) && (e->pos().y() < 48) && (e->pos().y() > 0))
	{
		m_bMove = true;
		//获取移动的位移量
		m_point = e->globalPos() - frameGeometry().topLeft();
	}
}

void showImage::mouseReleaseEvent(QMouseEvent * e)
{
	m_bMove = false;
}

void showImage::showEvent(QShowEvent * e)
{
	int pos_X = (sysWidget - this->width()) / 2;
	int pos_Y = (sysHeight - 40 - this->height()) / 2;
	move(pos_X, pos_Y);
}

//当做Image图片处理
QSize showImage::getGifSize(const QString & fileName)
{
	QImageReader reader(fileName);
	reader.setDecideFormatFromContent(true);
	if (!reader.canRead()) {
		qInfo() << QStringLiteral("[%1][file cant read]:").arg(__FUNCTION__) << fileName;
		return QSize();
	}
	int width = reader.size().width();
	int height = reader.size().height();
	return QSize(width, height);
}

int showImage::getImageFormat(const QString & fileName)
{
	//BMP格式特征码
	unsigned char BMPHeader[] = { 0x42, 0x4d };
	//JPG,JPEG格式特征码
	unsigned char JPGHeader1[] = { 0xff, 0xd8, 0xff, 0xdb };
	unsigned char JPGHeader2[] = { 0xff, 0xd8, 0xff, 0xe0 };
	unsigned char JPGHeader3[] = { 0xff, 0xd8, 0xff, 0xe1 };
	unsigned char JPGHeader4[] = { 0xff, 0xd8, 0xff, 0xe2 };
	unsigned char JPGHeader5[] = { 0xff, 0xd8, 0xff, 0xe3 };
	unsigned char JPGHeader6[] = { 0xff, 0xd8, 0xff, 0xe8 };
	//GIF格式特征码
	unsigned char GIFHeader1[] = { 0x47, 0x49, 0x46, 0x38, 0x37, 0x61 };
	unsigned char GIFHeader2[] = { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61 };
	//PNG格式特征码
	unsigned char PNGHeader[] = { 0x89, 0x50, 0x4E, 0x47 };

	int count = 0;
	int step = 2;
	//以二进制方式打开文件并读取前几个字节
	unsigned char header[16];
	std::ifstream readf(fileName.toStdString().c_str(), std::ios::binary);
	if (!readf.is_open()) {
		qDebug() << "文件读取失败";
		return E_Error;
	}

	//先读两个，判断是否BMP格式
	for (int i = 0; i < step; i++) {
		readf >> header[count + i];
	}
	count = count + step;
	if (memcmp(header, BMPHeader, count) == 0) {
		qDebug() << "BMP格式";
		return E_Image;
	}
	//再读两个，判断是否JPG格式、PNG格式
	for (int i = 0; i < step; i++) {
		readf >> header[count + i];
	}
	count = count + step;
	
	if ((memcmp(header, JPGHeader1, count) == 0)
		|| (memcmp(header, JPGHeader2, count) == 0)
		|| (memcmp(header, JPGHeader3, count) == 0)
		|| (memcmp(header, JPGHeader4, count) == 0)
		|| (memcmp(header, JPGHeader5, count) == 0)
		|| (memcmp(header, JPGHeader6, count) == 0)) {
		qDebug() << "JPG格式";
		return E_Image;
	}
	else if (memcmp(header, PNGHeader, count) == 0) {
		qDebug() << "PNG格式";
		return E_Image;
	}
	//再读两个，判断是否GIF格式
	for (int i = 0; i < step; i++) {
		readf >> header[count + i];
	}
	count = count + step;
	if ((memcmp(header, GIFHeader1, count) == 0)
		|| (memcmp(header, GIFHeader2, count) == 0)) {
		qDebug() << "GIF格式";
		return E_GifImage;
	}
	return E_Error;
}

//读取GIF内部数据
//QSize showImage::getGifSize(const QString & fileName)
//{
//	std::ifstream fp(fileName.toStdString().c_str(), std::ios::binary);
//	if (!fp) {
//		std::cout << "Can not open this file." << std::endl;
//		return QSize(0, 0);
//	}
//	char s1[2] = { 0 }, s2[2] = { 0 };
//	fp.seekg(6);
//	fp.read(s1, 2);
//	fp.read(s2, 2);
//	int width = (unsigned int)(s1[1]) << 8 | (unsigned int)(s1[0]);
//	int height = (unsigned int)(s2[1]) << 8 | (unsigned int)(s2[0]);
//	fp.close();
//	return QSize(width, height);
//}
