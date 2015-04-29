#pragma once
#include "baserender.h"
#include "ScanDataStructure.h"
#include "interactive.h"
//ɨ�����㷨ʵ����
namespace RenderAlgorithm{
class ScanRender :
	public BaseRender
{
public:
	ScanRender(void);
	~ScanRender(void);
public:
	virtual void drawModel(QPainter &painter);
	virtual void setglmModel(GLMmodel *glmModel);
	virtual GLMmodel *getglmModel();
	virtual void setVec(std::vector< std::vector<point> > &vec);
private:
	RenderAlgorithm::Zbuffer* buildTable();
	void runBuffer(QPainter &painter);
	void reset();  //set default mode
	void setClr();
private:
	GeometryInteractive::Interactive *m_interactive;
	std::vector<float> m_zvalue;  //z-value
	std::vector<color> m_clr;    //color of each pixel
};
}