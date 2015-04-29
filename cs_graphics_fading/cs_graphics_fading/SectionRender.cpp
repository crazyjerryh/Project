#include "SectionRender.h"
#include <omp.h>
#include "SegmentTree.h"
#include <algorithm>
#include <QPen>
#include <cmath>
#include <cassert>

namespace RenderAlgorithm{
SectionRender::SectionRender(void)
{
	m_interactive=new GeometryInteractive::Interactive;
}


SectionRender::~SectionRender(void)
{
}

void SectionRender::drawModel(QPainter &painter)
{
     this->runBuffer(painter);	
}

#pragma region GLOBAL FUNCTION
typedef struct sideNode_tag{
	GLfloat xValue;
	int sectionInx;
	int type;
	color clr;
	struct sideNode_tag(GLfloat _xValue,int _sInx,int _type,color _clr){
		xValue=_xValue,sectionInx=_sInx,type=_type;
		clr=_clr;
	}
	struct sideNode_tag(){}
}sideNode;


typedef struct sideZvalue_tag{
	float zValue;
	float zDelta;
	struct sideZvalue_tag(float _z,float _zd):zValue(_z),zDelta(_zd){}
	struct sideZvalue_tag(){}
}sideZvalue;


int cmp(const sideNode &a,const sideNode &b)
{
	return a.xValue<b.xValue;  //min to max
}

#pragma endregion

#pragma region PRIVATE FUCNTION
void SectionRender::setClr()
{
	for(int i=0;i<m_glmGroup->numtriangles;i++){
	   int tmp;
	   if(m_clrtype==0x01){
		  tmp=rand()%100+40;
		  m_tatcolor.push_back(color(tmp,80,10));  /*set each triangle's color*/
	   }
	   else
          m_tatcolor.push_back(color(255,0,0));	
	}
}

void SectionRender::runBuffer(QPainter &painter)
{
	std::vector<sideNode> sectionNode;
	std::vector<sideZvalue> sectionZvalue;

	using namespace RenderAlgorithm;
	Zbuffer* buffer = buildTables();

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
	segmentTree *root=NULL;   //segment tree
	create(root,0,m_width);
	for(int scanIndex = m_maxHeight; scanIndex >= m_minHeight; --scanIndex) {
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

		ActiveEdgeList::iterator aEdgeIte = activeEdgeList.begin();
		int objCount=0; 
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

			leftXValue = (leftXValue > -1) ? leftXValue : 0;
			rightXValue = (rightXValue > (windowWidth - 1)) ? (windowWidth - 1) : rightXValue;
			
			//save the pair of edge
			int index=aEdge.pageIndex;
		    sectionNode.push_back(sideNode(leftXValue,objCount,1,color(m_tatcolor[index].r,m_tatcolor[index].g,m_tatcolor[index].b)));
			sectionNode.push_back(sideNode(rightXValue,objCount,2,color(m_tatcolor[index].r,m_tatcolor[index].g,m_tatcolor[index].b)));
			
			sectionZvalue.push_back(sideZvalue(zValue,aEdge.deltaZXValue)); //objCount 对应sectionZvalue的位置
			++objCount;

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

		//use segment tree to divide section,main body of algorithm
	    int secNums=sectionNode.size();
		for(int i=0;i<secNums;i+=2){
			int l=sectionNode[i].xValue;
			int r;
			if(i<secNums-1)
			    r=sectionNode[i+1].xValue;
			else
				continue;

			int secInx=sectionNode[i].sectionInx; //section's index
			color clr=sectionNode[i].clr;
			insert(root,l,r,sectionZvalue[secInx].zValue,sectionZvalue[secInx].zDelta,clr);
		}

		//draw segment Tree
		drawTree(root,painter,-1000,scanIndex);
		clear(root);
		sectionNode.clear(),sectionZvalue.clear();  //clear the memory 
	}
}

RenderAlgorithm::Zbuffer* SectionRender::buildTables()
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

#pragma endregion
}
