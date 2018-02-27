/*
 *  Mr. Bone's Bones (cylinders)
 *
 *  A bunch of cylinders that are the bones of Mr. Bones that I didn't finish in time
 *
 *  Key bindings:
 *  arrows     Change view angle
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 double r, double g, double b)
{
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
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   // teeth
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
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
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
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

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph, double r, double g, double b)
{
   glColor3f(r,g,b);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
//Draw vertex in shifted polar coordinates
static void VertexBone(double th,double ph, double y, double r, double g, double b)
{
   glColor3f(r,g,b);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph)+y , Cos(th)*Cos(ph));
}

/*
 *  Draw a sphere (version 2)
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere2(double x,double y,double z,double r, double red, double g, double b)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

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

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(alpha, 1,0,0);
   glRotated(beta, 0,1,0);
   glRotated(gamma, 0,0,1);
   glScaled(radius, length, radius);
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
//didn't get to finish mr. Bones in time
static void skull(double x, double y, double z,
                  double r,
                  double alpha, double beta, double gamma,
                  double red, double g, double b)
{
   sphere2(x, y, z, r, red,g,b);
   cube(x+r/2,y-r/2,z, r/2,r/2,2*r/3, 0, red-0.1, g-0.1, b-0.1);
}

static void extremity(double x, double y, double z,
                      double dx, double dy, double dz,
                      double alpha, double beta, double gamma,
                      double r, double g, double b)
{
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
   glVertex3f(-1,+0.5,+0.5);
   glVertex3f(+1,+0.5,+1);
   glVertex3f(+1,+0.5,-1);
   glVertex3f(-1,+0.5,-0.5);
   //bottom
   glVertex3f(-1,-0.5,-0.5);
   glVertex3f(+1,-0.5,-1);
   glVertex3f(+1,-0.5,+1);
   glVertex3f(-1,-0.5,+0.5);
   //back
   glVertex3f(-1,-0.5,-0.5);
   glVertex3f(-1,-0.5,+0.5);
   glVertex3f(-1,+0.5,+0.5);
   glVertex3f(-1,+0.5,-0.5);
   //left
   glVertex3f(-1,-0.5, 0.5);
   glVertex3f(+1,-0.5, 1);
   glVertex3f(+1,+0.5, 1);
   glVertex3f(-1,+0.5, 0.5);
   //right
   glVertex3f(+1,-0.5,-1);
   glVertex3f(-1,-0.5,-0.5);
   glVertex3f(-1,+0.5,-0.5);
   glVertex3f(+1,+0.5,-1);
   //front
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
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   // teeth
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
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
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
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
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);
   //THE SKELITONS!
   body(0,0,0, 1,1,1, 1, 0,0,0, 0.9,0.9,0.9);
   body(-10,-10,5, 0.5,0.5,0.5, 0.5, 90,0,0, 1,0,0);
   body(10,10,-5, 1,1,2, 0.5, -45, 30, 120, 0,1,0);
   body(0,10,10, 0.5,2,0.5, 1, 180,0,0, 0,0,1);
   
   //  White
   glColor3f(1,1,1);
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
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Print("Angle=%d,%d SPOOKY SCARY SKELETONS RAINING EVERYWHERE!",th,ph);
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
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
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
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
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   const double dim=40;
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(700,700);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Michael Crozier HW3");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
