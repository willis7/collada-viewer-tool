#pragma once
#include <windows.h> //for HDC
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>





class MyOpenGLClass
{
private:
	bool ready;
	HDC							hDC;// windows device context
	HGLRC						hRC;// openGL rendering context
	
	// define globals to model the main viewport
	float			vpWidth;
	float			vpHeight;
	float			nearPlane;
	float			farPlane;

	// camera variables (for now model camera that simply rotates around the origin)
	float			cameraXZ, cameraY; // angles of orientation
	float			cameraR; //distance to origin

	//For spline drawing stuff
	float** Points;
	float** Points1;

	//spline functions
	float cathmullBlend1(float  t, float u);
	float cathmullBlend2(float  t, float u);
	float cathmullBlend3(float  t, float u);
	float cathmullBlend4(float  t, float u);

public:
	MyOpenGLClass(HDC hdc);
	MyOpenGLClass(void);
	~MyOpenGLClass(void);

	int init(HDC hdc);
	void draw(void);

	bool isReady(void);
	void setReady(bool);

	void dispCameraXZ(float);
	void dispCameraY(float);
	void dispCameraR(float);

	void drawBezierSegment(float** controlPoints, int offset, unsigned int LOD);
	void drawCathmullRomSegment(float** controlPoints, int offset, unsigned int LOD, float tention);
	void drawCathmullRomSegment2(float** controlPoints, int offset, unsigned int LOD, float tention);
};

