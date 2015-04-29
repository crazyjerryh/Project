#include "ZbufferRender.h"
#include <omp.h>
#include <QPen>
#include <ctime>
#include <fstream>
#include <assert.h>
#include <cstdlib>
#include <QColor>
#include "debugs.h"


namespace RenderAlgorithm{
ZbufferRender::ZbufferRender(void)
{ 
	m_glmGroup=NULL,m_glmModel=NULL;
	m_interactive=new GeometryInteractive::Interactive; /*interactive class*/
}


ZbufferRender::~ZbufferRender(void)
{
	if(m_glmModel)
		delete m_glmModel;

	if(m_glmGroup)
		delete m_glmGroup;   /*destructor*/

	m_zvalue.clear();
}

void ZbufferRender::setglmModel(GLMmodel *glmModel)
{
   	m_glmModel=glmModel;
	if(m_glmModel)
		m_glmGroup=m_glmModel->groups;
}

void ZbufferRender::drawModel(QPainter &painter)
{
	/*set parameter*/
	for(int i=0;i<=m_width;++i)
		m_zvalue.push_back(-0x3fffff);
	/*default background*/
	for(int i=0;i<=m_width;++i)
		m_color.push_back(color(255,255,255));

	this->runBuffer(painter); 
}

void ZbufferRender::setVec(std::vector< std::vector<point> > &vec)
{
	m_vec=vec; /*set triangle of point*/
}

#pragma region private function
GLMmodel* ZbufferRender::projective()
{
	/*for each group*/
	srand(time(NULL));
	GLMgroup *group=m_glmGroup;
#ifdef RUN_TIME_STL
	clock_t t1=clock();  
#endif
	int coreNums=omp_get_num_procs(); /*the number of CPU's core*/
	//coreNums=coreNums;
	double ratio=((double)m_height)/m_width;
	/*set lock*/
	while(group){
		int n_triangles=group->numtriangles; /*get the triangles' number*/
		int perprocs=ceil((double)n_triangles/coreNums);
		omp_lock_t lock;
		omp_init_lock(&lock);

#pragma omp parallel for num_threads(coreNums)
		/**use OMP*/
		for(int i=0;i<coreNums;++i){
			int startNumber=perprocs*i;
			int endnumbr;
			if(i<coreNums-1)
				endnumbr=startNumber+perprocs;
			else
				endnumbr=n_triangles;

			/*each CPU core process the triangles*/
			for(int j=startNumber;j<endnumbr;j++){  /*triangle index*/
				/*process each triangle*/
				std::vector<point> ptTriangle(3);
				std::vector<point> ptOrgin(3);
				GLMtriangle *triangle=&m_glmModel->triangles[m_glmGroup->triangles[j]];
				for(int k=0;k<3;k++){
					GLfloat *pt=&m_glmModel->vertices[3*triangle->vindices[k]]; /*get the point*/
					std::vector<GLfloat> vec(pt,pt+3);

					/*save the original triangle*/
					point *originpt=new point(vec[0],vec[1],vec[2]);
					ptOrgin[k]=*originpt;
					/*set the result*/
					/*because of ORTHO,so we can get the coordinate directly*/
					m_interactive->vecMulmat(m_modelViewArray,vec);

					vec[0]=(vec[0]+2)*m_width/4;  /*from view to window*/
					vec[1]=(2*ratio-vec[1])*m_height/(4*ratio);
					vec[2]=vec[2];  //z value of point
					/*this coordinate value is for debug*/
					point *dotpt=new point(vec[0],vec[1],vec[2]);  /*z-Value is for comparing,origin matrix */
					ptTriangle[k]=*dotpt;

					/*get min and max height*/
					if(m_minHeight>vec[1])
						m_minHeight=vec[1];
					if(m_maxHeight<vec[1])
						m_maxHeight=vec[1];
				}
				omp_set_lock(&lock);
				m_vec.push_back(ptTriangle);

				int tmp;
				if(m_clrtype==0x01){
				   tmp=rand()%80+40;
				   m_tatcolor.push_back(color(tmp,80,10));  /*set each triangle's color*/
				}
				else
				   m_tatcolor.push_back(color(255,0,0));  /*set each triangle's color*/

				
				omp_unset_lock(&lock);
			}
		}
		group=group->next;
		omp_destroy_lock(&lock); /*destroy lock*/
	}

	/*output the triangle,just for debug*/

	return m_glmModel;
}

void ZbufferRender::runBuffer(QPainter &painter)
{
	this->projective();
	/*use openMP to calculate each line*/
	/*for each line ,using OMP parallel to process it*/
	int size=m_vec.size();
	for(int line=m_minHeight;line<=m_maxHeight;++line){
	
		reset();  /*reset the color*/
		int minwidth,maxwidth;
		minwidth=m_width,maxwidth=0;
		for(int i=0;i<size;++i){  //i is triangle's index
			std::vector<point> vec=m_vec[i];
			int x1,x2;
			x1=x2=0;
			if(intersect(vec,line,x1,x2))  /*judge the triangle and line*/
			{
				float vzvalue=calZvalue(vec);
				for(int section=x1;section<=x2;++section){
//					int vzvalue=calZvalue(vec,section,line);
// 					if(m_zvalue[section]<vzvalue){
// 						m_zvalue[section]=vzvalue;  /**/
// 						m_color[section]=m_tatcolor[i];  /*change the value of pixel*/
// 					}
					if(m_zvalue[section]<vzvalue){   //不考虑平面交叉的情况
						m_zvalue[section]=vzvalue;
					    m_color[section]=m_tatcolor[i];
					}
					/*draw the model*/
				}
				//fout<<endl;
				if(minwidth>x1)
					minwidth=x1;

				if(maxwidth<x2)
					maxwidth=x2;
			}
		}
	
		for(int section=minwidth;section<=maxwidth;++section){
			QPen pen(QColor(m_color[section].r,m_color[section].g,m_color[section].b));
			painter.setPen(pen);
			painter.drawPoint(section,line);
		}
	  
	}
	
	/*update:call paintEvent function*/
	return;
}

void ZbufferRender::reset()
{
     /*reset the m_color to background*/
	for(int i=0;i<m_width;++i)
	{
		m_color[i]=color(255,255,255);
		m_zvalue[i]=-0x3ffff;
	}
}

float ZbufferRender::calZvalue(std::vector<point> &triangle)
{
    int size=triangle.size();
	assert(size==3);

	float minz=0xffff;
	for(int i=0;i<size;i++){
		if(triangle[i].z<minz)
			minz=triangle[i].z;
	}

	return minz;  //cal the min z-value 
	return 0;
}

bool ZbufferRender::intersect(std::vector<point> &vec,int line,int &x1,int &x2)
{
	int size=vec.size();
	if(3!=size)
		return false;

	int minHeight,maxHeight;
	minHeight=0xfffff,maxHeight=0;
	for(int k=0;k<3;k++){
		if(vec[k].y<minHeight)
		   minHeight=vec[k].y;
		
		if(vec[k].y>maxHeight)
		   maxHeight=vec[k].y;  /*get the max and min line*/
	}
   
   if(line<minHeight||line>maxHeight)
	   return false;

   /*solve the x1 and x2 value*/
   std::vector<int> ptx;  /*store the intersect point*/
   for(int i=0;i<3;i++){
	   /*for each edge*/
	   point hpt,lpt;
	   int inx1,inx2;
	   inx1=i,inx2=(i+1)%3;
	   hpt=vec[inx1],lpt=vec[inx2];
	   if(hpt.y<lpt.y)
		   std::swap(hpt,lpt);  /*swap the high point with the low point*/
	   
	   if(line<lpt.y||line>hpt.y)
		   continue;

	   int tmp;
	   if(hpt.x!=lpt.x)
	        tmp=(int)(hpt.x-(hpt.y-line)/(hpt.y-lpt.y)*(hpt.x-lpt.x));
	   else
		    tmp=lpt.x;

	   ptx.push_back(tmp);
   }
   if(ptx.size()!=2)
	   return false;

   if(ptx[0]>ptx[1])
	   std::swap(ptx[0],ptx[1]);

   x1=ptx[0],x2=ptx[1];
   ptx.clear();
   return true;
}

#pragma endregion

/*just for debug*/
#pragma region comment debug
void ZbufferRender::draw()
{
	/*draw each triangle*/
	int size=m_vec.size();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0,1.0);
	glPolygonMode(GL_FRONT_AND_BACK,true);
	/*the following function just for debug*/
	glBegin(GL_TRIANGLES);
	for(int i=0;i<size;i++){
		/*because of no normalize,so we don't use normalize*/
		std::vector<point> pt=m_vec[i];
		for(int k=0;k<3;k++){
			glVertex3f(pt[k].x,pt[k].y,0);
		}
	}
	glEnd();
	glDisable(GL_DEPTH_TEST);
	glPolygonOffset(0.0,0.0);
	glDisable(GL_POLYGON_OFFSET_FILL);
}
#pragma endregion

}