#pragma once
#include "baserender.h"
//����OpenGL standard �������ƣ���ӦNormal toolBar
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

