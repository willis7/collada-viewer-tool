#include ".\myopenglclass.h"
#include "stdafx.h"

MyOpenGLClass::MyOpenGLClass(void)
{
	ready = false;

	Points = (float**) malloc(8*sizeof(float*));
		Points[0] = (float*) malloc(3*sizeof(float*));
		Points[1] = (float*) malloc(3*sizeof(float*));
		Points[2] = (float*) malloc(3*sizeof(float*));
		Points[3] = (float*) malloc(3*sizeof(float*));
		Points[4] = (float*) malloc(3*sizeof(float*));
		Points[5] = (float*) malloc(3*sizeof(float*));
		Points[6] = (float*) malloc(3*sizeof(float*));
		Points[7] = (float*) malloc(3*sizeof(float*));

		Points[0][0] = 1;
		Points[0][1] = 1;
		Points[0][2] = 1;

		Points[1][0] = 0.5;
		Points[1][1] = 1;
		Points[1][2] = 0;

		Points[2][0] = -0.5;
		Points[2][1] = 0;
		Points[2][2] = 0;

		Points[3][0] = -1;
		Points[3][1] = -1;
		Points[3][2] = -1;

		Points[4][0] = -1;
		Points[4][1] = -1;
		Points[4][2] = -1;

		Points[5][0] = -1;
		Points[5][1] = 0;
		Points[5][2] = -0.5;

		Points[6][0] = -0.5;
		Points[6][1] = -0.5;
		Points[6][2] = -0.5;
		
		Points[7][0] = 0;
		Points[7][1] = -0.5;
		Points[7][2] = 0.5;
}

MyOpenGLClass::MyOpenGLClass(HDC inHDC)
{
	ready = false;
	init(inHDC);

	//This is dirty but I had to put it somewhere!
	Points = (float**) malloc(8*sizeof(float*));
		Points[0] = (float*) malloc(3*sizeof(float*));
		Points[1] = (float*) malloc(3*sizeof(float*));
		Points[2] = (float*) malloc(3*sizeof(float*));
		Points[3] = (float*) malloc(3*sizeof(float*));
		Points[4] = (float*) malloc(3*sizeof(float*));
		Points[5] = (float*) malloc(3*sizeof(float*));
		Points[6] = (float*) malloc(3*sizeof(float*));
		Points[7] = (float*) malloc(3*sizeof(float*));

		Points[0][0] = -1;
		Points[0][1] = 1;
		Points[0][2] = 0;

		Points[1][0] = -0.75;
		Points[1][1] = 0.5;
		Points[1][2] = 0;

		Points[2][0] = -0.5;
		Points[2][1] = 0;
		Points[2][2] = 0;

		Points[3][0] = 0;
		Points[3][1] = -0.5;
		Points[3][2] = 0;

		Points[4][0] = 0.25;
		Points[4][1] = 0;
		Points[4][2] = 0;

		Points[5][0] = 0.5;
		Points[5][1] = 0;
		Points[5][2] = 0;

		Points[6][0] = 0.75;
		Points[6][1] = 0.5;
		Points[6][2] = 0;
		
		Points[7][0] = 1;
		Points[7][1] = 1;
		Points[7][2] = 0;
}

MyOpenGLClass::~MyOpenGLClass(void)
{
	wglMakeCurrent(NULL, NULL);

	if (hRC)
		wglDeleteContext(hRC);
	hRC = NULL;

	free(Points);
	free(Points1);
}

int MyOpenGLClass::init(HDC hdc)
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
	cameraR = 15.0;

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
	Back face culling
	********************************************/
	glPolygonMode(GL_FRONT, GL_FILL);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK); // cull back faces - do not display them
	glEnable(GL_CULL_FACE); // enable back face culling

	/*******************************************
	Hidden Surface Removal
	********************************************/
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	
	ready = true; //Win32 may try get a message which requires a 
				// redraw while the setup method is running

	return 0;
}



