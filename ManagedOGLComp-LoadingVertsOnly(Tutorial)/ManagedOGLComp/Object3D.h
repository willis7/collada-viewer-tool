#pragma once
#include "Vector3f.h"

class Object3D //abstract class can never be instanciated. 
{
protected:
	Vector3f* position;
	Vector3f* orientation;
	Vector3f* scale; 

public:
	Object3D(void);
	Object3D(Vector3f*, Vector3f*); //Constructor which takes position and orientation. 
	~Object3D(void);

	void setPosition(Vector3f* v);
	void setOrientation(Vector3f* v);
	void setScale(Vector3f* v);

	Vector3f* getOrientation(void);
	Vector3f* getPosition(void);
	Vector3f* getScale(void);

	virtual void draw() = 0; //pure virtual function, makes class absract

};
