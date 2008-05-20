// This is the main DLL file.

#include "stdafx.h"

#include "ManagedOGLComp.h"

	ManagedOGLComp::OGLPanelControl::OGLPanelControl(System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight)
	{	
		//user overloaded attributes to set location and size
		this->Location = *(new Point(20,20)); //fix this should be a attribute
		this->Width = iWidth;
		this->Height = iHeight;

		//This creates a window handle from the pointer passed by C#
		HWND windowHandle = (HWND)this->Handle.ToPointer();

		//This creates a divice context from the window handle
		m_hDC = GetDC(windowHandle);

		gl = new MyGLClass(m_hDC);	

		collada = new ColladaImport();

	}

	
	ManagedOGLComp::OGLPanelControl::OGLPanelControl(System::Windows::Forms::Form ^ parentForm, 
						GLsizei iWidth, GLsizei iHeight, OGLPanelControl^ shareListObj)
	{
		//user overloaded attributes to set location and size
		this->Location = *(new Point(20,20)); //fix this should be a attribute
		this->Width = iWidth;
		this->Height = iHeight;

		//This creates a window handle from the pointer passed by C#
		HWND windowHandle = (HWND)this->Handle.ToPointer();

		//This creates a divice context from the window handle
		m_hDC = GetDC(windowHandle);

		gl = new MyGLClass(m_hDC);
		gl->setDrawList(shareListObj->getGL()->getDrawList());

		collada = new ColladaImport();
	
	}

	void ManagedOGLComp::OGLPanelControl::setSize(int h, int w)
	{
		this->Height = h;
		this->Width = w;
	}

	void ManagedOGLComp::OGLPanelControl::setLocation(int x, int y)
	{
		this->Location.X = x;
		this->Location.Y = y;
	}

	System::Void ManagedOGLComp::OGLPanelControl::Render(System::Void)
	{
		gl->draw();
	}

	System::Void ManagedOGLComp::OGLPanelControl::SwapOpenGLBuffers(System::Void)
	{
		SwapBuffers(m_hDC);
	}

	void ManagedOGLComp::OGLPanelControl::OnPaint(System::Windows::Forms::PaintEventArgs ^e) 
	{
		UNREFERENCED_PARAMETER(e);
		this->Render();
		this->SwapOpenGLBuffers();
	}

	void ManagedOGLComp::OGLPanelControl::OnMouseDown(System::Windows::Forms::MouseEventArgs ^e) 
	{
		mouseDown = true;
		xPos = this->MousePosition.X;
		yPos = this->MousePosition.Y;
	}

	void ManagedOGLComp::OGLPanelControl::OnMouseUp(System::Windows::Forms::MouseEventArgs ^e) 
	{
		mouseDown = false;
	}

	void ManagedOGLComp::OGLPanelControl::OnMouseMove(System::Windows::Forms::MouseEventArgs ^e) 
	{
		if(mouseDown == true)
		{
			xDrag = xPos - this->MousePosition.X;
			yDrag = yPos - this->MousePosition.Y;

			gl->dispCameraXZ(-yDrag);
			gl->dispCameraY(-xDrag);

			xPos = this->MousePosition.X;
			yPos = this->MousePosition.Y;
		}
		this->Render();
		this->SwapOpenGLBuffers();
	}

	void ManagedOGLComp::OGLPanelControl::OnMouseWheel(System::Windows::Forms::MouseEventArgs ^e)
	{
	//	gl->dispCameraR(e->Delta*1.0f); //Delta is the number of clics of the wheel
	//	this->Render();
	//	this->SwapOpenGLBuffers();
	}

	void ManagedOGLComp::OGLPanelControl::zoomIn()
	{
		gl->dispCameraR(0.5f);
		this->Render();
		this->SwapOpenGLBuffers();
	}

	void ManagedOGLComp::OGLPanelControl::zoomOut()
	{
		gl->dispCameraR(-0.5f);
		this->Render();
		this->SwapOpenGLBuffers();
	}

	void ManagedOGLComp::OGLPanelControl::rotateXZ(double offset)
	{
		gl->dispCameraXZ(offset);
	}

	void ManagedOGLComp::OGLPanelControl::rotateY(double offset)
	{
		gl->dispCameraY(offset);
	}

	void ManagedOGLComp::OGLPanelControl::moveCameraScreenX(double offset)
	{
		//Not implemented
	}

	void ManagedOGLComp::OGLPanelControl::moveCameraScreenY(double offset)
	{
		//Not implemented
	}
	
	void ManagedOGLComp::OGLPanelControl::addImportedModel(System::String^ URL)
	{
		//Convert incomming string to a char*
		char* cString = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(URL).ToPointer();
		
		//local MyGeometryObject - will add this like the test box
		MyGeometry *geom;
		
		//tell the colladaImporter to load the file
		this->collada->loadColladaFile(cString);
		
		//ask the collada importer for the geometry object
		geom = this->collada->getMyGeom();
		
		//set position oritentaiton and scale - scale is nessessary as duck is huge
		geom->setPosition(new Vector3f(0.0f,0.0f,0.0f));
		geom->setOrientation(new Vector3f(0.0f,0.0f,0.0f));
		geom->setScale(new Vector3f(0.01f,0.01f,0.01f));
		//geom->setScale(new Vector3f(1.0f,1.0f,1.0f));

		//add newly created geometry object to the render list
		gl->add(geom);
	}

	void ManagedOGLComp::OGLPanelControl::addTestBox()
	{
		MyGeometry *geom = new MyGeometry();
		MyPolygon *poly;

		//set position oritentaiton and scale 
		geom->setPosition(new Vector3f(0.0f,0.0f,0.0f));
		geom->setOrientation(new Vector3f(0.0f,0.0f,0.0f));
		geom->setScale(new Vector3f(1.0f,1.0f,1.0f));

		//Declare verticies

		geom->addVertex(new Vector3f(0.0f,0.0f,0.0f)); //0
		geom->addVertex(new Vector3f(0.0f,0.5f,0.0f)); //1
		geom->addVertex(new Vector3f(0.5f,0.5f,0.0f)); //2
		geom->addVertex(new Vector3f(0.5f,0.0f,0.0f)); //3

		geom->addVertex(new Vector3f(0.0f,0.0f,-0.5f)); //4
		geom->addVertex(new Vector3f(0.0f,0.5f,-0.5f)); //5
		geom->addVertex(new Vector3f(0.5f,0.5f,-0.5f)); //6
		geom->addVertex(new Vector3f(0.5f,0.0f,-0.5f)); //7

		//Declare normals - there are only 6 unique normals. 

		//North and South
		geom->addNormal(new Vector3f(0.0f,1.0f,0.0f)); //0
		geom->addNormal(new Vector3f(0.0f,-1.0f,0.0f)); //1

		//West and East
		geom->addNormal(new Vector3f(1.0f,0.0f,0.0f)); //2
		geom->addNormal(new Vector3f(-1.0f,0.0f,0.0f)); //3

		//Out and In
		geom->addNormal(new Vector3f(0.0f,0.0f,1.0f)); //4
		geom->addNormal(new Vector3f(0.0f,0.0f,-1.0f)); //5

		//Triangle Version - 12 triangles
		
		poly = new MyPolygon(3); //Triangle A
		
		poly->addVertIndex(3); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(1); 
		poly->addVertIndex(0);

		poly->addNormIndex(4);
		poly->addNormIndex(4);
		poly->addNormIndex(4);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle B

		poly->addVertIndex(2); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(1);
		poly->addVertIndex(3);

		poly->addNormIndex(4);
		poly->addNormIndex(4);
		poly->addNormIndex(4);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle C

		poly->addVertIndex(1); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(5);
		poly->addVertIndex(0);

		poly->addNormIndex(2);
		poly->addNormIndex(2);
		poly->addNormIndex(2);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle D

		poly->addVertIndex(0); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(5);
		poly->addVertIndex(4);

		poly->addNormIndex(2);
		poly->addNormIndex(2);
		poly->addNormIndex(2);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle E

		poly->addVertIndex(6); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(2);
		poly->addVertIndex(3);

		poly->addNormIndex(3);
		poly->addNormIndex(3);
		poly->addNormIndex(3);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle F

		poly->addVertIndex(3); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(7);
		poly->addVertIndex(6);

		poly->addNormIndex(3);
		poly->addNormIndex(3);
		poly->addNormIndex(3);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle G

		poly->addVertIndex(7); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(3);
		poly->addVertIndex(0);

		poly->addNormIndex(1);
		poly->addNormIndex(1);
		poly->addNormIndex(1);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle H

		poly->addVertIndex(4); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(7);
		poly->addVertIndex(0);

		poly->addNormIndex(1);
		poly->addNormIndex(1);
		poly->addNormIndex(1);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle I

		poly->addVertIndex(4); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(5);
		poly->addVertIndex(6);

		poly->addNormIndex(5);
		poly->addNormIndex(5);
		poly->addNormIndex(5);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle J

		poly->addVertIndex(4); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(6);
		poly->addVertIndex(7);

		poly->addNormIndex(5);
		poly->addNormIndex(5);
		poly->addNormIndex(5);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle K

		poly->addVertIndex(2); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(5);
		poly->addVertIndex(1);

		poly->addNormIndex(0);
		poly->addNormIndex(0);
		poly->addNormIndex(0);

		geom->addPolygon(poly);

		poly = new MyPolygon(3); //Triangle L

		poly->addVertIndex(2); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(6);
		poly->addVertIndex(5);

		poly->addNormIndex(0);
		poly->addNormIndex(0);
		poly->addNormIndex(0);

		geom->addPolygon(poly);

		gl->add(geom);



	}

	void ManagedOGLComp::OGLPanelControl::addTestPlane()
	{
		MyGeometry *geom = new MyGeometry();
		MyPolygon *poly;

		//set position oritentaiton and scale 
		geom->setPosition(new Vector3f(0.0f,0.0f,0.0f));
		geom->setOrientation(new Vector3f(0.0f,0.0f,0.0f));
		geom->setScale(new Vector3f(1.0f,1.0f,1.0f));

		//Declare verticies

		geom->addVertex(new Vector3f(0.0f,0.0f,0.0f)); //0
		geom->addVertex(new Vector3f(0.0f,0.5f,0.0f)); //1
		geom->addVertex(new Vector3f(0.5f,0.5f,0.0f)); //2
		geom->addVertex(new Vector3f(0.5f,0.0f,0.0f)); //3

		geom->addVertex(new Vector3f(0.0f,0.0f,-0.5f)); //4
		geom->addVertex(new Vector3f(0.0f,0.5f,-0.5f)); //5
		geom->addVertex(new Vector3f(0.5f,0.5f,-0.5f)); //6
		geom->addVertex(new Vector3f(0.5f,0.0f,-0.5f)); //7

		//Declare normals - there are only 6 unique normals. 

		//North and South
		geom->addNormal(new Vector3f(0.0f,1.0f,0.0f)); //0
		geom->addNormal(new Vector3f(0.0f,-1.0f,0.0f)); //1

		//West and East
		geom->addNormal(new Vector3f(1.0f,0.0f,0.0f)); //2
		geom->addNormal(new Vector3f(-1.0f,0.0f,0.0f)); //3

		//Out and In
		geom->addNormal(new Vector3f(0.0f,0.0f,1.0f)); //4
		geom->addNormal(new Vector3f(0.0f,0.0f,-1.0f)); //5

		poly = new MyPolygon(3); //triangles
		
		poly->addVertIndex(3); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(1); 
		poly->addVertIndex(0);

		poly->addNormIndex(4);
		poly->addNormIndex(4);
		poly->addNormIndex(4);

		geom->addPolygon(poly);

		poly = new MyPolygon(3);

		poly->addVertIndex(2); //counter clockwise vert indexes for each face. 
		poly->addVertIndex(1);
		poly->addVertIndex(3);

		poly->addNormIndex(4);
		poly->addNormIndex(4);
		poly->addNormIndex(4);

		geom->addPolygon(poly);



		gl->add(geom);

	}

	MyGLClass* ManagedOGLComp::OGLPanelControl::getGL()
	{
		return gl;
	}

	ManagedOGLComp::OGLPanelControl::~OGLPanelControl()
	{
		gl->~MyGLClass();
	}
