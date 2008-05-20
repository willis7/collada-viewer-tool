#include "StdAfx.h"
#include "MyGLClass.h"

MyGLClass::MyGLClass(void)
{
	ready = false;
	drawAxis = true;
}

MyGLClass::MyGLClass(HDC inHDC)
{
	ready = false;
	drawAxis = true;
	init(inHDC);
}

MyGLClass::~MyGLClass(void)
{
	//Dealocate draw list
	int i;
	Object3D* temp;
	for(i=0;i<drawList.size();i++)
	{
		temp = drawList.at(i);
		drawList.pop_back();
		delete(temp);
	}

	wglMakeCurrent(NULL, NULL);

	if (hRC)
		wglDeleteContext(hRC);
	hRC = NULL;

}

int MyGLClass::init(HDC hdc)
{
	
	BOOL						failOpenGLInitialisation;
	int							nPixelFormat;
	PIXELFORMATDESCRIPTOR		pfd;

	//set up view defaults
	vpWidth = 2.0;
	vpHeight = 2.0;
	nearPlane = 2.0;
	farPlane = 5000.0;

	// initialise camera position and direction
	cameraXZ = 0.0;
	cameraY = 0.0;
	cameraR = 5.0;

	failOpenGLInitialisation = TRUE;

	// initialise OpenGL
	this->hDC = hdc;

	ZeroMemory(&pfd,sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	// main steps to setup OpenGL
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
		failOpenGLInitialisation=TRUE;

	if (!SetPixelFormat(hDC, nPixelFormat, &pfd))
		failOpenGLInitialisation=TRUE;

	if (!(hRC = wglCreateContext(hDC)))
		failOpenGLInitialisation=TRUE;
		
	if (!wglMakeCurrent(hDC,hRC))
		failOpenGLInitialisation=TRUE;

	if(!failOpenGLInitialisation) return 1;

	// Initialise OpenGL properties
	// (i) Set 2D orthographic projection
	glMatrixMode(GL_PROJECTION);					// select the projection matrix
	glLoadIdentity();								// reset the projection matrix
	gluPerspective(54.0, vpWidth/vpHeight, 0.01, 5000.0);

	// (ii) Clear all previous transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// (iii) Set rendering properties
	glShadeModel(GL_SMOOTH);						// smooth shading model
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			// specify background color to black
	
	// function for blending colours
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);

	/*******************************************
	Lighting
	********************************************/
	
	glEnable(GL_LIGHTING);

	GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 0.0f, 1.0f, 1.0f }; 
	GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f }; 	

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); 
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1); 

	/*******************************************
	Back face culling
	********************************************/

	glPolygonMode(GL_FRONT, GL_FILL);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);	// cull back faces - do not display them
	glEnable(GL_CULL_FACE); // enable back face culling

	/*******************************************
	Hidden Surface Removal
	********************************************/
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	
	ready = true;	//Win32 may try get a message which requires a 
					// redraw while the setup method is running

	return 0;
}



void MyGLClass::draw(void)
{
	if(ready == false) return;

	// 1. Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// save transformations prior to rendering scene
	glPushMatrix();

		// position camera - viewing transformations
		glTranslatef(0.0, 0.0, -cameraR);
		glRotatef(cameraXZ, 1.0, 0.0, 0.0);
		glRotatef(cameraY, 0.0, 1.0, 0.0);
		
		if(drawAxis)
		{
			// Draw principle axes
			glPushMatrix();
				glScalef(0.65f, 1.0, 1.0);

				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(1.0, 0.0, 0.0);

					glVertex3f(1.025f, 0.025f, 0.0);
					glVertex3f(1.075f, 0.075f, 0.0);

					glVertex3f(1.025f, 0.075f, 0.0);
					glVertex3f(1.075f, 0.025f, 0.0);
				glEnd();
			glPopMatrix();
			

			glPushMatrix();
				glScalef(1.0, 0.65f, 1.0);
				glColor3f(0.0, 1.0, 0.0);
				glBegin(GL_LINES);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.0, 1.0, 0.0);

					glVertex3f(-0.075f, 1.075f, 0.0);
					glVertex3f(-0.05f, 1.05f, 0.0);

					glVertex3f(-0.025f, 1.075f, 0.0);
					glVertex3f(-0.075f, 1.025f, 0.0);
				glEnd();
			glPopMatrix();
			

			glPushMatrix();
				glScalef(1.0, 1.0, 0.65f);
				glColor3f(0.0, 0.0, 1.0);
				glBegin(GL_LINES);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 1.0);

					glVertex3f(0.025f, 0.075f, 1.0);
					glVertex3f(0.075f, 0.075f, 1.0);

					glVertex3f(0.075f, 0.075f, 1.0);
					glVertex3f(0.025f, 0.025f, 1.0);

					glVertex3f(0.025f, 0.025f, 1.0);
					glVertex3f(0.075f, 0.025f, 1.0);
				glEnd();
			glPopMatrix();
		}
		
		//Draw my draw list
		int i;
		for(i=0;i < drawList.size();i++) //.size() returns the number	
										// of elemnts in the vector
		{
			drawList.at(i)->draw(); //.at(i) returns a poitner 
									//to the object at i in the vector
		}

 glPopMatrix();
}


bool MyGLClass::isReady(void)
{
	return ready;
}

void MyGLClass::setReady(bool b)
{
	ready = b;
}

void MyGLClass::dispCameraXZ(float f)
{
	cameraXZ += f;
}

void MyGLClass::dispCameraY(float f)
{
	cameraY += f;
}

void MyGLClass::dispCameraR(float f)
{
	cameraR += f;
}

void MyGLClass::setAxis(bool b)
{
	drawAxis = b;
}
	
bool MyGLClass::getAxis(void)
{
	return drawAxis;
}

void MyGLClass::add(Object3D *obj)
{
	drawList.push_back(obj);
}

void MyGLClass::remove()
{
	drawList.pop_back();
}

void MyGLClass::setDrawList(vector<Object3D*> dl)
{
	drawList = dl;
}

vector<Object3D*> MyGLClass::getDrawList(void)
{
	return drawList;
}