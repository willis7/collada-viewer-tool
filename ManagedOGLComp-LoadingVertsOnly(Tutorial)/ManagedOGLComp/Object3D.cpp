#include "StdAfx.h"
#include "Object3D.h"

Object3D::Object3D(void)
{

}

Object3D::Object3D(Vector3f* p, Vector3f* o)
{
	position = new Vector3f(p);
	orientation = new Vector3f(o);
}

Object3D::~Object3D(void)
{
	delete(position);
	delete(orientation);
}

void Object3D::setPosition(Vector3f* v)
{
	position = new Vector3f(v);
}

void Object3D::setOrientation(Vector3f* v)
{
	orientation = new Vector3f(v);
}

void Object3D::setScale(Vector3f* v)
{
	scale = new Vector3f(v);
}

Vector3f* Object3D::getOrientation(void)
{
	return orientation;
}

Vector3f* Object3D::getPosition(void)
{
	return position;
}

Vector3f* Object3D::getScale(void)
{
	return scale;
}