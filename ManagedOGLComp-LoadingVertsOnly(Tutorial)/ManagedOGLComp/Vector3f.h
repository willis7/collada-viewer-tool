#pragma once

class Vector3f
{
private:
	float x;
	float y;
	float z;

public:
	Vector3f(void);
	Vector3f(Vector3f *v); //copy constructor
	Vector3f(float, float, float);
	
	~Vector3f(void);

	float getX(void);
	float getY(void);
	float getZ(void);

	void setX(float);
	void setY(float);
	void setZ(float);

};
