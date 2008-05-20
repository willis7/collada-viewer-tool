#include "StdAfx.h"
#include "ColladaImport.h"
#include "integrationSimple.h"


ColladaImport::ColladaImport(void)
{
	// Instantiate the reference implementation
	dae = new DAE();
	
	//tell the dae what function to call to register the intergration elements
	//this is the function in intergrationSimple.h/cpp
	dae->setIntegrationLibrary(&intRegisterElements);

	//set flag - make sure we don't try and access a geometry
	fileLoadedFlag = false;

	//just incase we call get geometry before loading a file
	myGeom=NULL;
}

void ColladaImport::loadColladaFile(char *URL)
{
	for(int i = 0; i < strlen(URL); i++)
	{
		if(URL[i] == '\\')
		{
			URL[i] = '/';
		}
	}
	char* filename = (char*) malloc((strlen(URL)+8)*sizeof (char*));
	strcpy(filename, "file:///");
	strcat(filename, URL);
	//daeString filename = changePath(URL);
	//load the COLLADA file
	//For this i encountered a problme late on, 
	//i tried to do the string handling in the c# but wasnt able to complete
	int res = dae->load(filename);
	//int res = dae->load(URL);

	//if result is not an error code set the flag to say the file is loaded
	if(res!=0)
		fileLoadedFlag = false;
	else 
		fileLoadedFlag = true;
	
	//extract the geometry to a MyGeometry obejct
	extractMyGeometry();
}
/*asks for the geometry element of the first loaded collada object*/
void ColladaImport::extractMyGeometry()
{
	if(fileLoadedFlag)
	{
		//query the runtime to retrieve an element
		domGeometry* domGeom = 0;
		int res = dae->getDatabase()->getElement((daeElement**)&domGeom, 
													0, NULL, COLLADA_TYPE_GEOMETRY);

		// Get the integration object from the element
		intGeometry *intGeom = (intGeometry*)domGeom->getIntObject();
		
		// Extract the user data from the integration object
		myGeom = (MyGeometry*)intGeom->getGeometry();
	}
}

/*gets the geometry function extracted from the dea */
MyGeometry* ColladaImport::getMyGeom()
{
	return myGeom;
}

ColladaImport::~ColladaImport(void)
{
	delete(dae);
}
