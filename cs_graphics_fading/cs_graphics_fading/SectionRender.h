#pragma once
#include "baserender.h"
#include "interactive.h" 
#include "ScanDataStructure.h" 
//区间扫描线算法实现类
namespace RenderAlgorithm{
	/*section algorithm. */
class SectionRender :
	public BaseRender
{
public:
	SectionRender(void);
	~SectionRender(void);
private:
	GeometryInteractive::Interactive *m_interactive;
	std::vector<float> m_zvalue;
	std::vector<color> m_clr;
public:
	void drawModel(QPainter &painter);
	void setglmModel(GLMmodel *glmModel){
		m_glmModel=glmModel;
		if(m_glmModel)
			m_glmGroup=m_glmModel->groups;
	}

	GLMmodel *getglmModel(){
		return m_glmModel;
	}

	void setVec(std::vector< std::vector<point> > &vec)
	{
		m_vec=vec;
	}
private:
    void runBuffer(QPainter &painter);  /**/
	RenderAlgorithm::Zbuffer* buildTables();
	void setClr();
	void reset(){}
};
}

