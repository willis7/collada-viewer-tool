/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License 
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 * implied. See the License for the specific language governing permissions and limitations under the 
 * License. 
 */
#include "StdAfx.h"
#include <dae/daeDom.h>
#include "intGeometry.h"
#include <dom/domGeometry.h>
#include <dom/domPolylist.h>
#include <string.h>


daeMetaElement * intGeometry::_Meta = NULL;

// ********************************************************
// ***** GENERATED INTERFACE - do NOT touch ***************
// ********************************************************

daeElementRef
intGeometry::create(daeInt bytes)
{
	intGeometryRef ref = new(bytes) intGeometry;
	return ref;
}

daeMetaElement *
intGeometry::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "geometry" );
	_Meta->registerClass(intGeometry::create, &_Meta);

	domGeometry::_Meta->setMetaIntegration(_Meta);

	_Meta->setElementSize(sizeof(intGeometry));
	_Meta->validate();

	return _Meta;
}


// ********************************************************
// ***** INTEGRATION INTERFACE ****************************
// ********************************************************

// CONSTRUCTOR 
intGeometry::intGeometry() 
{
}

/* DESTRUCTOR

This mops up any allocations we carried out in the 
createFerom e.g. creation of a new myGeometry object */

intGeometry::~intGeometry()
{
	delete(_object); //call delete for my object
}

// IMPORT

void
intGeometry::createFrom(daeElementRef element)
{
	// INSERT CODE TO CREATE YOUR USER DATA HERE
	// _object = new myRuntimeClass;

	/* 
	 This code creates a user defined class when a domGeometry object is encountered
	 during loading of a COLLADA document.

	 Create my class to hold geometry information and initialize it as empty */

	_object = new MyGeometry();	//my geometry class
	_element = element;			//empty dea element
}

