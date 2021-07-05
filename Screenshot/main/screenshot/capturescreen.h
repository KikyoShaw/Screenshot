#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include <QWidget>
#include <QPainter>
#include <QTime>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QSharedPointer>
#include <QMenu>
#include <QBuffer>

class ControlWidget;

enum CaptureState {
	InitCapture = 0,
	BeginCaptureImage,
	FinishCaptureImage,
	BeginMoveCaptureArea,
	FinishMoveCaptureArea,
	BeginMoveStretchRect,
	FinishMoveStretchRect,
	BeginMoveFullScreen,
	FinishMoveFullScreen
}; //进行截屏的状态;

enum StretchRectState {
	NotSelect = 0,
	TopLeftRect,
	TopRightRect,
	BottomLeftRect,
	BottomRightRect,
	LeftCenterRect,
	TopCenterRect,
	RightCenterRect,
	BottomCenterRect
};// 当前鼠标所在顶点状态;

class CaptureScreen : public QWidget
{
	Q_OBJECT

public:
	CaptureScreen(QWidget *parent = Q_NULLPTR);
	~CaptureScreen();

	//退出截图
	void exitPixmap();
	//保存截图
	void savePixmap();
	//完成截图
	void finishPixmap();

Q_SIGNALS:
	// 通知截图完毕，并将截取图片传递给调用类;
	void signalCompleteCature(QPixmap catureImage, QString ImageMD5, QByteArray ImageBinary);
	//重新截图通知
	void signalAfreshPixmap();
	//截图通知完成
	void signalFinshPixmap(QPixmap catureImage);

private:
	void initWindow();
	void initStretchRect();
	void loadBackgroundPixmap();
	QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);
	QRect getMoveRect();
	QRect getStretchRect();
	bool isPressPointInSelectRect(QPoint mousePressPoint);
	QRect getSelectRect();
	QPoint getMovePoint();
	StretchRectState getStrethRectState(QPoint point);
	void setStretchCursorStyle(StretchRectState stretchRectState);

	void drawCaptureImage();
	void drawStretchRect();

	QString getFileMd5(QString filePath);
	QByteArray getBinary(QString filePath);

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	QMenu m_menu;
	QPixmap m_loadPixmap, m_capturePixmap;
	int m_screenwidth;
	int m_screenheight;
	// 保存确定选区的坐标点;
	QPoint m_beginPoint, m_endPoint, m_beginMovePoint, m_endMovePoint;
	QPainter m_painter;
	//记录鼠标点击时的坐标
	QPoint m_mousePressPoint;
	// 保存当前截图状态;
	CaptureState m_currentCaptureState;
	// 当前选择区域矩形;
	QRect m_currentSelectRect;
	// 选中矩形8个顶点小矩形;
	QRect m_topLeftRect, m_topRightRect, m_bottomLeftRect, m_bottomRightRect;
	QRect m_leftCenterRect, m_topCenterRect, m_rightCenterRect, m_bottomCenterRect;
	// 当前鼠标所在顶点状态;
	StretchRectState m_stretchRectState;

	QSharedPointer<ControlWidget> controlW;

	bool m_bMove = false;

	
};

#endif // CAPTURESCREEN_H
