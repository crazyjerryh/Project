#include "qfadingwindow.h"
#include <QFileDialog>
#include <QMessageBox>

QFadingWindow::QFadingWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),m_subView(0)
{
	ui.setupUi(this);
	CreateSignal();
	/*Default Mode*/
	this->ui.actionNormal->setCheckable(true);
	this->ui.actionZbuffer->setCheckable(true);
	this->ui.actionScanning_Line->setCheckable(true);
	this->ui.actionSection_Zbuffer->setCheckable(true);
	this->ui.actionChange_Color->setCheckable(true);
	this->ui.actionSet_Color->setCheckable(true);

	this->ui.actionNormal->setChecked(true);
	this->ui.actionChange_Color->setChecked(false);
	this->ui.actionSet_Color->setChecked(true);  //random color set ,name is not key

	m_type=0x001;
	m_clrtype=0x001;
}

QFadingWindow::~QFadingWindow()
{
	if(m_subView)
		delete m_subView;
}

void QFadingWindow::CreateSignal()
{
	connect(this->ui.actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(this->ui.actionZbuffer,SIGNAL(triggered()),this,SLOT(zBufferRender()));
	connect(this->ui.actionScanning_Line,SIGNAL(triggered()),this,SLOT(scanningRender()));
	connect(this->ui.actionSection_Zbuffer,SIGNAL(triggered()),this,SLOT(sectionRender()));
	connect(this->ui.actionNormal,SIGNAL(triggered()),this,SLOT(normalRender()));
	connect(this->ui.actionChange_Color,SIGNAL(triggered()),this,SLOT(setColorType()));
	connect(this->ui.actionSet_Color,SIGNAL(triggered()),this,SLOT(setRandomColor()));
}

void QFadingWindow::setChecked(bool type)
{
	this->ui.actionNormal->setChecked(type);
	this->ui.actionScanning_Line->setChecked(type);
	this->ui.actionSection_Zbuffer->setChecked(type);
	this->ui.actionZbuffer->setChecked(type);
}

#pragma region SLOT
void QFadingWindow::openFile()
{
	QString pathName=QFileDialog::getOpenFileName(this,tr("Open OBJ FILE"),"..","OBJ FILES(*.obj)");
	if(0==pathName.length()){
		QMessageBox dialog(QMessageBox::Warning,tr("Error"),tr("please open files"));
		dialog.exec();
		return;
	}
	
	if(m_subView)
		delete m_subView;
	/*add the view int the main window*/
	m_subView=new GLView;
	m_subView->setWindowTitle(tr("Render Window"));
	this->ui.mdiArea->addSubWindow(m_subView);
	m_subView->showMaximized();  /*get width before openFile*/
	bool result;
	if(m_subView)
		result=m_subView->OpenFile(pathName);

	if(m_subView)
	{
		m_subView->setRenderType(m_type);
		m_subView->setClrtype(m_clrtype);  //set color change type
	}

    if(!result){
		QMessageBox dlg(QMessageBox::Warning,tr("Error"),tr("open File error"));
		dlg.exec();
		if(m_subView)
			delete m_subView;
		return;
	}
	m_subView->show();
	/*show main window*/
}

void QFadingWindow::zBufferRender()
{
	setChecked(false);
	this->ui.actionZbuffer->setChecked(true);
    m_type=0x002;
}

void QFadingWindow::sectionRender()
{
	setChecked(false);
	this->ui.actionSection_Zbuffer->setChecked(true);
	m_type=0x003;
}

void QFadingWindow::scanningRender()
{
	setChecked(false);
	this->ui.actionScanning_Line->setChecked(true);
	m_type=0x004;
}

void QFadingWindow::normalRender()
{
	setChecked(false);
	this->ui.actionNormal->setChecked(true);
	m_type=0x001;
}

void QFadingWindow::setColorType()
{
	//set color change type
	m_clrtype=0x02;
	this->ui.actionChange_Color->setChecked(true);
	this->ui.actionSet_Color->setChecked(false);
	
}

void QFadingWindow::setRandomColor()
{
	m_clrtype=0x01;
	this->ui.actionChange_Color->setChecked(false);
	this->ui.actionSet_Color->setChecked(true);
}
#pragma endregion