void MyOpenGLClass::draw(void)
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

		
		//Render Bezier Curve
		
		glColor3f(1.0,1.0,0.0);

		unsigned int LOD=100;

		//drawBezierSegment(Points, 0, LOD);
		///drawBezierSegment(Points, 4, LOD);
			
		glColor3f(1.0,1.0,1.0);

		//Render Cubic Spline Curve

		glColor3f(1.0f,0.0f,1.0f);


		drawCathmullRomSegment(Points,0,LOD,0.5);
		drawCathmullRomSegment(Points,1,LOD,0.5);
		drawCathmullRomSegment(Points,2,LOD,0.5);
		drawCathmullRomSegment(Points,3,LOD,0.5);
		drawCathmullRomSegment(Points,4,LOD,0.5);

		glColor3f(1.0f,0.4f,0.3f);

		drawCathmullRomSegment2(Points,0,LOD,0.5);
		drawCathmullRomSegment2(Points,1,LOD,0.5);
		drawCathmullRomSegment2(Points,2,LOD,0.5);
		drawCathmullRomSegment2(Points,3,LOD,0.5);
		drawCathmullRomSegment2(Points,4,LOD,0.5);
			

		//draw the points

		glColor3f(0.0f,1.0f,0.0f);
		glPointSize(2.0f);

		glBegin(GL_POINTS);
		for(int i =0; i<8;i++)
		{
			glVertex3f(Points[i][0],Points[i][1],Points[i][2]);
		}
		glEnd();

		

 glPopMatrix();
}


bool MyOpenGLClass::isReady(void)
{
	return ready;
}

void MyOpenGLClass::setReady(bool b)
{
	ready = b;
}

void MyOpenGLClass::dispCameraXZ(float f)
{
	cameraXZ += f;
}

void MyOpenGLClass::dispCameraY(float f)
{
	cameraY += f;
}

void MyOpenGLClass::dispCameraR(float f)
{
	cameraR += f;
}

//In these functions t is the tention and u the distance
float MyOpenGLClass::cathmullBlend1(float  t, float u)
{
	float u2 = u*u;
	float u3 = u*u*u;
	return (-t*u)+(2*t*u2)-(t*u3);
}

float MyOpenGLClass::cathmullBlend2(float  t, float u)
{
	float u2 = u*u;
	float u3 = u*u*u;
	return 1+(t-3)*u2+((2-t)*u3);
}

float MyOpenGLClass::cathmullBlend3(float  t, float u)
{
	float u2 = u*u;
	float u3 = u*u*u;
	return (t*u)+((3-2*t)*u2) + ((t-2)*u3);
}

float MyOpenGLClass::cathmullBlend4(float  t, float u)
{
	float u2 = u*u;
	float u3 = u*u*u;
	return (-t*u2)+(t*u3);
}

//takes 4 control points at a time. 
//takes an array of control points and an offset at which to start. 
void MyOpenGLClass::drawBezierSegment(float** controlPoints, int offset, unsigned int LOD)
{
	// we will draw lots of little lines to make our curve
		glBegin(GL_LINE_STRIP);

		for(int i=0;i!=LOD;++i) 
		{

			// use the parametric time value 0 to 1
			float t = (float)i/(LOD-1);

			// nice to pre-calculate 1.0f-t because we will need it frequently
			float it = 1.0f-t;

			// calculate blending functions
			float b0 = t*t*t;
			float b1 = 3*t*t*it;
			float b2 = 3*t*it*it;
			float b3 =  it*it*it;

			// calculate the x,y and z of the curve point by summing
			// the Control vertices weighted by their respective blending
			// functions
			//
			float x = b0*controlPoints[offset+0][0] +
					b1*controlPoints[offset+1][0] +
					b2*controlPoints[offset+2][0] +
					b3*controlPoints[offset+3][0] ;

			float y = b0*controlPoints[offset+0][1] +
					b1*controlPoints[offset+1][1] +
					b2*controlPoints[offset+2][1] +
					b3*controlPoints[offset+3][1] ;

			float z = b0*controlPoints[offset+0][2] +
					b1*controlPoints[offset+1][2] +
					b2*controlPoints[offset+2][2] +
					b3*controlPoints[offset+3][2] ;

			// specify the point
			glVertex3f( x,y,z );
		}
		glEnd();

}


