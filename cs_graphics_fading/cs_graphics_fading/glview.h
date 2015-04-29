#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QPoint>
#include <QMouseEvent>
#include "ui_glview.h"
#include "glm.h"
#include "heads.h"
#include "SceneBuild.h"
#include "color.h"
#include "interactive.h"

typedef RenderAlgorithm::color color;

/*this is file is the sub View attached to the Main window.
*the child class of QGLWidget.
*/
class GLView : public QGLWidget
{
	Q_OBJECT

public:
	GLView(QWidget *parent = 0);
	~GLView();

private:
	Ui::GLView ui;
	int render_type; /*0x000001: use OpenGL API to draw    0x000002: use Z-buffer to draw  0x000003: use scanning-line Z-buffer to draw 
					*0x000004: use section Z-buffer to draw
					*/
	GLMmodel *m_GLMmodel;
	GLMgroup *m_group;
	BaseRender *m_interface; /*the algorithm interface*/
	SceneBuilding *m_sceneBuilding;
	QPoint m_lastPoint;
	/*rotation angle*/
	GLfloat m_xRotation,m_yRotation;  /*just realize the rotation*/
	GLfloat m_modelViewMatrix[16],m_projection[16];
	std::vector<std::vector<point3D> > m_triangle;
	std::vector<std::vector<point3D> > m_orgin;
	int m_minHeight,m_maxHeight;
	std::vector<color> m_color;
	GeometryInteractive::Interactive *m_interactive;  //mouse control and key control,the second not be realized!
	int m_clrtype;
private:
	void saveGLState();
	void restoreGLState();
	void tranlate();
	/*rotate the model flag*/
protected:
	void paintEvent(QPaintEvent *event); /*paint the mesh model*/
	void initialize();
	/*interactive*/
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event); 
public:
	bool OpenFile(QString PathName);
	void drawModel();   /*using OpenGL to draw Model,just for comparing*/
	void setRenderType(int type){
		render_type=type;
	}
	//set color change type
	void setClrtype(int clrtype){
		m_clrtype=clrtype;
	}
};

#endif // GLVIEW_H
