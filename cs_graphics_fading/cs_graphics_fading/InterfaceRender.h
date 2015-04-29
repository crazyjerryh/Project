#pragma once
#include "baserender.h"
//利用OpenGL standard 正常绘制，对应Normal toolBar
namespace RenderAlgorithm{
class InterfaceRender :
	public BaseRender
{
public:
	InterfaceRender(void);
	~InterfaceRender(void);
protected:
	void drawModel(QPainter &painter);
	void setglmModel(GLMmodel *glmModel);
	GLMmodel * getglmModel(){return m_glmModel;}
	void setVec(std::vector< std::vector<point> > &vec){};  /*don't realize*/
};

}

