#ifndef QFADINGWINDOW_H
#define QFADINGWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_qfadingwindow.h"
#include "glview.h"

class QFadingWindow : public QMainWindow
{
	Q_OBJECT

public:
	QFadingWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QFadingWindow();

private:
	Ui::QFadingWindowClass ui;
	GLView *m_subView;
private:
	void CreateSignal();
	void setChecked(bool type);
	int m_type;
	int m_clrtype;
private slots:
    void openFile();	
	void normalRender();
	void zBufferRender();
	void scanningRender();
	void sectionRender();  /*section render*/
	void setColorType();
	void setRandomColor(); //set random color
};

#endif // QFADINGWINDOW_H
