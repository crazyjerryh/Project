#ifndef _SCENE_BUILD_H_
#define _SCENE_BUILD_H_
#include <gl/GL.h>
#include <gl/glut.h>
#include <gl/GLU.H>

/*this class is using for OpenGL's scene Interface Building*/
class SceneBuilding{
public:
	SceneBuilding(){}
	~SceneBuilding(){}
public:
	void SetLight()
	{
		glEnable(GL_DEPTH_TEST);
		// Default mode
		glPolygonMode(GL_FRONT,GL_FILL);
		glPolygonMode(GL_BACK,GL_FILL);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_NORMALIZE);

		// Lights, material properties
		GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.0f, 1.0f};
		GLfloat	diffuseProperties[]  = {0.8f, 0.2f, 0.2f, 1.0f};
		GLfloat	specularProperties[] = {0.0f, 0.8f, 0.2f, 1.0f};

		glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
		glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
    
	void setMatrial()
	{
	    //Default matrial of 
	}
};

#endif