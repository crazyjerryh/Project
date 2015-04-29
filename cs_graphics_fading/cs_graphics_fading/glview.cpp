#include "glview.h"
//#include "debugs.h"
#include <omp.h>  /*openMP header file*/
#include <gl/GL.H>
#include <gl/glut.h>
#include <gl/GLU.H>
#include <ctime>

GLView::GLView(QWidget *parent)
	: QGLWidget(parent),m_GLMmodel(NULL),m_group(NULL),m_interface(NULL),m_xRotation(0),m_yRotation(0)
{
	ui.setupUi(this);
	render_type=0x000001;
	m_sceneBuilding=new SceneBuilding;
	m_minHeight=0x3fffff,m_maxHeight=0;
	if(!m_interactive)
		m_interactive=new GeometryInteractive::Interactive;
}

GLView::~GLView()
{
}

bool GLView::OpenFile(QString PathName)
{
	if(0==PathName.length()){
		return false;
	}

	/*open the obj file*/
	if(m_GLMmodel!=NULL)  delete m_GLMmodel;
	m_GLMmodel=new GLMmodel;

	std::string errorString;
	if(!glmReadOBJ(PathName.toLatin1().data(),m_GLMmodel,&errorString)){
		return false;
	}
	/*scalar normalize the mesh*/
	glmUnitize(m_GLMmodel);
	m_group=m_GLMmodel->groups;
	
	//projective();   /*this function waste too much time, while using parallel*/
	initialize();
	return true;
}

#pragma region  private function
void GLView::saveGLState()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
}

void GLView::restoreGLState()
{
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GLView::tranlate()
{
    /*realize the interactive*/
	int size=m_triangle.size();
	for(int tatNums=0;tatNums<size;tatNums++){
		/*for each triangle*/
		std::vector<GLfloat>  vec;
		int ptsize=m_triangle[tatNums].size();
		if(3!=ptsize)
			return;

		for(int ptNums=0;ptNums<3;++ptNums){
			/*translate each pt*/
			vec.push_back(m_triangle[tatNums][ptNums].x);
			vec.push_back(m_triangle[tatNums][ptNums].y);
			vec.push_back(m_triangle[tatNums][ptNums].z);
            /*translation and rotation*/			
			m_interactive->vecMulmat(m_modelViewMatrix,vec);
			/*change the pt value in vector m_triangle*/
			m_triangle[tatNums][ptNums].x=vec[0];
			m_triangle[tatNums][ptNums].y=vec[1];
			m_triangle[tatNums][ptNums].z=vec[2];
			/*end*/
		}
	}
}

#pragma endregion
/*use OpenGL to draw the Model 
*it is standard
*/

#pragma region openGL Render
void GLView::initialize()
{
	makeCurrent();
	//m_sceneBuilding->SetLight();  //just realize a function
}

void GLView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::HighQualityAntialiasing);  /*anti-aliasing*/
	
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,width(),height());
    
	saveGLState();
	/*set projection matrix*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2,2,-2*((double)height()/width()),2*((double)height()/width()),-100,100);

// #pragma region debug
// 	for(int i=1;i<=4;i++){
// 		for(int j=0;j<4;j++)
// 			fout<<m_modelViewMatrix[4*(i-1)+j]<<" ";
// 		fout<<endl;
// 	}
// 	fout<<endl;
// #pragma endregion
	glGetFloatv(GL_PROJECTION_MATRIX,m_projection);  /*get projection Matrix*/

	switch(render_type)
	{
	case 0x001:
		glRotatef(m_xRotation,1.0f,0.0f,0.0f);
		glRotatef(m_yRotation,0.0f,1.0f,0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX,m_modelViewMatrix); /*get model-view Matrix*/

	    if(m_interface!=NULL) 
			delete m_interface;
		
		m_interface=new InterfaceRender;
		m_interface->setglmModel(m_GLMmodel);
		m_interface->setClrType(m_clrtype);
		m_interface->drawModel(painter); 
		break;
	case 0x002:
		glMatrixMode(GL_MODELVIEW);
		glRotatef(m_xRotation,1.0f,0.0f,0.0f);
		//glRotatef(30,1.0f,0.0f,0.0f);
		glRotatef(m_yRotation,0.0f,1.0f,0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX,m_modelViewMatrix); /*get model-view Matrix*/
		if(m_interface!=NULL)
			delete m_interface;

		m_interface=new ZbufferRender;
		m_interface->setWidth(width());
		m_interface->setHeight(m_minHeight,m_maxHeight,height()); 
		m_interface->setglmModel(m_GLMmodel);  /*realize the Z-Buffer algorithm*/
		m_interface->setmodelView(m_modelViewMatrix);
		m_interface->setClrType(m_clrtype);
		m_interface->drawModel(painter);
		break;
	case 0x003:
		glMatrixMode(GL_MODELVIEW);
		glRotatef(m_xRotation,1.0f,0.0f,0.0f);
		glRotatef(m_yRotation,0.0f,1.0f,0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX,m_modelViewMatrix); /*get model-view Matrix*/
		if(m_interface!=NULL)
			delete m_interface;

		m_interface=new SectionRender;
		m_interface->setWidth(width());
		m_interface->setHeight(m_minHeight,m_maxHeight,height());
		m_interface->setglmModel(m_GLMmodel);
		m_interface->setmodelView(m_modelViewMatrix);
		m_interface->setClrType(m_clrtype);
		m_interface->drawModel(painter);
		break;
	case 0x004:
		glMatrixMode(GL_MODELVIEW);
		glRotatef(m_xRotation,1.0f,0.0f,0.0f);
		glRotatef(m_yRotation,0.0f,1.0f,0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX,m_modelViewMatrix); /*get model-view Matrix*/
		if(m_interface!=NULL)
			delete m_interface;

		m_interface=new ScanRender; /**/
		m_interface->setWidth(width());
		m_interface->setHeight(m_minHeight,m_maxHeight,height());
		m_interface->setglmModel(m_GLMmodel);
		m_interface->setmodelView(m_modelViewMatrix);
		m_interface->setClrType(m_clrtype);
		m_interface->drawModel(painter);
		break;
	}
	
	restoreGLState();
}

#pragma endregion

#pragma region interactive 
void GLView::mouseMoveEvent(QMouseEvent *event)
{
	/*mouse move event*/
	if(event->buttons() & Qt::LeftButton){
		m_xRotation += 180 * GLfloat(event->y() - m_lastPoint.y()) / width();
		m_yRotation += 180 * GLfloat(event->x() - m_lastPoint.x()) / height();
		update();
	}
	m_lastPoint=event->pos();
}

void GLView::mousePressEvent(QMouseEvent *event)
{
	m_lastPoint=event->pos();
}
#pragma endregion