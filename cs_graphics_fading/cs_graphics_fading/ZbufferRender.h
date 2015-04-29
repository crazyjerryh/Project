#pragma once
#include <vector>
#include <algorithm>
#include <QPainter>
/*z-buffer algorithm ,you can render the model by z-buffer*/
#include "baserender.h"
#include "interactive.h"
//Z-Buffer算法实现类
namespace RenderAlgorithm{
class ZbufferRender :
	public BaseRender
{
public:
	ZbufferRender(void);
	~ZbufferRender(void);
private:
	std::vector<float> m_zvalue;
	std::vector<color> m_color;
	GeometryInteractive::Interactive *m_interactive;
public:
	void setglmModel(GLMmodel *glmModel);
	GLMmodel* getglmModel() {return m_glmModel;}
	void drawModel(QPainter &painter);
	void setVec(std::vector< std::vector<point> > &vec);
private:
	void runBuffer(QPainter &painter);  /*main body of algorithm*/
	bool intersect(std::vector<point> &vec,int line,int &x1,int &x2); 
	void draw();  /*use openGL API draw the projection triangle*/
	float calZvalue(std::vector<point> &triangle);
	void reset();
	GLMmodel* projective();  /*projective the model into the vector*/
};
}

