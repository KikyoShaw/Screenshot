#include "Screenshot.h"
#include "capturescreen.h"
#include <QMouseEvent>
#include <QKeyEvent>

static LPWSTR ConvertLPWSTRToLPSTR(const char* szStr)
{
	static WCHAR wszClassName[256];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
		sizeof(wszClassName) / sizeof(wszClassName[0]));
	return (LPWSTR)wszClassName;
}

Screenshot::Screenshot(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(ui.pushButton_close, &QPushButton::clicked, this, &QWidget::close);
	connect(ui.pushButton_screen, &QPushButton::clicked, this, &Screenshot::sltScreenShot);
	connect(ui.pushButton_newScreen, &QPushButton::clicked, this, [=]() {
		QString name = "1.bmp";
		//截取整个屏幕winId传NULL
		getScreenShotImage(name.toStdString().c_str(), (HWND)winId());
	});
}

Screenshot::~Screenshot()
{
}

void Screenshot::getScreenShotImage(const char * filename, HWND hWnd)
{
	HDC hdc = CreateDC(ConvertLPWSTRToLPSTR("DISPLAY"), NULL, NULL, NULL);
	int32_t ScrWidth = 0, ScrHeight = 0;
	RECT rect = { 0 };
	if (hWnd == NULL)
	{
		ScrWidth = GetDeviceCaps(hdc, HORZRES);
		ScrHeight = GetDeviceCaps(hdc, VERTRES);
	}
	else
	{
		GetWindowRect(hWnd, &rect);
		ScrWidth = rect.right - rect.left;
		ScrHeight = rect.bottom - rect.top;
	}
	HDC hmdc = CreateCompatibleDC(hdc);

	HBITMAP hBmpScreen = CreateCompatibleBitmap(hdc, ScrWidth, ScrHeight);
	HBITMAP holdbmp = (HBITMAP)SelectObject(hmdc, hBmpScreen);

	BITMAP bm;
	GetObject(hBmpScreen, sizeof(bm), &bm);

	BITMAPINFOHEADER bi = { 0 };
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = bm.bmPlanes;
	bi.biBitCount = bm.bmBitsPixel;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = bm.bmHeight * bm.bmWidthBytes;
	// 图片的像素数据
	char *buf = new char[bi.biSizeImage];
	BitBlt(hmdc, 0, 0, ScrWidth, ScrHeight, hdc, rect.left, rect.top, SRCCOPY);
	GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)ScrHeight, buf, (LPBITMAPINFO)&bi, (DWORD)DIB_RGB_COLORS);

	BITMAPFILEHEADER bfh = { 0 };
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	HANDLE hFile = CreateFile(ConvertLPWSTRToLPSTR(filename), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dwWrite;
	WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
	WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
	WriteFile(hFile, buf, bi.biSizeImage, &dwWrite, NULL);
	CloseHandle(hFile);
	hBmpScreen = (HBITMAP)SelectObject(hmdc, holdbmp);
}

void Screenshot::sltScreenShot()
{
	// 点击截图按钮开始截图;
	m_captureHelper = QSharedPointer<CaptureScreen>(new CaptureScreen);
	if (m_captureHelper) {
		if (!m_captureHelper->isVisible()) {
			connect(m_captureHelper.data(), SIGNAL(signalFinshPixmap(QPixmap)), this, SLOT(sltOnCompleteCature(QPixmap)));
			m_captureHelper->show();
		}
	}
}

void Screenshot::sltOnCompleteCature(QPixmap pix)
{
	if (!pix.isNull()) {
		ui.label_image->setPixmap(pix);
		ui.label_image->setScaledContents(true);
	}
}

void Screenshot::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void Screenshot::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void Screenshot::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

void Screenshot::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_S) {
		sltScreenShot();
	}
	QWidget::keyPressEvent(event);
}


