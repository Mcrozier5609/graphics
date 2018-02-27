/*
 *  Michael Crozier Final Project, Animal Crossing
 *
 *
 *  Key bindings:
 *  l          Toggles lamp lighting
 *  t/T        Turns on/off the television
 *  v/V        Changes channel of TV
 *  s/S        Change season
 *  p          Toggles inside or outside view of house
 *  +/-        Change field of view of perspective
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

int mode=1;               //  viewing mode
int tmode=0;              //  Texture mode
int move=1;               //  Move light
int th=0;                 //  Azimuth of view angle
int ph=0;                 //  Elevation of view angle
int fov=90;               //  Field of view (for perspective)
int light=0;              //  Lighting
double asp=1;             //  Aspect ratioi	
double dim=15.0;          //  Size of world
// Light values
float sco     = 180;      //  Spot cutoff angle
float Exp     =   0;      //  Spot exponent
int at0       = 100;      //  Constant  attenuation %
int at1       =   0;      //  Linear    attenuation %
int at2       =   0;      //  Quadratic attenuation %
int one       =   1;      // Unit value
int distance  =  30;      // Light distance
int inc       =  10;      // Ball increment
int smooth    =   1;      // Smooth/Flat shading
int local     =   0;      // Local Viewer Model
int emission  =   0;      // Emission intensity (%)
int ambient   =  30;      // Ambient intensity (%)
int diffuse   = 100;      // Diffuse intensity (%)
int specular  =   0;      // Specular intensity (%)
int shininess =   0;      // Shininess (power of two)
float shiny   =   1;      // Shininess (value)
int zh        =  90;      // Light azimuth
int zhz       =  45;
int bobbing   =   1;      // Whether the light is bobbing spookily
int magnitude =   8;      // Light bobbing magnitude
int frequency =   4;      // Light bobbing frequency
float zlight  =   0;      // Elevation of light
//textures
unsigned int texture[18]; // Texture names
//tv stuff
int watching  =   0;      //toggle television on and off
int channel   =   0;      //tv channel
int seconds   =   0;      //timer for tv
//tv channels, in order: Weather, cartoon, nature, gameshow, action movie, static
double channelTexture[24][8] = {
   {0.017,0.75, 0.082,0.75, 0.082,0.89, 0.017,0.89},
   {0.082,0.75, 0.147,0.75, 0.147,0.89, 0.082,0.89},
   {0.147,0.75, 0.212,0.75, 0.212,0.89, 0.147,0.89},
   {0.1,0.57, 0.164,0.57, 0.164,0.71, 0.1,0.71},

   {0.652,0.39, 0.717,0.39, 0.717,0.53, 0.652,0.53},
   {0.717,0.39, 0.782,0.39, 0.782,0.53, 0.717,0.53},
   {0.785,0.39, 0.850,0.39, 0.850,0.53, 0.785,0.53},
   {0.850,0.39, 0.915,0.39, 0.915,0.53, 0.850,0.53},

   {0.652,0.57, 0.717,0.57, 0.717,0.71, 0.652,0.71},
   {0.717,0.57, 0.782,0.57, 0.782,0.71, 0.717,0.71},
   {0.785,0.57, 0.850,0.57, 0.850,0.71, 0.785,0.71},
   {0.850,0.57, 0.915,0.57, 0.915,0.71, 0.850,0.71},

   {0.652,0.75, 0.717,0.75, 0.717,0.89, 0.652,0.89},
   {0.717,0.75, 0.782,0.75, 0.782,0.89, 0.717,0.89},
   {0.785,0.75, 0.850,0.75, 0.850,0.89, 0.785,0.89},
   {0.850,0.75, 0.915,0.75, 0.915,0.89, 0.850,0.89},

   {0.652,0.04, 0.717,0.04, 0.717,0.18, 0.652,0.18},
   {0.717,0.04, 0.782,0.04, 0.782,0.18, 0.717,0.18},
   {0.785,0.04, 0.850,0.04, 0.850,0.18, 0.785,0.18},
   {0.915,0.04, 0.980,0.04, 0.980,0.18, 0.915,0.18},

   {0.302,0.75, 0.367,0.75, 0.367,0.89, 0.302,0.89},
   {0.367,0.75, 0.432,0.75, 0.432,0.89, 0.367,0.89},
   {0.432,0.75, 0.497,0.75, 0.497,0.89, 0.432,0.89},
   {0.367,0.75, 0.432,0.75, 0.432,0.89, 0.367,0.89}
};
//seasons stuff
int season    =   0;      //summer or winter toggle
double particles[500][4]; //array of snow particles
int count     = 0;        //current number of snow particles

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 double r, double g, double b,
                 int textureNum, int normals)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[textureNum]);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(r,g,b);
   glNormal3f( 0, 0, 1*normals);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f( 0, 0, -1*normals);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f( +1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(r,g,b);
   glNormal3f( -1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f( 0, +1*normals, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0, -one*normals, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void Vertex(double th,double ph, double r, double g, double b)
{
   glColor3f(r,g,b);
   glNormal3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
//Draw vertex in shifted polar coordinates
static void VertexCylinder(double th,double ph, double radius, double y, double r, double g, double b)
{
   glColor3f(r,g,b);
   glNormal3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
   //glTexCoord2d(th/90,ph+y);
   glVertex3d(Sin(th)*Cos(ph)*radius , Sin(ph)+y , Cos(th)*Cos(ph)*radius);
}

static void sphere(double x,double y,double z,double r, double red, double g, double b,
                   int textureNum)
{
   const int d=5;
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[textureNum]);
   glColor3d(red,g,b);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph, red,g,b);
         Vertex(th,ph+d, red,g,b);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

//Includes circle caps
static void cylinder(double x, double y, double z,
		double radius, double length,
                double alpha, double beta, double gamma,
                double r, double g, double b,
                double amount,
                int textureNum, double texWidth, double texHeight)
{
   const int d=5;
   int th;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(alpha, 1,0,0);
   glRotated(beta, 0,1,0);
   glRotated(gamma, 0,0,1);
   glScaled(radius, length, radius);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[textureNum]);

   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=amount;th+=d) {
      glTexCoord2d(th/texWidth,0);
      VertexCylinder(th,0, 1,0, r,g,b);
      glTexCoord2d(th/texWidth,1/texHeight);
      VertexCylinder(th,0, 1,3, r,g,b);
   }
   glEnd();

   glPushMatrix();
   glRotated(90, 0,1,0);
   int i;
   glColor3f(r,g,b);
   int triangleAmount = 72; //# of triangles used to draw circle

   //GLfloat radius = 0.8f; //radius
   GLfloat twicePi = (amount/360)*2*3.1415826;

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f( 0, -1, 0);
   glTexCoord2d(0,0);
   glVertex3f(0, 0, 0); // center of circle
   for(i = 0; i <= triangleAmount;i++) {
      glTexCoord2d(cos(i *  twicePi / triangleAmount),sin(i *  twicePi / triangleAmount));
      glVertex3f(
         (cos(i *  twicePi / triangleAmount)), 
         0,
         (sin(i *  twicePi / triangleAmount))
         );
   }
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f( 0, +1, 0);
   glTexCoord2d(0,0);
   glVertex3f(0, 3, 0); // center of circle
   for(i = 0; i <= triangleAmount;i++) {
      glTexCoord2d(cos(i *  twicePi / triangleAmount),sin(i *  twicePi / triangleAmount));
      glVertex3f(
         (cos(i *  twicePi / triangleAmount)), 
         3,
         (sin(i *  twicePi / triangleAmount))
         );
   }
   glEnd();
   glPopMatrix();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void ring(double x, double y, double z,
		 double radius, double size,
                 double alpha, double beta, double gamma,
                 double r, double g, double b,
                 int textureNum, double texWidth, double texHeight)
{
   const int d=5;
   int th;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(alpha, 1,0,0);
   glRotated(beta, 0,1,0);
   glRotated(gamma, 0,0,1);
   glScaled(radius, size, radius);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[textureNum]);

   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d) {
      glTexCoord2d(th/texWidth,0);
      VertexCylinder(th,0, 1,0, r,g,b);
      glTexCoord2d(th/texWidth,1/texHeight);
      VertexCylinder(th,0, 1,3, r,g,b);
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d) {
      glTexCoord2d(th/texWidth,0);
      VertexCylinder(th,0, 1.5,0, r,g,b);
      glTexCoord2d(th/texWidth,1/texHeight);
      VertexCylinder(th,0, 1.5,3, r,g,b);
   }
   glEnd();

   int i;
   glColor3f(r,g,b);
   int triangleAmount = 72; //# of triangles used to draw circle

   //GLfloat radius = 0.8f; //radius
   GLfloat twicePi = 2.0f * 3.1415826;

   glBegin(GL_QUAD_STRIP);
   glNormal3f( 0, -1, 0);
   for(i = 0; i <= triangleAmount;i++) {
      glTexCoord2d(cos(i *  twicePi / triangleAmount),sin(i *  twicePi / triangleAmount));
      glVertex3f(
         (cos(i *  twicePi / triangleAmount)), 
         0,
         (sin(i *  twicePi / triangleAmount))
         );
      glTexCoord2d(1.5*cos(i *  twicePi / triangleAmount),1.5*sin(i *  twicePi / triangleAmount));
      glVertex3f(
         (1.5 * cos(i *  twicePi / triangleAmount)),
         0,
         (1.5 * sin(i *  twicePi / triangleAmount))
         );
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   glNormal3f( 0, +1, 0);
   for(i = 0; i <= triangleAmount;i++) {
      glTexCoord2d(cos(i *  twicePi / triangleAmount),sin(i *  twicePi / triangleAmount));
      glVertex3f(
         (cos(i *  twicePi / triangleAmount)),
         3,
         (sin(i *  twicePi / triangleAmount))
         );
      glTexCoord2d(1.5*cos(i *  twicePi / triangleAmount),1.5*sin(i *  twicePi / triangleAmount));
      glVertex3f(
         (1.5 * cos(i *  twicePi / triangleAmount)), 
         3,
         (1.5 * sin(i *  twicePi / triangleAmount))
         );
   }
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void cone(double x, double y, double z,
		double radius, double length,
                double alpha, double beta, double gamma,
                double r, double g, double b,
                int textureNum, double texWidth, double texHeight)
{
   const int d=5;
   int th;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(alpha, 1,0,0);
   glRotated(beta, 0,1,0);
   glRotated(gamma, 0,0,1);
   glScaled(radius, length, radius);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[textureNum]);

   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d) {
      glTexCoord2d(th/texWidth,0);
      VertexCylinder(th,0, 1,0, r,g,b);
      glTexCoord2d(th/texWidth,1/texHeight);
      glVertex3d(0,length,0);
   }
   glEnd();

   int i;
   glColor3f(r,g,b);
   int triangleAmount = 72; //# of triangles used to draw circle

   //radius
   GLfloat twicePi = 2.0f * 3.1415826;

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f( 0, -1, 0);
   glTexCoord2d(0,0);
   glVertex3f(0, 0, 0); // center of circle
   for(i = 0; i <= triangleAmount;i++) {
      glTexCoord2d(cos(i *  twicePi / triangleAmount),sin(i *  twicePi / triangleAmount));
      glVertex3f(
         (cos(i *  twicePi / triangleAmount)), 
         0,
         (sin(i *  twicePi / triangleAmount))
         );
   }
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void openConeFrustum(double x, double y, double z,
		            double radius, double length,
                            double alpha, double beta, double gamma,
                            double r, double g, double b,
                            int textureNum, double texWidth, double texHeight)
{
   const int d=5;
   int th;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(alpha, 1,0,0);
   glRotated(beta, 0,1,0);
   glRotated(gamma, 0,0,1);
   glScaled(radius, length, radius);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[textureNum]);

   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d) {
      glTexCoord2d(th/texWidth,0);
      VertexCylinder(th,0, 3,0, r,g,b);
      glTexCoord2d(th/texWidth,1/texHeight);
      VertexCylinder(th,0, 1,3, r,g,b);
   }
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void house(int normals)
{
   //walls
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glEnable(GL_POLYGON_OFFSET_FILL);

   glPolygonOffset(1,1);
   //cube(0,0,0, 10,10,10, 0, 1,1,1, 1, normals);
   //  Save transformation
   glPushMatrix();
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,1,1);
   glNormal3f( 0, 0, 1*normals);
   glTexCoord2f(0,0); glVertex3f(-10,-10, 10);
   glTexCoord2f(1,0); glVertex3f(+10,-10, 10);
   glTexCoord2f(1,1); glVertex3f(+10,+10, 10);
   glTexCoord2f(0,1); glVertex3f(-10,+10, 10);
   //  Back
   glNormal3f( 0, 0, -1*normals);
   glTexCoord2f(0,0); glVertex3f(+10,-10,-10);
   glTexCoord2f(1,0); glVertex3f(-10,-10,-10);
   glTexCoord2f(1,1); glVertex3f(-10,+10,-10);
   glTexCoord2f(0,1); glVertex3f(+10,+10,-10);
   //  Right with hole for window
   glNormal3f( +1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+10,-10,+10);
   glTexCoord2f(1,0); glVertex3f(+10,-10,-10);
   glTexCoord2f(1,1); glVertex3f(+10,-10/3,-10);
   glTexCoord2f(0,1); glVertex3f(+10,-10/3,+10);
   glNormal3f( +1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+10,+10/3,+10);
   glTexCoord2f(1,0); glVertex3f(+10,+10/3,-10);
   glTexCoord2f(1,1); glVertex3f(+10,+10,-10);
   glTexCoord2f(0,1); glVertex3f(+10,+10,+10);
   glNormal3f( +1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+10,-10/3,+10);
   glTexCoord2f(1,0); glVertex3f(+10,-10/3,+10/3);
   glTexCoord2f(1,1); glVertex3f(+10,+10/3,+10/3);
   glTexCoord2f(0,1); glVertex3f(+10,+10/3,+10);
   glNormal3f( +1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+10,-10/3,-10/3);
   glTexCoord2f(1,0); glVertex3f(+10,-10/3,-10);
   glTexCoord2f(1,1); glVertex3f(+10,+10/3,-10);
   glTexCoord2f(0,1); glVertex3f(+10,+10/3,-10/3);
   //  Left
   glNormal3f( -1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-10,-10,-10);
   glTexCoord2f(1,0); glVertex3f(-10,-10,+10);
   glTexCoord2f(1,1); glVertex3f(-10,+10,+10);
   glTexCoord2f(0,1); glVertex3f(-10,+10,-10);
   //  Top
   glNormal3f( 0, +1*normals, 0);
   glTexCoord2f(0,0); glVertex3f(-10,+10,+10);
   glTexCoord2f(1,0); glVertex3f(+10,+10,+10);
   glTexCoord2f(1,1); glVertex3f(+10,+10,-10);
   glTexCoord2f(0,1); glVertex3f(-10,+10,-10);
   //  Bottom
   glNormal3f( 0, -one*normals, 0);
   glTexCoord2f(0,0); glVertex3f(-10,-10,-10);
   glTexCoord2f(1,0); glVertex3f(+10,-10,-10);
   glTexCoord2f(1,1); glVertex3f(+10,-10,+10);
   glTexCoord2f(0,1); glVertex3f(-10,-10,+10);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_POLYGON_OFFSET_FILL);
   //roof
   if(season == 3 && normals == 1)
   {
      glBindTexture(GL_TEXTURE_2D,texture[15]);
      glColor3f(1,1,1);
   }
   else
   {
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      glColor3f(1,0,0);
   }

   glBegin(GL_QUADS);
   //top right
   glNormal3f(sqrt(2)*normals,sqrt(2)*normals,0);
   glTexCoord2f(0,0); glVertex3f(+10,+10, +10);
   glTexCoord2f(1,0); glVertex3f(+10,+10, -10);
   glTexCoord2f(1,1); glVertex3f(+0 ,+15, -10);
   glTexCoord2f(0,1); glVertex3f(+0 ,+15, +10);
   //top left
   glNormal3f(-sqrt(2)*normals,sqrt(2)*normals,0);
   glTexCoord2f(0,0); glVertex3f(-10,+10, -10);
   glTexCoord2f(1,0); glVertex3f(-10,+10, +10);
   glTexCoord2f(1,1); glVertex3f(+0 ,+15, +10);
   glTexCoord2f(0,1); glVertex3f(+0 ,+15, -10);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glColor3f(1,0,0);
   glBegin(GL_QUADS);
   //front
   glNormal3f( 0, 0, 1*normals);
   glTexCoord2f(0,0); glVertex3f(-10,+10, +10);
   glTexCoord2f(1,0); glVertex3f(+10,+10, +10);
   glTexCoord2f(0.5,0.5); glVertex3f(+0 ,+15, +10);
   glTexCoord2f(0,0); glVertex3f(-10,+10, +10);
   //back
   glNormal3f(0,0,-1*normals);
   glTexCoord2f(0,0); glVertex3f(+10,+10, -10);
   glTexCoord2f(1,0); glVertex3f(-10,+10, -10);
   glTexCoord2f(0.5,0.5); glVertex3f(+0 ,+15, -10);
   glTexCoord2f(0,0); glVertex3f(+10,+10, -10);
   //door
   glNormal3f(0,0,1*normals);
   glTexCoord2f(0,0); glVertex3f(-3 ,-10, +10);
   glTexCoord2f(1,0); glVertex3f(+3 ,-10, +10);
   glTexCoord2f(1,1); glVertex3f(+3 ,+0 , +10);
   glTexCoord2f(0,1); glVertex3f(-3 ,+0 , +10);
   glEnd();
   //rug
   //if (normals == -1)
   //{
   glFrontFace(GL_CW);
     glCullFace(GL_BACK);
     glEnable(GL_CULL_FACE);
      //rug
      glBindTexture(GL_TEXTURE_2D,texture[3]);
      //  square
      glBegin(GL_QUADS);
      glColor3f(1,1,1);
      glNormal3f( 0, +1, 0);
      glTexCoord2f(0,0); glVertex3f(-8,-10,-8);
      glTexCoord2f(1,0); glVertex3f(+8,-10,-8);
      glTexCoord2f(1,1); glVertex3f(+8,-10,+8);
      glTexCoord2f(0,1); glVertex3f(-8,-10,+8);
      glEnd();
      
      //poster 1
      glBindTexture(GL_TEXTURE_2D,texture[4]);
      glBegin(GL_QUADS);
      glNormal3f( 0, 0, +1);
      glTexCoord2f(0.2,0.9); glVertex3f(-2,+2.33,-10);
      glTexCoord2f(0.8,0.9); glVertex3f(+2,+2.33,-10);
      glTexCoord2f(0.8,0.1); glVertex3f(+2,-2.33,-10);
      glTexCoord2f(0.2,0.1); glVertex3f(-2,-2.33,-10);
      glEnd();
   if (normals == 1)
      glDisable(GL_CULL_FACE);
   //}
   //window
   glColor4f(1,1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   glBegin(GL_QUADS);
   glNormal3f( +1*normals, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+10,-10/3,+10/3);
   glTexCoord2f(1,0); glVertex3f(+10,-10/3,-10/3);
   glTexCoord2f(1,1); glVertex3f(+10,+10/3,-10/3);
   glTexCoord2f(0,1); glVertex3f(+10,+10/3,+10/3);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glDisable(GL_CULL_FACE);
}

static void armchair(double x, double y, double z,
                     double size,
                     double theta)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(theta, 0,1,0);
   glScaled(size, size, size);
   //legs
   cylinder(-5,0,+5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,+5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,-5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(-5,0,-5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   //cushion
   cube(0,5,0, 7,2,7, 0, 0.3,0.3,0.3, 8, 1);
   //right arm
   cube(-5,8,0, 2,2,7, 0, 0.3,0.3,0.3, 8, 1);
   //left arm
   cube(+5,8,0, 2,2,7, 0, 0.3,0.3,0.3, 8, 1);
   //back
   cube(0,10,-5, 7,5,2, 0, 0.3,0.3,0.3, 8, 1);
   glPopMatrix();
}

static void tableChair(double x, double y, double z,
                       double size, double theta)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(theta, 0,1,0);
   glScaled(size, size, size);
   //legs
   cylinder(-5,0,+5, 1,2, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,+5, 1,2, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,-5, 1,2, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(-5,0,-5, 1,2, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   //seat
   cube(0,7,0, 7,1,7, 0, 1,1,1, 1,1);
   //back
   cube(-5,13,-5, 1,7,0.9, 0, 0.9,0.5,0.1, 1,1);
   cube(+5,13,-5, 1,7,0.9, 0, 0.9,0.5,0.1, 1,1);
   cube(0,14,-5, 1,8,0.9, 0, 0.9,0.5,0.1, 1,1);
   ring(0,16,-6, 5,0.7, 90,0,0, 1,1,1, 1,90,1);
   glPopMatrix();
}

static void lamp(double x, double y, double z,
                 double size)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(size, size, size);
   cone(0,0,0, 3,1.5, 0,0,0, 0.9,0.5,0.3, 6,360,1);
   cylinder(0,0,0, 1,5, 0,0,0, 0.9,0.5,0.3, 360, 6,360,6);
   openConeFrustum(0,14,0, 1,1.2, 0,0,0, 1,1,1, 7,360,1);
   glPopMatrix();
}

static void tv(double x, double y, double z,
               double size, double theta)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(theta, 0,1,0);
   glScaled(size, size, size);
   //legs
   cylinder(-5,0,+5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,+5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,-5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(-5,0,-5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   //box
   cube(0,9,0, 7,7,7, 0, 0.1,0.1,0.1, 6, 1);
   //screen
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(-1,1);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   if (watching == 1)
   {
      glColor3f(1,1,1);
      glBindTexture(GL_TEXTURE_2D,texture[16]);
      glBegin(GL_QUADS);
      glNormal3f( 0, 0, -1);
      int screen = 4*channel+seconds;
      glTexCoord2f(channelTexture[screen][0],channelTexture[screen][1]); glVertex3f(+6,+3,-7);
      glTexCoord2f(channelTexture[screen][2],channelTexture[screen][3]); glVertex3f(-6,+3,-7);
      glTexCoord2f(channelTexture[screen][4],channelTexture[screen][5]); glVertex3f(-6,+15,-7);
      glTexCoord2f(channelTexture[screen][6],channelTexture[screen][7]); glVertex3f(+6,+15,-7);
      glEnd();
   }
   else
   {
      glColor3f(0.1,0.1,0.1);
      glBindTexture(GL_TEXTURE_2D,texture[0]);
      glBegin(GL_QUADS);
      glNormal3f( 0, 0, -1);
      glTexCoord2f(0,0); glVertex3f(+6,+3,-7);
      glTexCoord2f(1,0); glVertex3f(-6,+3,-7);
      glTexCoord2f(1,1); glVertex3f(-6,+15,-7);
      glTexCoord2f(0,1); glVertex3f(+6,+15,-7);
      glEnd();
   }
   glDisable(GL_POLYGON_OFFSET_FILL);
   //antena
   sphere(0, +14.5, 0, 2.5, 1,1,1, 6);
   cylinder(0,+15,0, 0.5,2.0, 0,0,45, 0.9,0.5,0.1, 360, 6,360,1);
   cylinder(0,+15,0, 0.5,2.0, 0,0,-45, 0.9,0.5,0.1, 360, 6,360,1);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void table(double x, double y, double z,
                  double size, double theta)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(theta, 0,1,0);
   glScaled(size, size, size);
   //legs
   cylinder(-5,0,+5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,+5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(+5,0,-5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   cylinder(-5,0,-5, 1,1.5, 0,0,0, 0.9,0.5,0.1, 360, 1,360,1);
   //top
   cylinder(0,4,0, 10,0.5, 0,0,0, 0.9,0.5,0.3, 360, 1,90,3.5);
   glPopMatrix();
}

static void mailBox(double x, double y, double z,
                    double size, double theta)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(theta, 0,1,0);
   glScaled(size, size, size);
   //post
   cube(0,0,0, 1,10,1, 0, 1,1,1, 1, 1);
   //box
   cube(0,10,0, 3,2,5.5, 0, 1,1,1, 6, 1);
   cylinder(0,12,-5.5, 3,3.67, 0,90,90, 1,1,1, 180, 6,360,1);
   //flag
   cube(3,14,3, 0.1,3,1, 0, 1,0,0, 6, 1);
   cube(3,16,2, 0.1,1,2, 0, 1,0,0, 6, 1);
   glPopMatrix();
}

static void fence(double x, double y, double z,
                  double size, double theta)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(theta, 0,1,0);
   glScaled(size, size, size);
   cube(0,0,0, 1,4,0.3, 0, 1,1,1, 1, 1);
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[1]);

   glBegin(GL_QUADS);
   //front
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,+4,+0.3);
   glTexCoord2f(1,0); glVertex3f(+1,+4,+0.3);
   glTexCoord2f(0.5,0.5); glVertex3f(+0,+5,+0.3);
   glTexCoord2f(0,0); glVertex3f(-1,+4,+0.3);
   //back
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,+4,-0.3);
   glTexCoord2f(1,0); glVertex3f(-1,+4,-0.3);
   glTexCoord2f(0.5,0.5); glVertex3f(+0 ,+5,-0.3);
   glTexCoord2f(0,0); glVertex3f(+1,+4,-0.3);
   //top right
   glNormal3f(sqrt(2),sqrt(2),0);
   glTexCoord2f(0,0); glVertex3f(+1,+4,+0.3);
   glTexCoord2f(1,0); glVertex3f(+1,+4,-0.3);
   glTexCoord2f(1,1); glVertex3f(+0,+5,-0.3);
   glTexCoord2f(0,1); glVertex3f(+0,+5,+0.3);
   //top left
   glNormal3f(-sqrt(2),sqrt(2),0);
   glTexCoord2f(0,0); glVertex3f(-1,+4,-0.3);
   glTexCoord2f(1,0); glVertex3f(-1,+4,+0.3);
   glTexCoord2f(1,1); glVertex3f(+0,+5,+0.3);
   glTexCoord2f(0,1); glVertex3f(+0,+5,-0.3);
   glEnd();
   //upper cross bar
   cube(0,2,0.3, 2,1,0.3, 0, 1,1,1, 1, 1);
   //lower cross bar
   cube(0,-1,0.3, 2,1,0.3, 0, 1,1,1, 1, 1);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void pickitFence()
{
   //front
   fence(6,-7,15, 0.75, 0);
   fence(9,-7,15, 0.75, 0);
   fence(12,-7,15, 0.75, 0);
   
   fence(-6,-7,15, 0.75, 0);
   fence(-9,-7,15, 0.75, 0);
   fence(-12,-7,15, 0.75, 0);
   //right
   fence(13,-7,14, 0.75, 90);
   fence(13,-7,11, 0.75, 90);
   fence(13,-7,8, 0.75, 90);
   fence(13,-7,5, 0.75, 90);
   fence(13,-7,2, 0.75, 90);
   fence(13,-7,-1, 0.75, 90);
   fence(13,-7,-4, 0.75, 90);
   fence(13,-7,-7, 0.75, 90);
   fence(13,-7,-10, 0.75, 90);
   //back
   fence(12,-7,-11, 0.75, 180);
   fence(9,-7,-11, 0.75, 180);
   fence(6,-7,-11, 0.75, 180);
   fence(3,-7,-11, 0.75, 180);
   fence(0,-7,-11, 0.75, 180);
   fence(-3,-7,-11, 0.75, 180);
   fence(-6,-7,-11, 0.75, 180);
   fence(-9,-7,-11, 0.75, 180);
   fence(-12,-7,-11, 0.75, 180);
   //left
   fence(-13,-7,-10, 0.75, -90);
   fence(-13,-7,-7, 0.75, -90);
   fence(-13,-7,-4, 0.75, -90);
   fence(-13,-7,-1, 0.75, -90);
   fence(-13,-7,2, 0.75, -90);
   fence(-13,-7,5, 0.75, -90);
   fence(-13,-7,8, 0.75, -90);
   fence(-13,-7,11, 0.75, -90);
   fence(-13,-7,14, 0.75, -90);
}

static void flower(double x, double y, double z,
                   double dx, double dy, double dz,
                   double theta,
                   double r, double g, double b)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(theta, 0,1,0);
   glScaled(dx, dy, dz);
   //stem
   cylinder(0,0,0, 0.1,1, 0,0,0, 0,0.3,0, 360, 0,90,1);
   //center bit thingy
   sphere(0,3,0, 0.5, 0.54,0.27,0.07, 6);
   //petals
   int i;
   int j;
   glColor3f(r,g,b);
   int triangleAmount = 72; //# of triangles used to draw circle

   //radius
   GLfloat twicePi = 2.0f * 3.1415826;
   for(j = 0; j < 8; j++) {
      glPushMatrix();
      glTranslated(cos(j*twicePi/8),3+0.75*sin(j*twicePi/8),-0.75*sin(j*twicePi/8));
      glRotated(45, 1,0,0);
      glRotated(j*360/8, 0,1,0);
      glScaled(0.75,1,0.25);
      glBegin(GL_TRIANGLE_FAN);
      glNormal3f( 0, -1, 0);
      glVertex3f(0, 0, 0); // center of circle
      for(i = 0; i <= triangleAmount;i++) {
         glVertex3f(
            (cos(i *  twicePi / triangleAmount)), 
            0,
            (sin(i *  twicePi / triangleAmount))
            );
      }
      glEnd();
      glPopMatrix();
   }
   glPopMatrix();
}

static void flowers()
{
   flower(5,-10,13, 1,1,1, 0, 1,1,1);
   flower(7,-10,12, 1,1,1, 0, 0.1,0.1,1);
   flower(9,-10,13, 1,1,1, 0, 1,1,0.1);
   flower(11,-10,12, 1,1,1, 0, 0.6,0,0.3);

   flower(-5,-10,13, 1,1,1, 0, 1,0.1,0.1);
   flower(-7,-10,12, 1,1,1, 0, 0.1,1,1);
   flower(-9,-10,13, 1,1,1, 0, 1,0.1,1);
   flower(-11,-10,12, 1,1,1, 0, 1,0.8,1);
}

static void snowParticles()
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[17]);
   //Creat new particles, but no more than 500 at any given time.
   if (count < 500)
   {
      //x position
      particles[count][0] = -50.0 + 100*((double)rand()/(double)RAND_MAX);
      //y position
      particles[count][1] = 50;
      //z position
      particles[count][2] = -50.0 + 100*((double)rand()/(double)RAND_MAX);
      //size
      particles[count][3] = 0.3*((double)rand()/(double)RAND_MAX);
      count += 1;
   }
   //draw circle for each particle
   for (int j = 0; j < count; j++)
   {
      if (particles[j][1] < -10.0 || (particles[j][1] < 15 && particles[j][0] < 10 && particles[j][0] > -10 && particles[j][2] < 10 && particles[j][2] > -10))
      {
         for (int k = j; k < count; k++)
         {
            particles[k][0] = particles[k+1][0];
            particles[k][1] = particles[k+1][1];
            particles[k][2] = particles[k+1][2];
            particles[k][3] = particles[k+1][3];
         }
         count--;
         j--;
      }
      else
      {
         particles[j][0] += -0.1 + 0.2*((double)rand()/(double)RAND_MAX);
         particles[j][1] += -0.1;
         particles[j][2] += -0.1 + 0.2*((double)rand()/(double)RAND_MAX);
         int i;
         int triangleAmount = 72; //# of triangles used to draw circle

         //radius
         GLfloat twicePi = 2.0f * 3.1415826;
         glPushMatrix();
         glTranslated(particles[j][0],particles[j][1],particles[j][2]);
         glRotated(90, 1,0,0);
         glRotated(th, 0,0,1);
         glScaled(particles[j][3],particles[j][3],particles[j][3]);
         glBegin(GL_TRIANGLE_FAN);
         glColor3f(1,1,1);
         glTexCoord2d(0.5,0.5);
         glVertex3f(0, 0, 0); // center of circle
         for(i = 0; i <= triangleAmount;i++) {
            glTexCoord2d(0.5 + 0.5*cos(i *  twicePi / triangleAmount),0.5 + 0.5*sin(i *  twicePi / triangleAmount));
            glVertex3f(
               (cos(i *  twicePi / triangleAmount)), 
               0,
               (sin(i *  twicePi / triangleAmount))
               );
         }
         glEnd();
         glPopMatrix();
      }
   }
   glDisable(GL_TEXTURE_2D);
}

static void outside()
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   if (season == 3)
   {
      glBindTexture(GL_TEXTURE_2D,texture[15]);
   }
   else
      glBindTexture(GL_TEXTURE_2D,texture[5]);
   //  square
   glBegin(GL_QUADS);
   glColor3f(1,1,1);
   glNormal3f( 0, +1, 0);
   glTexCoord2f(0,0); glVertex3f(-50,-10.001,-50);
   glTexCoord2f(10,0); glVertex3f(+50,-10.001,-50);
   glTexCoord2f(10,10); glVertex3f(+50,-10.001,+50);
   glTexCoord2f(0,10); glVertex3f(-50,-10.001,+50);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   if (season == 0)
      flowers();
}

/* 
 *  Draw sky box
 */
