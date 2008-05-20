// ManagedOGLComp.h

#pragma once


#include "myGL.h"

#include "MyGLClass.h"
#include "MyGeometry.h"
#include "MyPolygon.h"
#include "ColladaImport.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace ManagedOGLComp 
{

	public ref class OGLPanelControl : public System::Windows::Forms::Panel
	{
	
	private:
		//HDC for ogl
		HDC m_hDC;

		//OpenGL object
		MyGLClass* gl;

		//dragging variables
		static bool mouseDown = false;
		static double xPos = 0.0;
		static double yPos = 0.0;

		static double xDrag = 0.0;
		static double yDrag = 0.0;

		//collada importer
		ColladaImport *collada;

	public:
		//Constructor
		OGLPanelControl(System::Windows::Forms::Form ^ parentForm, 
						GLsizei iWidth, GLsizei iHeight);

		//Constructor
		OGLPanelControl(System::Windows::Forms::Form ^ parentForm, 
						GLsizei iWidth, GLsizei iHeight, OGLPanelControl^ shareListObj);

				//Panel Properties set methods
		//Set size
		void setSize(int h, int w);

		//Set location
		void setLocation(int x, int y);

				//Panel event overide methods
			
		//Mouse control overides for the panel object
		virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs ^e) override;
		virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs ^e) override;
		virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs ^e) override;
		virtual void OnPaint(System::Windows::Forms::PaintEventArgs ^e) override;
		virtual void OnMouseWheel(System::Windows::Forms::MouseEventArgs ^e) override;
		
				//OpenGL object wrapper methods
		//Renderer for object
		virtual System::Void Render(System::Void);
		
		//Swap Buffers for object
		System::Void SwapOpenGLBuffers(System::Void);
	
		//Zoom Functions
		void zoomIn();
		void zoomOut();

		//Camera Rotation
		void rotateXZ(double offset);
		void rotateY(double offset);

		//Move camera 2D
		void moveCameraScreenX(double offset);
		void moveCameraScreenY(double offset);

		void addTestBox();
		void addTestPlane();

		void addImportedModel(System::String^ URL);

		MyGLClass* getGL();

		protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~OGLPanelControl();
	};
}
