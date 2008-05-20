#include "StdAfx.h"
#include "MyPolygon.h"

MyPolygon::MyPolygon(void)
{
	indexCount = 0;
}

MyPolygon::~MyPolygon(void)
{
	while(!vertIndicies.empty())
	{
		vertIndicies.pop_back();	
	}

	while(!normIndicies.empty())
	{
		normIndicies.pop_back();	
	}
}

MyPolygon::MyPolygon(int n)
{
	indexCount = n;
}

int MyPolygon::getIndexCount()
{
	return indexCount;
}

void MyPolygon::setIndexCount(int i)
{
	indexCount = i;
}

void MyPolygon::addVertIndex(int i)
{
	vertIndicies.push_back(i);
}

void MyPolygon::addNormIndex(int i)
{
	normIndicies.push_back(i);
}

int MyPolygon::getVertIndexN(int i)
{
	return vertIndicies.at(i);
}

int MyPolygon::getNormIndexN(int i)
{
	return normIndicies.at(i);
}

void MyPolygon::printVertIndex()
{
	printf("-------- Start print vertex Indexes -------------\n");
	for(int i=0; i< vertIndicies.size(); i++)
	{
		printf("Vertex Index %d :- %f\n",i,vertIndicies.at(i));
	}
	printf("-------- End print vertex Indexes ---------------\n");
}

void MyPolygon::printNormIndex()
{
	printf("-------- Start print normal Indexes -------------\n");
	for(int i=0; i< normIndicies.size(); i++)
	{
		printf("Normal Index %d :- %f\n",i,normIndicies.at(i));
	}
	printf("-------- End print normal Indexes ---------------\n");
}