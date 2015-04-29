#include "ScanRender.h"
#include <omp.h>
#include <cassert>
using namespace std;

namespace RenderAlgorithm{
ScanRender::ScanRender(void)
{
	m_interactive=new GeometryInteractive::Interactive;
}

ScanRender::~ScanRender(void)
{
}

#pragma region overload function
void ScanRender::setglmModel(GLMmodel *glmModel)
{
	m_glmModel=glmModel;
	if(m_glmModel)
		m_glmGroup=m_glmModel->groups;
}

GLMmodel *ScanRender::getglmModel()
{
	return m_glmModel;
}

void ScanRender::drawModel(QPainter &painter)
{
	/*from the up scanning line to the bottom scanning line*/
	for(int i=0;i<m_width;++i)
		m_zvalue.push_back(-0x3ffff);
	
	for(int i=0;i<m_width;++i)
		m_clr.push_back(color(255,255,255));

	this->runBuffer(painter);
}

void ScanRender::setVec(std::vector< std::vector<point> > &vec)
{
     m_vec=vec;
}
#pragma endregion

#pragma region private function

void ScanRender::reset()
{
	//reset vector of color and z-value
    for(int i=0;i<m_width;++i)
	{
		m_zvalue[i]=-0x3ffff;
		m_clr[i]=color(255,255,255);
	}
}

void ScanRender::setClr()
{
	for(int i=0;i<m_glmGroup->numtriangles;i++){
	   int tmp;
	   if(m_clrtype==0x01){
		 tmp=rand()%80+40;
	     m_tatcolor.push_back(color(tmp,80,10));  /*set each triangle's color*/
	   }
	   else
		 m_tatcolor.push_back(color(255,0,0));
	}
}

RenderAlgorithm::Zbuffer* ScanRender::buildTable()
{
	RenderAlgorithm::Model model;

	int w = m_width;
	int h = m_height;
	double ratio=(double)h/w;

	//the number of current CPU cores
	int numCores =omp_get_num_procs();

	GLMgroup *group = m_glmModel->groups;
	int objCount = 0;

	//Loop all the groups, each group is saved in a mytype::Object structure
	while(group) {
		model.push_back(RenderAlgorithm::Object());

		int number = group->numtriangles;
		int perCoreNumber = ceil((double)(number) / numCores);
		omp_lock_t lock;
		omp_init_lock(&lock);

		/*
		*Loop all the triangles
		*make full use of all the cores of CPU
		*each core run a range of triangles rotation
		*/
#pragma omp parallel for num_threads(numCores)
		for(int coreId = 0; coreId < numCores; coreId++) {
			int startNumber, endNumber;
			startNumber = coreId * perCoreNumber;

			//if this is not the last core
			if(coreId < numCores - 1)
				endNumber = startNumber + perCoreNumber;
			else
				endNumber = number;

			for(int i = startNumber; i < endNumber; ++i)
			{
				RenderAlgorithm::Polygon polygon;

				//Loop the 3 point in a triangle
				for(int k = 0; k < 3; ++k)
				{
					//Get the triangle
					GLMtriangle* triangle = &(m_glmModel->triangles[(group->triangles[i])]);

					//Get the 3-D point
					GLfloat * pointArray = &m_glmModel->vertices[3 * triangle->vindices[k]];
					std::vector<GLfloat> vec(pointArray, pointArray + 3);

					//Multiple moeView and projection matrix, and convert the the screen coordinate
					m_interactive->vecMulmat(m_modelViewArray, vec);
					//vecLMultiplyMat(vec, matProject);
					vec[0]=(vec[0]+2)*w/4;  /*from view to window*/
					vec[1]=(2*ratio-vec[1])*h/(4*ratio);  

					//Save the result
					polygon.push_back(RenderAlgorithm::Point3D(vec[0], vec[1], vec[2]));

					if(vec[1] > m_maxHeight)
						m_maxHeight = vec[1];
					if(vec[1] < m_minHeight)
						m_minHeight = vec[1];
				}
				omp_set_lock(&lock);
				model[objCount].push_back(polygon);
				omp_unset_lock(&lock);
			}
		}

		//Get next object
		++objCount;
		group = group->next;

		omp_destroy_lock(&lock);
	}

	//return NULL;  /*for debug*/
	return new RenderAlgorithm::Zbuffer(model, m_maxHeight + 1);
}

void ScanRender::runBuffer(QPainter &painter)
{
	using namespace RenderAlgorithm;
    Zbuffer* buffer = buildTable();

	setClr();
	int windowWidth = m_width;
	static const GLfloat BACKGROUNDVALUE = -100000.0;
	std::vector<GLfloat> zValueBuffer(windowWidth, BACKGROUNDVALUE);

	const PageTable &pageTable = buffer->getPageTable();
	const EdgeTable &edgeTable = buffer->getEdgeTable();

	ActivePageList activePageList;
	ActiveEdgeList activeEdgeList;

	m_minHeight = (m_minHeight > 0) ? m_minHeight : 0;

	//Loop the scan line from the highest value to the lowest value
	for(int scanIndex = m_maxHeight; scanIndex >= m_minHeight; --scanIndex) {

		reset();
		int minwidth=windowWidth;
		int maxwidth=0;

		const PageList &pageList = pageTable[scanIndex];
		//If current scan line has a new page, put the page and its new edge into tables
		if(pageList.size() != 0) {
			//get the page of current scan line
			for(size_t pageIndex = 0; pageIndex < pageList.size(); ++pageIndex) {
				const Page& page = pageList[pageIndex];
				activePageList.push_back(page);

				//get the two edges of the active page
				const PageEdgeMap &edgeMap = edgeTable[scanIndex];
				PageEdgeMap::const_iterator ite = edgeMap.find(page.index);

				const EdgeList &edgeList = (*ite).second;
				assert(edgeList.size() == 2);

				Edge firstEdge = edgeList[0];
				Edge secondEdge = edgeList[1];
				if(firstEdge.upperXValue > secondEdge.upperXValue)
					std::swap(firstEdge, secondEdge);
				else if(firstEdge.upperXValue == secondEdge.upperXValue
					&& firstEdge.deltaX > secondEdge.deltaX)
					std::swap(firstEdge, secondEdge);

				//make the active edge
				ActiveEdge aEdge(firstEdge, secondEdge, page, scanIndex);

				//put active edge into table
				activeEdgeList.push_back(aEdge);
			}
		}

		//loop all the active pages
		ActivePageList::iterator aPageIte = activePageList.begin();
		while(aPageIte != activePageList.end()) {
			Page &aPage = *aPageIte;

			//the page is not active anymore
			if(aPage.lineCount < 0) {
				aPageIte = activePageList.erase(aPageIte);
				continue;
			}
			aPage.lineCount--;
			aPageIte++;
		}//end loop all the active pages


		//loop all the active edges
		ActiveEdgeList::iterator aEdgeIte = activeEdgeList.begin();
		while(aEdgeIte != activeEdgeList.end()) {
			ActiveEdge &aEdge = *aEdgeIte;

			//compare the zValue of all the activeEdge with the zValueBuffer
			float zValue = aEdge.zValue;

			//first make sure the x value is in the screen
			int leftXValue = aEdge.leftXValue;
			int rightXValue = aEdge.rightXValue;
			if(leftXValue > (windowWidth - 1) || rightXValue < 0) {
				++aEdgeIte;
				continue;
			}
			
			if(leftXValue<minwidth)
				 minwidth=leftXValue;

			if(rightXValue>maxwidth)
				 maxwidth=rightXValue;

			leftXValue = (leftXValue > -1) ? leftXValue : 0;
			rightXValue = (rightXValue > (windowWidth - 1)) ? (windowWidth - 1) : rightXValue;

			for(int xValue = leftXValue; xValue <= rightXValue; ++xValue) {
				int index=aEdge.pageIndex;
				if(zValue > zValueBuffer[xValue]) {
					zValueBuffer[xValue] = zValue;
					m_clr[xValue]=m_tatcolor[index];
				}
				zValue += aEdge.deltaZXValue;
			}

			//calculate para for next scan line
			--aEdge.leftLineCount;
			--aEdge.rightLineCount;

			if(aEdge.leftLineCount < 0 && aEdge.rightLineCount < 0) {
				aEdgeIte = activeEdgeList.erase(aEdgeIte);
				continue;
			}

			if(aEdge.leftLineCount < 0 || aEdge.rightLineCount < 0) {
				const PageEdgeMap &edgeMap = edgeTable[scanIndex];
				PageEdgeMap::const_iterator ite = edgeMap.find(aEdge.pageIndex);
				if(ite == edgeMap.end()) {
					aEdgeIte = activeEdgeList.erase(aEdgeIte);
					continue;
				}

				const EdgeList &edgeList = (*ite).second;
				assert(edgeList.size() == 1);

				Edge replaceEdge = edgeList[0];

				if(aEdge.leftLineCount < 0) {
					//assert(replaceEdge.upperXValue <= aEdge.rightXValue)
					aEdge.insertLeftEdge(replaceEdge, scanIndex);
				} else {
					//assert(replaceEdge.upperXValue >= aEdge.leftXValue);
					aEdge.insertRightEdge(replaceEdge, scanIndex);
				}
			}

			aEdge.leftXValue += aEdge.leftDeltaX;
			aEdge.rightXValue += aEdge.rightDeltaX;

			aEdge.zValue += aEdge.deltaZYValue + aEdge.leftDeltaX * aEdge.deltaZXValue;

			++aEdgeIte;
		}//end loop all the active edges

		for(int xValue = minwidth; xValue < maxwidth; ++xValue) {
			float zValue =zValueBuffer[xValue];
			if(fabs(zValue - BACKGROUNDVALUE) > 1e-4) {
				//int colorValue = (zValue + 0.866f) / 1.732f * 255;
				painter.setPen(QColor(m_clr[xValue].r,m_clr[xValue].g,m_clr[xValue].b));
				painter.drawPoint(xValue, scanIndex);
			}
		}

	
		zValueBuffer.assign(windowWidth, BACKGROUNDVALUE);
	}//End Loop for scan line

}

#pragma endregion
}