static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,texture[9+season]);
   glBegin(GL_QUADS);
   glColor3f(0.9,0.9,1);
   glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,texture[10+season]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.25,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(-D,+D,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[11+season]);
   glBegin(GL_QUADS);

   glTexCoord2f(0.25,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

   Sky(3.5*dim);

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   //  Light position.
   float PositionSun[] = {10+distance*Cos(zh),10+distance*Sin(zh),10+distance*Sin(-zh),1.0};
   float PositionLamp[] = {-8,-1,-8,1.0};
   //float DirectionDown[] = {Cos(Th)*Sin(Ph),Sin(Th)*Sin(Ph),-Cos(Ph),0};
   //float DirectionUp[] = 
   //  Draw light position (still no lighting here)
   glColor3f(1,1,1);
   glPushMatrix();
   glTranslated(PositionSun[0],PositionSun[1],PositionSun[2]);
   sphere(0,0,0, 2, 0.6,0.5,0.5, 0);
   glPopMatrix();
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Location of viewer for specular calculations
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
   //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   //  Enable light 0 (sun)
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,PositionSun);
   //  Light switch
   if (light)
   {
      sphere(PositionLamp[0],PositionLamp[1],PositionLamp[2], 0.7, 1.0,1.0,0.0, 0);
      //  Enalble light 1 (lamp)
      glEnable(GL_LIGHT1);
      glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT1,GL_POSITION,PositionLamp);
   }
   else
   {
     sphere(PositionLamp[0],PositionLamp[1],PositionLamp[2], 0.7, 0.1,0.1,0.1, 0);
     glDisable(GL_LIGHT1);
   }

   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   outside();
   glDisable(GL_POLYGON_OFFSET_FILL);
   mailBox(-5,-7,20, 0.3, 0);
   pickitFence();
   glFrontFace(GL_CW);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);
   glDisable(GL_CULL_FACE);
   armchair(+7,-10,-3, 0.3, 0);
   lamp(-8,-10,-8, 0.6);
   tv(+7,-10,+6, 0.3, 0);
   table(-5,-10,0, 0.5,0);
   tableChair(-1.5,-10,-5, 0.24,30);
   tableChair(-6,-10,+5, 0.24,180);
   if (mode)
   {
     house(1);
   }
   else
   {
     glFrontFace(GL_CW);
     glCullFace(GL_BACK);
     glEnable(GL_CULL_FACE);
     house(-1);
     glDisable(GL_CULL_FACE);
   }
   glDisable(GL_LIGHTING);
   if (season == 3)
      snowParticles();

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   seconds = (int) t;
   seconds = seconds % 4;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==2) ? 30 : 2;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Change field of view angle
   else if (ch == '+' && ch>1)
      fov--;
   else if (ch == '-' && ch<179)
      fov++;
   //  Switch between winter and summer
   else if (ch == 's' || ch == 'S')
      season = 3-season;
   //  Turn on and off TV
   else if (ch == 't' || ch == 'T')
      watching = 1-watching;
   //  Change channel
   else if (ch == 'v' || ch == 'V')
   {
      if (channel < 5)
         channel += 1;
      else
         channel = 0;
   }
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(750,700);
   glutCreateWindow("Michael Crozier Final Project");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[1] = LoadTexBMP("wallPlanks.bmp");
   texture[2] = LoadTexBMP("roof.bmp");
   texture[3] = LoadTexBMP("rug.bmp");
   texture[4] = LoadTexBMP("kkslider.bmp");
   texture[5] = LoadTexBMP("grass.bmp");
   texture[6] = LoadTexBMP("metal.bmp");
   texture[7] = LoadTexBMP("lamp_shade_outer.bmp");
   texture[8] = LoadTexBMP("brown_leather.bmp");
   texture[9] = LoadTexBMP("forest_sides.bmp");
   texture[10] = LoadTexBMP("forest_top.bmp");
   texture[11] = LoadTexBMP("forest_bottom.bmp");
   texture[12] = LoadTexBMP("winter_sides.bmp");
   texture[13] = LoadTexBMP("winter_top.bmp");
   texture[14] = LoadTexBMP("winter_bottom.bmp");
   texture[15] = LoadTexBMP("snow.bmp");
   texture[16] = LoadTexBMP("tv.bmp");
   texture[17] = LoadTexBMP("Snowflake.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
