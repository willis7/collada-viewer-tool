using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CShrpWindow
{
    
    public partial class Form1 : Form
    {
        //OGL Object 
        private ManagedOGLComp.OGLPanelControl ogl;

        public Form1()
        {
            InitializeComponent();
            this.Size = new Size(800, 800);

            //OGL Setup
            ogl = new ManagedOGLComp.OGLPanelControl(this, 400, 500);
            ogl.Location = new Point(20, 20);
            ogl.Parent = this;

            //ogl.addTestBox();
            String s = "Currently hardcoded";

            //import a model note function is currently hard coded
            //ogl.addImportedModel(s);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            ogl.zoomIn();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ogl.zoomOut();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            OpenFileDialog browseFile = new OpenFileDialog();
            browseFile.Filter = "DAE Files (*.dae)|*.dae";
            browseFile.Title = "Browse .DAE file";
            if (browseFile.ShowDialog() == DialogResult.OK)
            {
                browseFile.FileName.Replace("\\", "/");
                ogl.addImportedModel(browseFile.FileName);           
            }

        }

       

    
       
    }
}