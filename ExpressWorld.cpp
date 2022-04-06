//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: ExpressWorld.cpp
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include "loadBMP.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <string>
#include <cmath>


using namespace std;

GLuint txId[2];
float angle = 25, cam_hgt = 70, cam_rot = 0.1, cam_move_spd = 2, cam_dst = 200; //rotation angle, camera height


//--------------- GROUND PLANE ------------------------------------
// This is a square shaped region on the xz-plane of size 400x400 units
// centered at the origin.  This region is constructed using small quads
// of unit size, to facilitate rendering of spotlights
//-----------------------------------------------------------------
void floor()
{
    float white[4] = {1., 1., 1., 1.};
    float black[4] = {0};
    glColor3f(0.3f, 0.3f, 0.3f); //Dark grey
    glNormal3f(0.0, 1.0, 0.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    //The floor is made up of several tiny squares on a 400 x 400 grid. Each square has a unit size.
    glBegin(GL_QUADS);
    for(int i = -400; i < 500; i++)
    {
        for(int j = -400;  j < 500; j++)
        {
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glVertex3f(i+1, 0, j+1);
            glVertex3f(i+1, 0, j);
        }
    }
    glEnd();

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

//--------------- RAILWAY TRACK ------------------------------------
// A circular railway track of specified median radius and width
// The tracks are modelled using small quadrilateral segments each covering 5 deg arc
// The tracks have a height of 1 unit (refer to lab notes)
//-----------------------------------------------------------------
void tracks(float medRadius, float width)
{
    float inRad  = medRadius - width * 0.5;
    float outRad = medRadius + width * 0.5;
    float angle1,angle2, ca1,sa1, ca2,sa2;
    float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians
    float points[] = {0.0, -40.0, -1.0, -40.0, -2.0, -40.0, -3.0, -40.0, -4.0, -40.0, -5.0, -40.0, -6.0, -40.0, -7.0, -40.0, -8.0, -40.0, -9.0, -40.0, -10.0, -40.0, -11.0, -40.0, -12.0, -40.0, -13.0, -40.0, -14.0, -40.0, -15.0, -40.0, -16.0, -40.0, -17.0, -40.0, -18.0, -40.0, -19.0, -40.0, -20.0, -40.0, -21.0, -40.0, -22.0, -40.0, -23.0, -40.0, -24.0, -40.0, -25.0, -40.0, -26.0, -40.0, -27.0, -40.0, -28.0, -40.0, -29.0, -40.0, -30.0, -40.0, -31.0, -40.0, -32.0, -40.0, -33.0, -40.0, -34.0, -40.0, -35.0, -40.0, -36.0, -40.0, -37.0, -40.0, -38.0, -40.0, -39.0, -40.0, -40.0, -40.0, -41.0, -40.0, -42.0, -40.0, -43.0, -40.0, -44.0, -40.0, -45.0, -40.0, -46.0, -40.0, -47.0, -40.0, -48.0, -40.0, -49.0, -40.0, -50.0, -40.0, -51.0, -40.0, -52.0, -40.0, -53.0, -40.0, -54.0, -40.0, -55.0, -40.0, -56.0, -40.0, -57.0, -40.0, -58.0, -40.0, -59.0, -40.0, -60.0, -40.0, -60.9999, -39.9875, -61.9992, -39.95, -62.9972, -39.8876, -63.9933, -39.8002, -64.987, -39.6879, -65.9775, -39.5508, -66.9643, -39.3891, -67.9468, -39.2027, -68.9243, -38.9918, -69.8962, -38.7565, -70.8619, -38.497, -71.8208, -38.2135, -72.7724, -37.906, -73.7159, -37.5749, -74.6509, -37.2203, -75.5767, -36.8424, -76.4928, -36.4415, -77.3986, -36.0179, -78.2935, -35.5717, -79.177, -35.1033, -80.0485, -34.613, -80.9075, -34.101, -81.7534, -33.5677, -82.5857, -33.0134, -83.4039, -32.4385, -84.2075, -31.8434, -84.9959, -31.2283, -85.7687, -30.5937, -86.5254, -29.94, -87.2655, -29.2676, -87.9886, -28.5768, -88.6942, -27.8683, -89.3819, -27.1423, -90.0512, -26.3993, -90.7017, -25.6399, -91.3331, -24.8644, -91.9448, -24.0734, -92.5366, -23.2673, -93.1081, -22.4467, -93.6588, -21.6121, -94.1886, -20.764, -94.6969, -19.9029, -95.1836, -19.0293, -95.6483, -18.1439, -96.0907, -17.2471, -96.5105, -16.3395, -96.9076, -15.4217, -97.2816, -14.4943, -97.6322, -13.5579, -97.9594, -12.6129, -98.2628, -11.6601, -98.5423, -10.7, -98.7977, -9.73318, -99.0289, -8.76029, -99.2357, -7.78194, -99.418, -6.79872, -99.5756, -5.81124, -99.7085, -4.82014, -99.8166, -3.82603, -99.8998, -2.82952, -99.9581, -1.83125, -99.9913, -0.831828, -99.9996, 0.16811, -99.9829, 1.16794, -99.9413, 2.16705, -99.8746, 3.1648, -99.783, 4.16057, -99.6666, 5.15374, -99.5254, 6.14369, -99.3594, 7.1298, -99.1689, 8.11145, -98.9539, 9.08804, -98.7146, 10.0589, -98.451, 11.0236, -98.1634, 11.9813, -97.852, 12.9315, -97.5169, 13.8737, -97.1584, 14.8072, -96.7767, 15.7314, -96.3719, 16.6458, -95.9445, 17.5498, -95.4945, 18.4429, -95.0224, 19.3244, -94.5284, 20.1938, -94.0128, 21.0506, -93.476, 21.8943, -92.9182, 22.7243, -92.3399, 23.54, -91.7413, 24.3411, -91.1229, 25.1269, -90.4851, 25.8971, -89.8282, 26.651, -89.1527, 27.3883, -88.4589, 28.1085, -87.7474, 28.8111, -87.0185, 29.4958, -86.2728, 30.1619, -85.5106, 30.8093, -84.7325, 31.4373, -83.9389, 32.0458, -83.1303, 32.6342, -82.3073, 33.2022, -81.4704, 33.7494, -80.62, 34.2756, -79.7568, 34.7803, -78.8812, 35.2633, -77.9938, 35.7243, -77.0952, 36.1629, -76.1858, 36.5789, -75.2664, 36.9721, -74.3374, 37.3422, -73.3995, 37.6889, -72.4531, 38.0121, -71.4991, 38.3115, -70.5378, 38.587, -69.5699, 38.8383, -68.596, 39.0654, -67.6168, 39.2681, -66.6328, 39.4462, -65.6447, 39.5997, -64.653, 39.7284, -63.6585, 39.8323, -62.6616, 39.9114, -61.6631, 39.9654, -60.6636, 39.9945, -60.0, 40.0, -59.0, 40.0, -58.0, 40.0, -57.0, 40.0, -56.0, 40.0, -55.0, 40.0, -54.0, 40.0, -53.0, 40.0, -52.0, 40.0, -51.0, 40.0, -50.0, 40.0, -49.0, 40.0, -48.0, 40.0, -47.0, 40.0, -46.0, 40.0, -45.0, 40.0, -44.0, 40.0, -43.0, 40.0, -42.0, 40.0, -41.0, 40.0, -40.0, 40.0, -39.0, 40.0, -38.0, 40.0, -37.0, 40.0, -36.0, 40.0, -35.0, 40.0, -34.0, 40.0, -33.0, 40.0, -32.0, 40.0, -31.0, 40.0, -30.0, 40.0, -29.0, 40.0, -28.0, 40.0, -27.0, 40.0, -26.0, 40.0, -25.0, 40.0, -24.0, 40.0, -23.0, 40.0, -22.0, 40.0, -21.0, 40.0, -20.0, 40.0, -19.0, 40.0, -18.0, 40.0, -17.0, 40.0, -16.0, 40.0, -15.0, 40.0, -14.0, 40.0, -13.0, 40.0, -12.0, 40.0, -11.0, 40.0, -10.0, 40.0, -9.0, 40.0, -8.0, 40.0, -7.0, 40.0, -6.0, 40.0, -5.0, 40.0, -4.0, 40.0, -3.0, 40.0, -2.0, 40.0, -1.0, 40.0, 0.0, 40.0, 1.0, 40.0, 2.0, 40.0, 3.0, 40.0, 4.0, 40.0, 5.0, 40.0, 6.0, 40.0, 7.0, 40.0, 8.0, 40.0, 9.0, 40.0, 10.0, 40.0, 11.0, 40.0, 12.0, 40.0, 13.0, 40.0, 14.0, 40.0, 15.0, 40.0, 16.0, 40.0, 17.0, 40.0, 18.0, 40.0, 19.0, 40.0, 20.0, 40.0, 21.0, 40.0, 22.0, 40.0, 23.0, 40.0, 24.0, 40.0, 25.0, 40.0, 26.0, 40.0, 27.0, 40.0, 28.0, 40.0, 29.0, 40.0, 30.0, 40.0, 31.0, 40.0, 32.0, 40.0, 33.0, 40.0, 34.0, 40.0, 35.0, 40.0, 36.0, 40.0, 37.0, 40.0, 38.0, 40.0, 39.0, 40.0, 40.0, 40.0, 41.0, 40.0, 42.0, 40.0, 43.0, 40.0, 44.0, 40.0, 45.0, 40.0, 46.0, 40.0, 47.0, 40.0, 48.0, 40.0, 49.0, 40.0, 50.0, 40.0, 51.0, 40.0, 52.0, 40.0, 53.0, 40.0, 54.0, 40.0, 55.0, 40.0, 56.0, 40.0, 57.0, 40.0, 58.0, 40.0, 59.0, 40.0, 60.0, 40.0, 60.9999, 39.9875, 61.9992, 39.95, 62.9972, 39.8876, 63.9933, 39.8002, 64.987, 39.6879, 65.9775, 39.5508, 66.9643, 39.3891, 67.9468, 39.2027, 68.9243, 38.9918, 69.8962, 38.7565, 70.8619, 38.497, 71.8208, 38.2135, 72.7724, 37.906, 73.7159, 37.5749, 74.6509, 37.2203, 75.5767, 36.8424, 76.4928, 36.4415, 77.3986, 36.0179, 78.2935, 35.5717, 79.177, 35.1033, 80.0485, 34.613, 80.9075, 34.101, 81.7534, 33.5677, 82.5857, 33.0134, 83.4039, 32.4385, 84.2075, 31.8434, 84.9959, 31.2283, 85.7687, 30.5937, 86.5254, 29.94, 87.2655, 29.2676, 87.9886, 28.5768, 88.6942, 27.8683, 89.3819, 27.1423, 90.0512, 26.3993, 90.7017, 25.6399, 91.3331, 24.8644, 91.9448, 24.0734, 92.5366, 23.2673, 93.1081, 22.4467, 93.6588, 21.6121, 94.1886, 20.764, 94.6969, 19.9029, 95.1836, 19.0293, 95.6483, 18.1439, 96.0907, 17.2471, 96.5105, 16.3395, 96.9076, 15.4217, 97.2816, 14.4943, 97.6322, 13.5579, 97.9594, 12.6129, 98.2628, 11.6601, 98.5423, 10.7, 98.7977, 9.73318, 99.0289, 8.76029, 99.2357, 7.78194, 99.418, 6.79872, 99.5756, 5.81124, 99.7085, 4.82014, 99.8166, 3.82603, 99.8998, 2.82952, 99.9581, 1.83125, 99.9913, 0.831828, 99.9996, -0.16811, 99.9829, -1.16794, 99.9413, -2.16705, 99.8746, -3.1648, 99.783, -4.16057, 99.6666, -5.15374, 99.5254, -6.14369, 99.3594, -7.1298, 99.1689, -8.11145, 98.9539, -9.08804, 98.7146, -10.0589, 98.451, -11.0236, 98.1634, -11.9813, 97.852, -12.9315, 97.5169, -13.8737, 97.1584, -14.8072, 96.7767, -15.7314, 96.3719, -16.6458, 95.9445, -17.5498, 95.4945, -18.4429, 95.0224, -19.3244, 94.5284, -20.1938, 94.0128, -21.0506, 93.476, -21.8943, 92.9182, -22.7243, 92.3399, -23.54, 91.7413, -24.3411, 91.1229, -25.1269, 90.4851, -25.8971, 89.8282, -26.651, 89.1527, -27.3883, 88.4589, -28.1085, 87.7474, -28.8111, 87.0185, -29.4958, 86.2728, -30.1619, 85.5106, -30.8093, 84.7325, -31.4373, 83.9389, -32.0458, 83.1303, -32.6342, 82.3073, -33.2022, 81.4704, -33.7494, 80.62, -34.2756, 79.7568, -34.7803, 78.8812, -35.2633, 77.9938, -35.7243, 77.0952, -36.1629, 76.1858, -36.5789, 75.2664, -36.9721, 74.3374, -37.3422, 73.3995, -37.6889, 72.4531, -38.0121, 71.4991, -38.3115, 70.5378, -38.587, 69.5699, -38.8383, 68.596, -39.0654, 67.6168, -39.2681, 66.6328, -39.4462, 65.6447, -39.5997, 64.653, -39.7284, 63.6585, -39.8323, 62.6616, -39.9114, 61.6631, -39.9654, 60.6636, -39.9945, 60.0, -40.0, 59.0, -40.0, 58.0, -40.0, 57.0, -40.0, 56.0, -40.0, 55.0, -40.0, 54.0, -40.0, 53.0, -40.0, 52.0, -40.0, 51.0, -40.0, 50.0, -40.0, 49.0, -40.0, 48.0, -40.0, 47.0, -40.0, 46.0, -40.0, 45.0, -40.0, 44.0, -40.0, 43.0, -40.0, 42.0, -40.0, 41.0, -40.0, 40.0, -40.0, 39.0, -40.0, 38.0, -40.0, 37.0, -40.0, 36.0, -40.0, 35.0, -40.0, 34.0, -40.0, 33.0, -40.0, 32.0, -40.0, 31.0, -40.0, 30.0, -40.0, 29.0, -40.0, 28.0, -40.0, 27.0, -40.0, 26.0, -40.0, 25.0, -40.0, 24.0, -40.0, 23.0, -40.0, 22.0, -40.0, 21.0, -40.0, 20.0, -40.0, 19.0, -40.0, 18.0, -40.0, 17.0, -40.0, 16.0, -40.0, 15.0, -40.0, 14.0, -40.0, 13.0, -40.0, 12.0, -40.0, 11.0, -40.0, 10.0, -40.0, 9.0, -40.0, 8.0, -40.0, 7.0, -40.0, 6.0, -40.0, 5.0, -40.0, 4.0, -40.0, 3.0, -40.0, 2.0, -40.0, 1.0, -40.0};
    glColor4f(0.0, 0.0, 0.3, 1.0);
    glBegin(GL_QUADS);
    float radius = inRad;

    // Track segments




    for (int i = 0; i < 2; i++)   //Two parallel tracks (radius = inRad, outRad)
    {
        for (int i = 0; i < 492; i += 5)    //5 deg intervals
        {
            angle1 = i * toRad;       //Computation of angles, cos, sin etc
            angle2 = (i + 5) * toRad;
            ca1 = cos(angle1); ca2 = cos(angle2);
            sa1 = sin(angle1); sa2 = sin(angle2);
            x1 = (radius - 0.5)*sa1; z1 = (radius - 0.5)*ca1;
            x2 = (radius + 0.5)*sa1; z2 = (radius + 0.5)*ca1;
            x3 = (radius + 0.5)*sa2; z3 = (radius + 0.5)*ca2;
            x4 = (radius - 0.5)*sa2; z4 = (radius - 0.5)*ca2;

            glNormal3f(0., 1., 0.);       //Quad 1 facing up
            glVertex3f(x1, 1.0, z1);
            glVertex3f(x2, 1.0, z2);
            glVertex3f(x3, 1.0, z3);
            glVertex3f(x4, 1.0, z4);

            glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
            glVertex3f(x1, 0.0, z1);
            glVertex3f(x1, 1.0, z1);
            glNormal3f(-sa2, 0.0, -ca2);
            glVertex3f(x4, 1.0, z4);
            glVertex3f(x4, 0.0, z4);

            glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
            glVertex3f(x2, 1.0, z2);
            glVertex3f(x2, 0.0, z2);
            glNormal3f(sa2, 0.0, ca2);
            glVertex3f(x3, 0.0, z3);
            glVertex3f(x3, 1.0, z3);
        }
        radius = outRad;
    }
    glEnd();


}

//--------------- MODEL BASE --------------------------------------
// This is a common base for the locomotive and wagons
// The base is of rectangular shape (20 length x 10 width x 2 height)
// and has wheels and connectors attached.
//-----------------------------------------------------------------
void base()
{
    glColor4f(0.2, 0.2, 0.2, 1.0);   //Base color
    glPushMatrix();
      glTranslatef(0.0, 4.0, 0.0);
      glScalef(20.0, 2.0, 10.0);     //Size 20x10 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Connector between wagons
      glTranslatef(11.0, 4.0, 0.0);
      glutSolidCube(2.0);
    glPopMatrix();

    //4 Wheels (radius = 2 units)
    //x, z positions of wheels:
    float wx[4] = {  -8,   8,   -8,    8 };
    float wz[4] = { 5.1, 5.1, -5.1, -5.1 };
    glColor4f(0.5, 0., 0., 1.0);    //Wheel color
    GLUquadric *q = gluNewQuadric();   //Disc

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(wx[i], 2.0, wz[i]);
        gluDisk(q, 0.0, 2.0, 20, 2);
        glPopMatrix();
    }
}

//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine()
{
    base();

    //Cab
    glColor3f(0.0f, 0.1f, 0.1f);//Dark blue

    glPushMatrix();
      glTranslatef(7.0, 8.5, 0.0);
      glScalef(6.0, 7.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(6.0, 14.0, 0.0);
      glScalef(4.0, 4.0, 8.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //Boiler
    GLUquadric *q = gluNewQuadric();   //Cylinder
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0);
      glTranslatef(4.0, 10.0, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
      glTranslatef(0.0, 0.0, 14.0);
      gluDisk(q, 0.0, 5.0, 20, 3);
      glColor4f(1.0, 1.0, 0.0, 1.0);
      glTranslatef(0.0, 4.0, 0.2);
      gluDisk(q, 0.0, 1.0, 20,2);  //headlight!
    glPopMatrix();

}

//--------------- WAGON ----------------------------------
// This simple model of a rail wagon consists of the base,
// and a cube(!)
//--------------------------------------------------------
void wagon()
{
    base();

    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, txId[1]);

    glColor3f(0.0f, 0.5f, 1.0f);
    glPushMatrix();
      glTranslatef(0.0, 10.0, 0.0);
      glScalef(20.0, 10.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();


}
//------ Draw train station  ------------------------
float depth = 64;
float x;
float z;
GLenum light;
void drawStationEnd(float depth, float nz)

{
    float nx = 0, ny = 0;
    glBegin(GL_POLYGON);
        glNormal3f(nx, ny, nz);
        glVertex3f(0, 0, 64); // 0 0 64
        glVertex3f(32, 0, 64); // 32 0 64
        glVertex3f(32, 4, 64); // 32 4 64
        glVertex3f(0, 4, 64); // 0 4 64
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3f(nx, ny, nz);
        glVertex3f(32, 4, 64); // 32 4 64
        glVertex3f(32, 28, 64);  // 32 28 64
        glVertex3f(21, 28, 64); // 21 28 64
        glVertex3f(21, 4, 64); // 21 4 64
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3f(nx, ny, nz);
        glVertex3f(32, 28, 64);  // 32 28 64
        glVertex3f(16, 36, 64); // 16 36 64
        glVertex3f(0, 28, 64); // 0 28 64
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
}

void drawStationBody(float depth)
{
    glBegin(GL_QUAD_STRIP);
        glTexCoord2f(2/3, 1);
        glNormal3f(0, -1, 0);
        glVertex3f(0, 0, 64);
        glVertex3f(0, 0, 0);
        glVertex3f(32, 0, 64);
        glVertex3f(32, 0, 0);

        glNormal3f(1, 0, 0);
        glVertex3f(32, 28, 64);
        glVertex3f(32, 28, 0);

        glNormal3f(16, -8, 0);
        glVertex3f(16, 36, 64);
        glVertex3f(16, 36, 0);

        glNormal3f(16, 8, 0);
        glVertex3f(0, 28, 64);
        glVertex3f(0, 28, 0);

        glNormal3f(0, -1, 0);
        glTexCoord2f(2/3, 0);
        glVertex3f(21, 28, 64);
        glTexCoord2f(1/3, 0);
        glVertex3f(21, 28, 0);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(2/3, 0);
        glVertex3f(21, 4, 64);
        glTexCoord2f(1/3, 0);
        glVertex3f(21, 4, 0);

        glNormal3f(0, 1, 0);
        glTexCoord2f(2/3, 1);
        glVertex3f(0, 4, 64);
        glTexCoord2f(1/3, 1);
        glVertex3f(0, 4, 0);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(2/3, 1);
        glVertex3f(0, 0, 64);
        glTexCoord2f(1/3, 1);
        glVertex3f(0, 0, 0);

    glEnd();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
}
// tunnels
float TUNNEL_X = 40;

// tracks
int TRACK_LENGTH = 800;
float TRACK_X = 00.0;
float TRACK_Y = 0.0;
float TRACK_Z = 180.0;
void drawStation() {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    glTranslatef(130, 0, 30);
    glRotatef(-20, 0, 1, 0);

    glColor4f(1.0f, 0.5f, 0.0f, 0.0f); // Orange/Brown
    drawStationEnd(0, -1);
    drawStationEnd(depth, 1);
    drawStationBody(depth);

    glEnable(GL_LIGHTING);
    float lgt2_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float spotDir[] = { -0.5, -1.0, 0 };

    glTranslatef(21/2, 28, 64/2);

}


//------ Draw tunnel  ------------------------

void drawTunnel(void) {

    // Tunnels for tracks
    glPushMatrix();
        glTranslatef(TUNNEL_X, 7.0, 50);
        glScalef(15.0f, 2.0f, 1.0f);
        glRotatef(90.0, 0, 1.0, 0);
        glutSolidTorus(5.5, 30.7, 40, 40);
    glPopMatrix();

}

bool STOP_LIGHT = true;

//------ Draw traffic light  ------------------------
void drawTrafficLight() {
    if (STOP_LIGHT) {
        // red
        glColor3f(1.0, 0.1, 0.1);
    } else {
        // green
        glColor3f(0.0, 1.0, 0.1);
    }
    glPushMatrix();
        glTranslatef(-50, 15, 170);
        glutSolidSphere(3.0, 20.0, 20.0);
    glPopMatrix();
}
//------Function to load a texture in bmp format  ------------------------
void loadTexture()
{
    glGenTextures(2, txId);                // Create a Texture object
    glBindTexture(GL_TEXTURE_2D, txId[0]);     //Use this texture
    loadBMP("brick.bmp"); //Station
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
    loadBMP("train-texture1.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // *** top ***
    glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture name for the following OpenGL texture
    loadBMP("L1.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}


//------Function to draw skybox texture in bmp format  ------------------------

void drawSkybox()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    /////////////////////// TOP //////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-400, 200, -400);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(400, 200, -400);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(400, 200, 400);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-400, 200, 400);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}


//---------------------------------------------------------------------
void initialize(void)
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

   loadTexture();

//  Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glEnable(GL_TEXTURE_2D);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection


}
//---------------------- Timer -----------------------------------
float theta = 0;
void myTimer(int value)
{
    theta ++;
    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);
}


//------Event handler for special key events -------------------------------------
void special(int key, int x, int y)
{
    if(key==GLUT_KEY_UP) cam_hgt += 1;
    else if(key==GLUT_KEY_DOWN) cam_hgt -= 1;

    if (key==GLUT_KEY_LEFT) cam_rot -= 0.1;
    else if(key==GLUT_KEY_RIGHT) cam_rot+= 0.1;
    glutPostRedisplay();
}

void normalKeys(unsigned char key, int x, int y)
{
    if (key == 'w') cam_dst -= cam_move_spd;
    else if (key == 's') cam_dst += cam_move_spd;
    glutPostRedisplay();
}

//-------------------------------------------------------------------
void display(void)
{
   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)

   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(cam_dst * sin(cam_rot), cam_hgt, cam_dst*cos(cam_rot), 0, 0., 0., 0, 1., 0.);

   //gluLookAt(0, cam_hgt, 350, cam_rot, 0, 0,   0, 1, 0);
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

   glLightf(-10, 14, 0);

    glPushMatrix();
        glRotatef(theta+41.5, 0., 1., 0);
        glTranslatef(0, 1, -120);
        engine();
    glPopMatrix();

   floor();

   tracks(120, 10);  //mean radius 120 units, width 10 units

    glEnable(GL_TEXTURE_2D);
    drawSkybox();

    glPushMatrix();
        glRotatef(theta, 0., 1., 0);
        glTranslatef(0, 1, -120);
        wagon();
    glPopMatrix();

    glPushMatrix();
        glRotatef(theta+10.5, 0., 1., 0);
        glTranslatef(0, 1, -120);
        wagon();
    glPopMatrix();

    glPushMatrix();
        glRotatef(theta+21, 0., 1., 0);
        glTranslatef(0, 1, -120);
        wagon();
    glPopMatrix();

    glPushMatrix();
        glRotatef(theta+31.5, 0., 1., 0);
        glTranslatef(0, 1, -120);
        wagon();
    glPopMatrix();


    drawStation();
    //drawTunnel();
    drawTrafficLight();

   glutSwapBuffers();   //Useful for animation
}

//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Faversham Express");
   initialize ();
   glutDisplayFunc(display);
   glutTimerFunc(50, myTimer, 0);
   glutSpecialFunc(special);
    glutKeyboardFunc(normalKeys);
   glutMainLoop();
   return 0;
}
