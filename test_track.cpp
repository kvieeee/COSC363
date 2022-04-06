//  ========================================================================
//  COSC363: Computer Graphics (2021);  University of Canterbury.
//
//  FILE NAME: Model3D.cpp
//  See Lab01.pdf for details
//
//  Program to load a mesh file in OFF format.
//  Assumptions:
//      1.  Each polygon has 3 or 4 vertices.
//      2.  The file does not contain any additional comment lines.
//  ========================================================================

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
using namespace std;

//--Globals ---------------------------------------------------------------
float *x, *y, *z;                   //vertex coordinates
int *nv, *t1, *t2, *t3, *t4;        //number of vertices and vertex indices of each face
int nvert, nface;                   //total number of vertices and faces
float angleX = 10.0,  angleY = -20; //Rotation angles about x, y axes
float xmin, xmax, ymin, ymax;       //min, max values of  object coordinates

//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)
{
    ifstream fp_in;
    int ne;

    fp_in.open(fname, ios::in);
    if(!fp_in.is_open())
    {
        cout << "Error opening mesh file" << endl;
        exit(1);
    }

    fp_in.ignore(INT_MAX, '\n');                //ignore first line
    fp_in >> nvert >> nface >> ne;              // read number of vertices, polygons, edges (not used)

    x = new float[nvert];                        //create arrays
    z = new float[nvert];


    for(int i=0; i < nvert; i++)                         //read vertex list
        fp_in >> x[i] >> z[i];

    fp_in.close();
    cout << " File successfully read." << endl;
}

//-- Computes the min, max values of coordinates  -----------------------
void computeMinMax()
{
    xmin = xmax = x[0];
    ymin = ymax = y[0];
    for(int i = 1; i < nvert; i++)
    {
        if(x[i] < xmin) xmin = x[i];
        else if(x[i] > xmax) xmax = x[i];
        if(y[i] < ymin) ymin = y[i];
        else if(y[i] > ymax) ymax = y[i];
    }
}

//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()
{
    float lpos[4] = {10., 10., 10., 1.0};           //light's position

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 1, 20, 0, 0, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);    //set light position

    glRotatef(angleX, 1.0, 0.0, 0.0);           //rotate the object about the x-axis
    glRotatef(angleY, 0.0, 1.0, 0.0);           //rotate the object about the y-axis

    glColor3f(0., 1., 1.);

    for(int indx = 0; indx < nface; indx++)     //draw each face
    {
        normal(indx);
        if (nv[indx] == 3)  glBegin(GL_TRIANGLES);
        else                glBegin(GL_QUADS);
            glVertex3d(x[t1[indx]], y[t1[indx]], z[t1[indx]]);
            glVertex3d(x[t2[indx]], y[t2[indx]], z[t2[indx]]);
            glVertex3d(x[t3[indx]], y[t3[indx]], z[t3[indx]]);
            if(nv[indx]==4)
              glVertex3d(x[t4[indx]], y[t4[indx]], z[t4[indx]]);
        glEnd();
    }

    glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    float model_wid, model_hgt;
    loadMeshFile("track.off");         //Specify mesh file name here
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   //Background colour

    glEnable(GL_LIGHTING);                  //Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    computeMinMax();                            //Compute min, max values of x, y coordinates for defining camera frustum
    model_wid = xmax-xmin;                      //Model width and height
    model_hgt = ymax-ymin;
    xmin -= 0.2*model_wid;                      //Extend minmax window further by 20% of its size.
    xmax += 0.2*model_wid;
    ymin -= 0.2*model_hgt;
    ymax += 0.2*model_hgt;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(xmin, xmax, ymin, ymax, 10, 1000);  //The camera view volume
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angleY--;
    else if(key == GLUT_KEY_RIGHT) angleY++;
    else if (key == GLUT_KEY_UP) angleX--;
    else if (key == GLUT_KEY_DOWN) angleX++;
    glutPostRedisplay();
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Model3D");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
