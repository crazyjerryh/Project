#include "InterfaceRender.h"
#include <ctime>
#include <cstdlib>

/*use OPENGL interface to render the model*/
namespace RenderAlgorithm{
InterfaceRender::InterfaceRender(void)
{
	m_fnormalize=1;
}

InterfaceRender::~InterfaceRender(void)
{
	if(m_glmModel)
	     delete m_glmModel;

	if(m_glmGroup)
		 delete m_glmGroup;
}

void InterfaceRender::setglmModel(GLMmodel *glmModel)
{
	m_glmModel=glmModel;
	if(m_glmModel)
		m_glmGroup=m_glmModel->groups;
}

void InterfaceRender::drawModel(QPainter &painter)
{
	/*use the model to */
	GLMgroup *group=m_glmGroup;
	GLMtriangle *triangle;

	if(!m_glmModel->numnormals)
		m_fnormalize=false;
	
	while(group)
	{
		/*get the group's triangles*/
		int n_triangles=group->numtriangles;
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.1,1.0);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			
	    glBegin(GL_TRIANGLES);
		//set rand send
		srand(time(NULL));
		for(int i=0;i<n_triangles;++i){
			glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);  //set material 
			color clr;
			if(m_clrtype==0x01)
			{
				clr.r=0;
				clr.g=60;  //standard GL can't random color. don't know it's reason
				clr.b=0;
			}
			else
			{
				clr.r=255;
				clr.g=0;
				clr.b=0;
			}
			/*get the triangle's index*/
			triangle=&(m_glmModel->triangles[group->triangles[i]]);  /*the group's triangles is the triangle's index
																	 but the model's triangles structure is the triangle
																	 GLMtriangle*/
			/*from the triangle structure,we can get the vindices*/
 			if(m_fnormalize)
 				 glNormal3fv(&m_glmModel->normals[3 * triangle->nindices[0]]);
			
			glColor3d(clr.r,clr.g,clr.b);	
			glVertex3fv(&m_glmModel->vertices[3*triangle->vindices[0]]);  /*vindices store x,y,z value ,so multiply 3*/
			if(m_fnormalize)
				 glNormal3fv(&m_glmModel->normals[3*triangle->nindices[1]]);
			glColor3d(clr.r,clr.g,clr.b);	
			glVertex3fv(&m_glmModel->vertices[3*triangle->vindices[1]]);
			if(m_fnormalize)
				 glNormal3fv(&m_glmModel->vertices[3*triangle->nindices[2]]);
			glColor3d(clr.r,clr.g,clr.b);	
			glVertex3fv(&m_glmModel->vertices[3*triangle->vindices[2]]);
			glDisable(GL_COLOR_MATERIAL);
		}
		glEnd();
		//glPolygonOffset(0.0,0.0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		group=group->next;
	}
}
}