void
intGeometry::fromCOLLADA()
{
	// INSERT CODE TO TRANSLATE TO YOUR RUNTIME HERE
	// This code translates from a COLLADA element to a user provided structure

	// Note: this is a very simple example of an integration class that converts
	// a collada GEOMETRY to a user class.  It gets vertex and connectivity data
	// from the polygons and float array found inside the geometry.  The example
	// only supports one float array and one polygons per domGeometry and makes
	// some assumptions about their format to keep the example simple.
	// If you were doing this for a real application you would probably want to 
	// have seperate integration objects for geometry, mesh, polygons...etc.
	// so you could support multiple polygons, techniques, flexible input formats...etc.

	int triangleCount;	//number of triangles
	int indexCount;		// total lenght of the <p> array
	int vOffset = -1;	//vertex offset in <p> array
	int nOffset = -1;   //normal offset in <p> array
	int tOffset = -1;	//textCoord offset in <p> array

	//string id's of source arrays - so we process the right one to the right place
	char* vertexArrayID; //must be mapped to <verticies><source>
	char* normalArrayID =""; 
	char* textCoordArrayID;

	// Get a properly typed pointer to the COLLADA domGeometry element 
	//(this is the element that called us)
	domGeometry* geometryElement = (domGeometry*)(domElement*)_element;

	// Get a pointer to the domGeometry's domMesh element
	domMesh *meshElement = geometryElement->getMesh();

	// Get a pointer to the user defined geometry 
	//object that was automatically created during load
	// by calling createFrom
	MyGeometry *local = (MyGeometry *)_object;

	//Interrogate the dom, find out how may triangles we have
	//assume only one triangle library
	domTriangles *t = meshElement->getTriangles_array()[0]; 
	triangleCount = t->getCount();
	
	/* Process indicies (vertex and normal) */

	//Get the <p> array - interleaved indexes into source arrays (vert, norm, text coord)
	domListOfUInts P = t->getP()->getValue();
	indexCount = P.getCount();

	//Look at the semantics of the input arrays to decide how <p> is organised
	//and process it accordingly
	for(int i =0; i < t->getInput_array().getCount(); i++)
	{
		domInputLocalOffsetRef d1 = t->getInput_array().get(i);

		//get input array semantic string
		char* str = (char*)d1->getSemantic();

		if(strcmp(str,"VERTEX")==0)
		{
			vOffset = d1->getOffset(); //numerical offset of data in <p>
			vertexArrayID = (char*)d1->getSource().getID(); //id of <source> array
		}
		else
		{
			if(strcmp(str,"NORMAL")==0)
			{
				//set nOffset to the offset of the normal in <p>
				//set normalArrayID to the id of the source indicated in this input element
				nOffset = d1->getOffset();
				normalArrayID = (char*)d1->getSource().getID();
			}
			else
			{
				if(strcmp(str,"TEXCOORD")==0)
				{
					//process text coordinates!
					printf("Found a text coord - ignoring");
				}
				else
				{
					//something else?!
					printf("Attribute found %s \n", str);
				}
			}
		}
	}

	//need to step over as many elements as are present to move to the next one. 
	int pStep = t->getInput_array().getCount(); 

	//Local arrays - to contain one index each / by number of entries
	//ie the step calculated above
	int* pVertIndex = (int*)malloc(((indexCount/pStep) * sizeof(int)));
	int* pNormIndex = (int*)malloc(((indexCount/pStep) * sizeof(int)));

	int i; //index into <p>
	int j; //index into internal arrays
	
	//read from single <p> array into two seperate arrays
	for(i=0,j =0; i<indexCount; i+=pStep, j++)
	{
		pVertIndex[j] = P[vOffset+i];
		//recover the normal index from the <p> array using the offset and increment (i)
		pNormIndex[j] = P[nOffset+i];
		

	}

	//ready from seperate arrays into a polygon
	for(int i=0; i<j; i+=3)
	{
		//create a new polygone object, set the number 
		//of indicies and read in the indexes
		//3 indexes per triangle 
		MyPolygon *p = new MyPolygon();
		p->setIndexCount(3);
		
		//assign indexes
		p->addVertIndex(pVertIndex[i+0]);
		p->addVertIndex(pVertIndex[i+1]);
		p->addVertIndex(pVertIndex[i+2]);

		//assign normals - 1 normal per vertex
		//assign 3 normals to the polygon
		//simular to vertex above
		p->addNormIndex(pNormIndex[i+0]);
		p->addNormIndex(pNormIndex[i+1]);
		p->addNormIndex(pNormIndex[i+2]);

		//add the polygon to the end of the vector
		local->addPolygon(p);
	}

	free(pVertIndex);
	free(pNormIndex);

	/*Process source arrays */
	
	//Number of source arrays
	//This includes - the position array (referenced though the normal array!)
	//Cannot assume order, so must use 
	int sourceArrayCount = meshElement->getSource_array().getCount();

	//store the name <verticies> array - not used
	char* verticiesID =(char*)meshElement->getVertices()->getId();
	//store the id of the source of the verticeis array
	//input object for vertex will give verticiesID as its source but
	//actual source is the source of the <verticies> so store this!
	char* positionID = 
	(char*)((meshElement->getVertices()->getInput_array().get(0))->getSource().getID());

	//There may be a number of source arrays
	char* sourceID; //current source;
	unsigned int stride; //how many elements per vertex (3 or 4)
	domFloat_array *floatArray; //stores the source data
	unsigned int sourceCount;

	//For each source array check whether it maches the stored source ids for vertex
	//or normals then store it in the appropriate place within our 
	//MyGeometry object
	for(int i=0; i<sourceArrayCount; i++)
	{
		//get the ith source array
		domSource *source = meshElement->getSource_array()[i];
		//get its id
		sourceID = (char*)source->getID();

		if(source->getFloat_array()==NULL)
		{
			printf("Float array was NULL possible mall formed collada file\n");
		}
		else
		{
			//get the array of floats
			floatArray = source->getFloat_array(); 
			
			// Interrogate the common technique to get stride
			stride = source->getTechnique_common()->getAccessor()->getStride();

			//set source count
			sourceCount = floatArray->getCount();

			//if this is the vertex array
			if((strcmp(sourceID,vertexArrayID)==0)||(strcmp(sourceID,positionID)==0)) 
			{
				// Copy the vertices into my structure one-by-one 
				//(converts from COLLADA's doubles to floats)
				unsigned int i;
				for (i = 0; i < sourceCount; i+=stride) 
				{
					//Assumes x,y,z order - could interrogate accessor to check this
					local->addVertex(new Vector3f(floatArray->getValue()[i],
													floatArray->getValue()[i+1],
													floatArray->getValue()[i+2]));
				}

				//  debuggnig printout has a big performance impact
			}
			else 
			{
				//if source array is the normals array
				if(strcmp(sourceID,normalArrayID)==0) 
				{
					// Copy the normals into my structure one-by-one 
					//(converts from COLLADA's doubles to floats)
					
					//Add all the normals to the normal array in the local MyGeometry
					//object from the source array

					unsigned int i;
					for(i = 0; i <sourceCount; i += stride)
					{
						local->addNormal(new Vector3f(floatArray->getValue()[i],
														floatArray->getValue()[i+1],
														floatArray->getValue()[i+2]));
					}

				//  debuggnig printout has a big performance impact
					//	local->printNormals();
				}
				else
				{
					printf("Found unsupported array - ignoring \n");
				}
			}
		}
	}
	
	
}

void
intGeometry::fromCOLLADAPostProcess()
{
	// INSERT CODE TO POST PROCESS HERE
	// myRuntimeClassType* local = (myRuntimeClassType*)_object;
	// local->renderingContext = MYGLOBAL::getRenderingContext;
	// MYGLOBAL::registerInstance(local);
}

// EXPORT

void
intGeometry::createTo(void* userData)
{
	// INSERT CODE TO CREATE COLLADA DOM OBJECTS HERE
	
}

void
intGeometry::toCOLLADA()
{
	
}

void
intGeometry::toCOLLADAPostProcess()
{
	// INSERT CODE TO POST PROCESS HERE
	// myRuntimeClassType* local = (myRuntimeClassType*)_object;
	// local->renderingContext = MYGLOBAL::getRenderingContext;
	// MYGLOBAL::registerInstance(local);
}

//USER CREATED METHODS

/*	These methods must be created seperatly from generated files as they are specific to
	and indervidual implementation */

/*getGeometry method

This allows us to get the created geometry object from intGeometry */

MyGeometry*
intGeometry::getGeometry()
 {
	 return _object; 
 }

