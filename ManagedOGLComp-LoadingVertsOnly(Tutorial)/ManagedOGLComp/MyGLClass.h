#pragma once

//#include <windows.h> //for HDC
#include <vector>
#include "Object3D.h" //include this 
#include "myGl.h" //Windows.h and opengl includes moved to seperate header 
				// this prevents issues surrounding include order. 

using namespace std; //this avoids using std::vector can use vector

class MyGLClass
{
private:
	bool						ready;
	HDC							hDC;// windows device context
	HGLRC						hRC;// openGL rendering context
	bool						drawAxis;

	// define globals to model the main viewport
	float			vpWidth;
	float			vpHeight;
	float			nearPlane;
	float			farPlane;

	// camera variables (for now model camera that simply rotates around the origin)
	float			cameraXZ, cameraY; // angles of orientation
	float			cameraR; //distance to origin

	//List of shapes to be drawn, the standard library template vector is used
	//this is a dynamic array making handeling of an arbritrarily long draw list simple.
	vector<Object3D*>	drawList;
	//Note: A scene graph of some sort would probably be used in game to identify objects for
	//rendering given a current camera position. 
	

public:
	MyGLClass(HDC hdc);
	MyGLClass(void);
	~MyGLClass(void);

	int init(HDC hdc);
	void draw(void); //this function effectivly calls the draw command on
					// each of the objects in the drawList vector. 

	void setDrawList(vector<Object3D*>);
	vector<Object3D*> getDrawList(void);

	bool isReady(void);
	void setReady(bool);

	void dispCameraXZ(float);
	void dispCameraY(float);
	void dispCameraR(float);

	void setAxis(bool);
	bool getAxis(void);

	void add(Object3D *obj); //basically does a push
	void remove();			// and a pop
	
};
