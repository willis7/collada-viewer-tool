#include "StdAfx.h"
#include "Vector3f.h"

Vector3f::Vector3f(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3f::Vector3f(float inX, float inY, float inZ)
{
	x = inX;
	y = inY;
	z = inZ;
}

Vector3f::~Vector3f(void)
{
}

Vector3f::Vector3f(Vector3f *v) //copy constructor
{
	x = v->getX();
	y = v->getY();
	z = v->getZ();
}
	
float Vector3f::getX(void)
{
	return x;
}

float Vector3f::getY(void)
{
	return y;
}

float Vector3f::getZ(void)
{
	return z;
}

void Vector3f::setX(float f)
{
	x = f;	
}

void Vector3f::setY(float f)
{
	y = f;
}

void Vector3f::setZ(float f)
{
	z = f;
}