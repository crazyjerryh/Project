#pragma once
#include "glm.h"
#include <gl/GL.H>
#include <gl/glut.h>
#include <gl/GLU.H>
#include <QPainter>
#include "color.h"
//»ùÀà
namespace RenderAlgorithm{
typedef struct point_tag{
	double x,y,z; /*record the coordinate of x,y,z*/
	struct point_tag(double _x,double _y,double _z):x(_x),y(_y),z(_z){}
	struct point_tag(){}
}point;

class BaseRender
{
public:
	BaseRender(void);
	~BaseRender(void);
protected:
	GLMmodel *m_glmModel;
	GLMgroup *m_glmGroup;
	std::vector<std::vector<point> > m_vec;
	std::vector<std::vector<point>>  m_triangle; /*original triangle*/
	std::vector<color> m_tatcolor;  /*each triangle's color*/
	int m_fnormalize;  /*the flag of normalize*/
	int m_minHeight,m_maxHeight;
	int m_width,m_height;
	GLfloat m_modelViewArray[16];
	int m_clrtype;  //color change type
public:
    virtual void setglmModel(GLMmodel *glmModel)=0;
	virtual GLMmodel *getglmModel()=0;
	virtual void drawModel(QPainter &painter)=0;
	virtual void setVec(std::vector< std::vector<point> > &vec)=0;
	virtual void setHeight(int up,int bottom,int height){
		m_minHeight=up,m_maxHeight=bottom;
		m_height=height;  /*record the window's height*/
	}
	virtual void setWidth(int width){
	    m_width=width; /*set width*/
	}

	virtual void setColor(std::vector<color> &clr){
        m_tatcolor=clr;  /*the triangle's color*/
	}

	virtual void setOrigin(std::vector<std::vector<point>> &origin){
		m_triangle=origin;
	}

	virtual void setmodelView(GLfloat modelView[]){
		for(int i=0;i<16;i++)
			m_modelViewArray[i]=modelView[i];
	}

	virtual void setClrType(int clrtype){
		m_clrtype=clrtype;
	}
};
}

