#pragma once
#include <vector>

using namespace std;

class MyPolygon
{
private:
	int indexCount; //number of indexes into the poly array for each face
	vector<int> vertIndicies;
	vector<int> normIndicies;

public:
	MyPolygon(void);
	MyPolygon(int); //takes index count (ie no of verts/norms per face);
	~MyPolygon(void);

	int getIndexCount();
	void setIndexCount(int);

	void addVertIndex(int);
	void addNormIndex(int);

	int getVertIndexN(int);
	int getNormIndexN(int);

	void printVertIndex();
	void printNormIndex();

};