//takes an array of control points and an offset at which to start. 
void MyOpenGLClass::drawCathmullRomSegment(float** controlPoints, int offset, unsigned int LOD, float tention)
{
	// we will draw lots of little lines to make our curve
		glBegin(GL_LINE_STRIP);

		for(int i=0;i!=LOD;++i) 
		{

			// use the parametric time value 0 to 1
			float t = (float)i/(LOD-1);

			// nice to pre-calculate 1.0f-t because we will need it frequently
			float it = 1.0f-t;

			// calculate blending functions
			// blending funcitons written as funitons

			// calculate the x,y and z of the curve point by summing
			// the Control vertices weighted by their respective blending
			// functions
			//
			float x = cathmullBlend1(tention,t)*controlPoints[offset+0][0] +
					cathmullBlend2(tention,t)*controlPoints[offset+1][0]+
					cathmullBlend3(tention,t)*controlPoints[offset+2][0]+
					cathmullBlend4(tention,t)*controlPoints[offset+3][0];

			float y = cathmullBlend1(tention,t)*controlPoints[offset+0][1] +
					cathmullBlend2(tention,t)*controlPoints[offset+1][1] +
					cathmullBlend3(tention,t)*controlPoints[offset+2][1] +
					cathmullBlend4(tention,t)*controlPoints[offset+3][1] ;

			float z = cathmullBlend1(tention,t)*controlPoints[offset+0][2] +
					cathmullBlend2(tention,t)*controlPoints[offset+1][2] +
					cathmullBlend3(tention,t)*controlPoints[offset+2][2] +
					cathmullBlend4(tention,t)*controlPoints[offset+3][2];

			// specify the point
			glVertex3f( x,y,z );
		}
		glEnd();
}

void MyOpenGLClass::drawCathmullRomSegment2(float** controlPoints, int offset, unsigned int LOD, float tention)
{
	// we will draw lots of little lines to make our curve
		glBegin(GL_LINE_STRIP);

		for(int i=0;i!=LOD;++i) 
		{

			// use the parametric time value 0 to 1
			float t = (float)i/(LOD-1);

			// nice to pre-calculate 1.0f-t because we will need it frequently
			float it = 1.0f-t;

			// calculate blending functions
			// blending funcitons written as funitons

			// calculate the x,y and z of the curve point by summing
			// the Control vertices weighted by their respective blending
			// functions

			/*
			out.x = 0.5f * ( ( 2.0f * p1.x ) +
		( -p0.x + p2.x ) * t +
		( 2.0f * p0.x - 5.0f * p1.x + 4 * p2.x - p3.x ) * t2 +
		( -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x ) * t3 );
			*/
			//
			float t2 = t*t;
			float t3 = t*t*t;

			float x = 0.5f * ( (2.0f * controlPoints[offset+1][0]) +
							 (-controlPoints[offset+0][0] + controlPoints[offset+2][0]) * t +
							 (2.0f * controlPoints[offset+0][0] -5.0f * controlPoints[offset+1][0] +  4 * controlPoints[offset+2][0] - controlPoints[offset+3][0]) * t2 +
							 (-controlPoints[offset+0][0] + 3.0f * controlPoints[offset+1][0] - 3.0f * controlPoints[offset+2][0] + controlPoints[offset+3][0]) *t3);
				

			float y = 0.5f * ( (2.0f * controlPoints[offset+1][1]) +
							 (-controlPoints[offset+0][1] + controlPoints[offset+2][1]) * t +
							 (2.0f * controlPoints[offset+0][1] -5.0f * controlPoints[offset+1][1] +  4 * controlPoints[offset+2][1] - controlPoints[offset+3][1]) * t2 +
							 (-controlPoints[offset+0][1] + 3.0f * controlPoints[offset+1][1] - 3.0f * controlPoints[offset+2][1] + controlPoints[offset+3][1]) *t3);

			float z = 0.5f * ( (2.0f * controlPoints[offset+1][2]) +
							 (-controlPoints[offset+0][2] + controlPoints[offset+2][2]) * t +
							 (2.0f * controlPoints[offset+0][2] -5.0f * controlPoints[offset+1][2] +  4 * controlPoints[offset+2][2] - controlPoints[offset+3][2]) * t2 +
							 (-controlPoints[offset+0][2] + 3.0f * controlPoints[offset+1][2] - 3.0f * controlPoints[offset+2][2] + controlPoints[offset+3][2]) *t3);

			// specify the point
			glVertex3f( x,y,z );
		}
		glEnd();
}