#pragma once

#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domProfile_CG.h>
#include <dom/domConstants.h>
#include <dae/daeDomTypes.h>
#include "MyGeometry.h"
#include <string.h>

class ColladaImport
{
private:
		DAE *dae;
		domGeometry* geometryElement;
		domMesh* meshElement;
		daeElement* element;

		MyGeometry* myGeom;

		bool fileLoadedFlag;
		void extractMyGeometry();

public:
	ColladaImport(void);
	~ColladaImport(void);

	void loadColladaFile(char* URL);
	MyGeometry* getMyGeom(void);
	
};
