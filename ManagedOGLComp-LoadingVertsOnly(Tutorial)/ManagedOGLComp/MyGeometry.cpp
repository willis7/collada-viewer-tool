#include "StdAfx.h"
#include "MyGeometry.h"


MyGeometry::MyGeometry(void)
{
	vertexCount = 0;
	normalCount = 0;

}

MyGeometry::~MyGeometry(void)
{
	
	Vector3f* temp; 
	MyPolygon* tempPoly;
	while(!verts.empty())
	{
		temp = verts.back();
		verts.pop_back();
		delete(temp);
	}

	while(!norms.empty())
	{
		temp = norms.back();
		norms.pop_back();
		delete(temp);
	}

	while(!polys.empty())
	{
		tempPoly = polys.back();
		polys.pop_back();
		delete(tempPoly);
	}
}

int MyGeometry::getVertexCount()
{
	return vertexCount;
}

int MyGeometry::getNormalCount()
{
	return normalCount;
}

void MyGeometry::addVertex(Vector3f* v)
{
	verts.push_back(v);
	vertexCount++;
}

void MyGeometry::addNormal(Vector3f* v)
{
	norms.push_back(v);
	normalCount++;
}

void MyGeometry::removeVertex()
{
	verts.pop_back();
	vertexCount--;
}

void MyGeometry::removeNormal()
{
	norms.pop_back();
	normalCount--;
}

void MyGeometry::addVertNormPair(Vector3f* v, Vector3f* n)
{
	verts.push_back(v);
	norms.push_back(n);
	vertexCount++;
	normalCount++;
}

void MyGeometry::removeVertexNormPair()
{
	verts.pop_back();
	norms.pop_back();
}

Vector3f* MyGeometry::getVertexN(int i)
{
	return verts.at(i);
}

Vector3f* MyGeometry::getNormalN(int i)
{
	return norms.at(i);
}

MyPolygon* MyGeometry::getPolygonN(int i)
{
	return polys.at(i);
}

void MyGeometry::addPolygon(MyPolygon *p)
{
	polys.push_back(p);
}

void MyGeometry::draw()
{
	int i,j;
	int numberOfFaces = polys.size();
	MyPolygon *p; //local pointer to face
	Vector3f *vert;
	Vector3f *norm;
	int normIndex;
	int vertIndex;

	int mode;

	//work out how may verts per face
	switch(polys.front()->getIndexCount()) 
	{
		case 3:
			mode = GL_TRIANGLES;
			break;
		case 4:
			mode = GL_QUADS;
			break;
		default:
			mode = GL_POLYGON;
			break;
	}

	glPushMatrix();

		glTranslatef(position->getX(),position->getY(),position->getZ());
		glRotatef(orientation->getX(),1.0f,0.0f,0.0f);
		glRotatef(orientation->getY(),0.0f,1.0f,0.0f);
		glRotatef(orientation->getZ(),0.0f,0.0f,1.0f);
		
		glScalef(scale->getX(),scale->getY(),scale->getZ());

		glBegin(mode);
		//glBegin(GL_TRIANGLES);
			
			for(i=0;i<numberOfFaces; i++)
			{
				//Draw each face - i.e draw each point
				p = polys.at(i);
				for(j=0;j<p->getIndexCount();j++)
				{
					if(norms.size()!=0)
					{
						normIndex = p->getNormIndexN(j);
						norm = norms.at(normIndex);
						glNormal3f(norm->getX(),norm->getY(),norm->getZ());
					}

					if(verts.size()!=0)
					{
						vertIndex = p->getVertIndexN(j);
						vert = verts.at(vertIndex);
						glVertex3f(vert->getX(),vert->getY(),vert->getZ());
					}
					glEnd();

						glBegin(GL_LINES);
						{
							glVertex3f(vert->getX(),vert->getY(),vert->getZ());
							glVertex3f(vert->getX()+norm->getX(),vert->getY()+norm->getY(),vert->getZ()+norm->getZ());
						}
						glEnd();
					
				}
			}
		
		
	glPopMatrix();


}

void MyGeometry::printVerts()
{
	Vector3f* temp;
	printf("------------ Start Vertex Print ------------\n");
	for(int i =0; i<verts.size(); i++)
	{
		temp = verts.at(i);
		printf("Vertex %d :- {%f,%f,%f}",i,temp->getX(),temp->getY(), temp->getZ());
	}
	printf("------------ End Vertex Print --------------\n");
}

void MyGeometry::printNormals()
{
	Vector3f* temp;
	printf("------------ Start Normals Print ------------\n");
	for(int i =0; i<norms.size(); i++)
	{
		temp = norms.at(i);
		printf("Normal %d :- {%f,%f,%f}",i,temp->getX(),temp->getY(), temp->getZ());
	}
	printf("------------ End Normals Print --------------\n");
}

void MyGeometry::printPolyIndexes()
{
	MyPolygon *temp;
	printf("----------- Start Polygon Index Print ------------\n");
	for(int i =0; i<polys.size(); i++)
	{
		temp = polys.at(i);
		printf("======= Start Normals and Vertexs for Polygon %d ======= \n",i);
		temp->printVertIndex();
		temp->printNormIndex();
		printf("======= End Normals and Vertexs for Polygon %d ======= \n",i);
	}
}
