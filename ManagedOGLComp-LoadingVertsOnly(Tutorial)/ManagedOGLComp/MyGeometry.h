#pragma once
#include "object3d.h"
#include "Vector3f.h"
#include "MyPolygon.h"
#include "myGL.h"
#include <vector>

using namespace std;

class MyGeometry : public Object3D
{

private:
	int vertexCount; // in collada these will be the same as it has a normal per vertex
	int normalCount; // this is not always the case. 


	vector<Vector3f*> verts; //verticies
	vector<Vector3f*> norms; //normals	

	vector<MyPolygon*> polys; //polygons - specify the verts and norms of each polygon
							  //might be triangles, quads etc. 

public:
	MyGeometry(void);
	~MyGeometry(void);

	int getVertexCount();
	int getNormalCount();

	void addVertex(Vector3f*);
	void addNormal(Vector3f*);

	void removeVertex();
	void removeNormal();

	void addVertNormPair(Vector3f* v, Vector3f* n);
	void removeVertexNormPair();

	Vector3f* getVertexN(int);
	Vector3f* getNormalN(int);

	MyPolygon* getPolygonN(int);
	void addPolygon(MyPolygon *p);

	void draw(void); //this overrides the parent method, which is virtual. 

	void printVerts();
	void printNormals();
	void printPolyIndexes();


	

};
