/*
 *  Michael Crozier HW5, spooky scary mood lighting
 *
 *  Demonstrates basic lighting on a set of skeletons, can also make the light bob up and down
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  b          Toggle light bobbing
 *  h/H        Decrease/Increase bobbing magnitude
 *  j/J        Decrease/Increase bobbing frequency
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (2/30)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=50.0;  //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =  30;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
int bobbing   =   1;  // Whether the light is bobbing spookily
int magnitude =   8;  // Light bobbing magnitude
int frequency =   4;  // Light bobbing frequency
float ylight  =   0;  // Elevation of light

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 double r, double g, double b)
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
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(r,g,b);
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   // teeth
   //  Back
   glNormal3f( 0, 0, -1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f( +1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   // teeth
   glColor3f(0,0,0);
   glVertex3f(1.1,-1,0.8);
   glVertex3f(1.1,-1,0.4);
   glVertex3f(1.1,0.5,0.4);
   glVertex3f(1.1,0.5,0.8);

   glVertex3f(1.1,-1,0.2);
   glVertex3f(1.1,-1,-0.2);
   glVertex3f(1.1,0.5,-0.2);
   glVertex3f(1.1,0.5,0.2);

   glVertex3f(1.1,-1,-0.4);
   glVertex3f(1.1,-1,-0.8);
   glVertex3f(1.1,0.5,-0.8);
   glVertex3f(1.1,0.5,-0.4);
   glEnd();
   // nose
   glBegin(GL_TRIANGLES);
   glVertex3f(1,1,0.5);
   glVertex3f(1,1,-0.5);
   glVertex3f(1,2,0);
   glEnd();
   //  Left
   glBegin(GL_QUADS);
   glColor3f(r,g,b);
   glNormal3f( -1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f( 0, +1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0, -one, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();

   //eyes
   glColor3f(0,0,0);
   int i;
   int triangleAmount = 72; //# of triangles used to draw circle

   //GLfloat radius = 0.8f; //radius
   GLfloat twicePi = 2.0f * 3.1415826;

   glBegin(GL_TRIANGLE_FAN);
      glVertex3f(1, 2, 0.6); // center of circle
      for(i = 0; i <= triangleAmount;i++) { 
      glVertex3f(
         1, 
         2 + 0.5*(sin(i *  twicePi / triangleAmount)),
         0.6 + 0.5*(cos(i *  twicePi / triangleAmount))
	 );
      }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
      glVertex3f(1, 2, -0.6); // center of circle
      for(i = 0; i <= triangleAmount;i++) { 
      glVertex3f(
         1, 
         2 + 0.5*(sin(i *  twicePi / triangleAmount)),
         -0.6 + 0.5*(cos(i *  twicePi / triangleAmount))
	 );
      }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

static void Vertex(double th,double ph, double r, double g, double b)
{
   glColor3f(r,g,b);
   glNormal3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
//Draw vertex in shifted polar coordinates
static void VertexBone(double th,double ph, double y, double r, double g, double b)
{
   glColor3f(r,g,b);
   glNormal3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph)+y , Cos(th)*Cos(ph));
}

static void sphere(double x,double y,double z,double r, double red, double g, double b)
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
}

//more like a cylinder than a bone, but it's a water tight open cylinder with circle caps
static void bone(double x, double y, double z,
		double radius, double length,
                double alpha, double beta, double gamma,
                double r, double g, double b)
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

   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d) {
      VertexBone(th,0,0, r,g,b);
      VertexBone(th,0,3, r,g,b);
   }
   glEnd();

   int i;
   glColor3f(r,g,b);
   int triangleAmount = 72; //# of triangles used to draw circle

   //GLfloat radius = 0.8f; //radius
   GLfloat twicePi = 2.0f * 3.1415826;

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
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f( 0, +1, 0);
   glVertex3f(0, 3, 0); // center of circle
   for(i = 0; i <= triangleAmount;i++) { 
      glVertex3f(
         (cos(i *  twicePi / triangleAmount)), 
         3,
         (sin(i *  twicePi / triangleAmount))
         );
   }
   glEnd();

   glPopMatrix();
}

static void skull(double x, double y, double z,
                  double r,
                  double alpha, double beta, double gamma,
                  double red, double g, double b)
{
   sphere(x, y, z, r, red,g,b);
   cube(x+r/2,y-r/2,z, r/2,r/2,2*r/3, 0, red-0.1, g-0.1, b-0.1);
}

static void extremity(double x, double y, double z,
                      double dx, double dy, double dz,
                      double alpha, double beta, double gamma,
                      double r, double g, double b)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(alpha,1,0,0);
   glRotated(beta,0,1,0);
   glRotated(gamma,0,0,1);
   glScaled(dx,dy,dz);
   //foot/hand
   glBegin(GL_QUADS);
   glColor3f(r,g,b);
   //top
   glNormal3f( 0, +1, 0);
   glVertex3f(-1,+0.5,+0.5);
   glVertex3f(+1,+0.5,+1);
   glVertex3f(+1,+0.5,-1);
   glVertex3f(-1,+0.5,-0.5);
   //bottom
   glNormal3f( 0, -1, 0);
   glVertex3f(-1,-0.5,-0.5);
   glVertex3f(+1,-0.5,-1);
   glVertex3f(+1,-0.5,+1);
   glVertex3f(-1,-0.5,+0.5);
   //back
   glNormal3f( -1, 0, 0);
   glVertex3f(-1,-0.5,-0.5);
   glVertex3f(-1,-0.5,+0.5);
   glVertex3f(-1,+0.5,+0.5);
   glVertex3f(-1,+0.5,-0.5);
   //left
   glNormal3f( 0, 0, +1);
   glVertex3f(-1,-0.5, 0.5);
   glVertex3f(+1,-0.5, 1);
   glVertex3f(+1,+0.5, 1);
   glVertex3f(-1,+0.5, 0.5);
   //right
   glNormal3f( 0, 0, -1);
   glVertex3f(+1,-0.5,-1);
   glVertex3f(-1,-0.5,-0.5);
   glVertex3f(-1,+0.5,-0.5);
   glVertex3f(+1,+0.5,-1);
   //front
   glNormal3f( +1, 0, 0);
   glVertex3f(+1,-0.5,+1);
   glVertex3f(+1,-0.5,-1);
   glVertex3f(+1,+0.5,-1);
   glVertex3f(+1,+0.5,+1);
   glEnd();
   glPopMatrix();
}

static void ribs(double x, double y, double z,
                 double dx, double dy, double dz,
                 double alpha, double beta, double gamma,
                 double r, double g, double b)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(alpha,1,0,0);
   glRotated(beta,0,1,0);
   glRotated(gamma,0,0,1);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(r,g,b);
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f( 0, 0, -1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f( 1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   // ribs
   // front
   glColor3f(0,0,0);
   glVertex3f(1.1,-0.6,1.1);
   glVertex3f(1.1,-0.6,0.2);
   glVertex3f(1.1,-0.4,0.2);
   glVertex3f(1.1,-0.4,1.1);

   glVertex3f(1.1,-0.6,-1.1);
   glVertex3f(1.1,-0.6,-0.2);
   glVertex3f(1.1,-0.4,-0.2);
   glVertex3f(1.1,-0.4,-1.1);

   glVertex3f(1.1,0,1.1);
   glVertex3f(1.1,0,0.2);
   glVertex3f(1.1,0.2,0.2);
   glVertex3f(1.1,0.2,1.1);

   glVertex3f(1.1,0,-1.1);
   glVertex3f(1.1,0,-0.2);
   glVertex3f(1.1,0.2,-0.2);
   glVertex3f(1.1,0.2,-1.1);

   glVertex3f(1.1,0.6,1.1);
   glVertex3f(1.1,0.6,0.2);
   glVertex3f(1.1,0.8,0.2);
   glVertex3f(1.1,0.8,1.1);

   glVertex3f(1.1,0.6,-1.1);
   glVertex3f(1.1,0.6,-0.2);
   glVertex3f(1.1,0.8,-0.2);
   glVertex3f(1.1,0.8,-1.1);
   //right
   glColor3f(0,0,0);
   glVertex3f(1.1,-0.6,1.1);
   glVertex3f(0.2,-0.6,1.1);
   glVertex3f(0.2,-0.4,1.1);
   glVertex3f(1.1,-0.4,1.1);

   glVertex3f(1.1,-0.6,1.1);
   glVertex3f(0.2,-0.6,1.1);
   glVertex3f(0.2,-0.4,1.1);
   glVertex3f(1.1,-0.4,1.1);

   glVertex3f(1.1,0,1.1);
   glVertex3f(0.2,0,1.1);
   glVertex3f(0.2,0.2,1.1);
   glVertex3f(1.1,0.2,1.1);

   glVertex3f(1.1,0,1.1);
   glVertex3f(0.2,0,1.1);
   glVertex3f(0.2,0.2,1.1);
   glVertex3f(1.1,0.2,1.1);

   glVertex3f(1.1,0.6,1.1);
   glVertex3f(0.2,0.6,1.1);
   glVertex3f(0.2,0.8,1.1);
   glVertex3f(1.1,0.8,1.1);

   glVertex3f(1.1,0.6,1.1);
   glVertex3f(0.2,0.6,1.1);
   glVertex3f(0.2,0.8,1.1);
   glVertex3f(1.1,0.8,1.1);
   //left
   glColor3f(0,0,0);
   glVertex3f(1.1,-0.6,-1.1);
   glVertex3f(0.2,-0.6,-1.1);
   glVertex3f(0.2,-0.4,-1.1);
   glVertex3f(1.1,-0.4,-1.1);

   glVertex3f(1.1,-0.6,-1.1);
   glVertex3f(0.2,-0.6,-1.1);
   glVertex3f(0.2,-0.4,-1.1);
   glVertex3f(1.1,-0.4,-1.1);

   glVertex3f(1.1,0,-1.1);
   glVertex3f(0.2,0,-1.1);
   glVertex3f(0.2,0.2,-1.1);
   glVertex3f(1.1,0.2,-1.1);

   glVertex3f(1.1,0,-1.1);
   glVertex3f(0.2,0,-1.1);
   glVertex3f(0.2,0.2,-1.1);
   glVertex3f(1.1,0.2,-1.1);

   glVertex3f(1.1,0.6,-1.1);
   glVertex3f(0.2,0.6,-1.1);
   glVertex3f(0.2,0.8,-1.1);
   glVertex3f(1.1,0.8,-1.1);

   glVertex3f(1.1,0.6,-1.1);
   glVertex3f(0.2,0.6,-1.1);
   glVertex3f(0.2,0.8,-1.1);
   glVertex3f(1.1,0.8,-1.1);
   glEnd();
   //  Left
   glBegin(GL_QUADS);
   glColor3f(r,g,b);
   glNormal3f( -1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f( 0, 1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   glPopMatrix();
}

static void body(double x, double y, double z,
                 double dx, double dy, double dz,
                 double headRadius,
                 double alpha, double beta, double gamma,
                 double r, double g, double b)
{
   glPushMatrix();
   glRotated(alpha,1,0,0);
   glRotated(beta,0,1,0);
   glRotated(gamma,0,0,1);
   //body
   ribs(x+1*dx,y+6*dy,z, 2*dx,2*dy,2*dz, 0,0,0, r,g,b);
   bone(x,y+2*dy,z, 0.5*dz,dy, 0,0,0, r,g,b);
   //legs
   bone(x,y-1*dy,z+2*dz, 0.5*dz,dy, -20,0,0, r,g,b);
   bone(x,y-1*dy,z-2*dz, 0.5*dz,dy, +20,0,0, r,g,b);
   bone(x,y-5*dy,z+2*dz, 0.5*dz,dy, 0,0,0, r,g,b);
   bone(x, y-5*dy,z-2*dz, 0.5*dz,dy, 0,0,0, r,g,b);
   extremity(x,y-7*dy,z+2*dz, dx,dy,dz, 0,0,-20, r,g,b);
   extremity(x,y-7*dy,z-2*dz, dx,dy,dz, 0,0,-20, r,g,b);
   //arms
   bone(x,y+7*dy,z+3.5*dz, 0.5*dz,dy, -30,0,0, r,g,b);
   bone(x,y+7*dy,z-3.5*dz, 0.5*dz,dy, +30,0,0, r,g,b);
   bone(x,y+3*dy,z+3.5*dz, 0.5*dz,dy, 0,0,0, r,g,b);
   bone(x, y+3*dy,z-3.5*dz, 0.5*dz,dy, 0,0,0, r,g,b);
   extremity(x,y+1*dy,z+3.5*dz, dx,dy,dz, 0,+90,-90, r,g,b);
   extremity(x,y+1*dy,z-3.5*dz, dx,dy,dz, 0,+90,-90, r,g,b);
   //head
   skull(x,y+11*dy,z, headRadius*2, 0,0,0, r,g,b);
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=15;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	//  Light position. If it is bobbing up and down, add a Sin bit to the y value
	float Position[] = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
	if (bobbing) {
        	Position[1] = ylight+magnitude*Sin(frequency*zh);
	}
        //  Draw light position as SPOOKY SKULL (still no lighting here)
        glColor3f(1,1,1);
	glPushMatrix();
	glTranslated(Position[0],Position[1],Position[2]);
	glRotated(-zh-180,0,1,0);
	skull(0,0,0, 2, 0,0,0, 0.9,0.9,0.9);
	glPopMatrix();
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   //  Draw scene
   body(0,0,0, 1,1,1, 1, 0,0,0, 0.9,0.9,0.9);
   body(-10,-10,5, 0.5,0.5,0.5, 0.5, 90,0,0, 1,0,0);
   body(10,10,-5, 1,1,2, 0.5, -45, 30, 120, 0,1,0);
   body(0,10,10, 0.5,2,0.5, 1, 180,0,0, 0,0,1);
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   if (bobbing) {
      glWindowPos2i(5,65);
      Print("Magnitude=%d Frequency=%d",magnitude,frequency);
   }

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
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
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
   Project(mode?fov:0,asp,dim);
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
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   else if (ch=='b')
   //Make the light (and skull) bob up and down as it goes along it's path
      bobbing = 1-bobbing;
   //change magnitude of bobbing
   else if (ch == 'H' && magnitude < 40)
      magnitude += 1;
   else if (ch == 'h' && magnitude > 0)
      magnitude -= 1;
   //change frequency of bobbing
   else if (ch == 'J' && frequency < 359)
      frequency += 1;
   else if (ch == 'j' && frequency > 0)
      frequency -= 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
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
   Project(mode?fov:0,asp,dim);
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
   glutCreateWindow("Michael Crozier HW5: Spooky Scary Mood Lighting!");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
