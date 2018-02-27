//hunter haskins
//final project

/*
 *  Lighting
 *
 *  Demonstrates basic lighting using a sphere and a cube.
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
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
double dim=5.0;   //  Size of world
int m = 1;        //  flag for displaying monster
double up = 0;		//  displacement of the boat
int ro = 0;			//  rotation angle of the boat
int f = 0;        //  flag for fog
int cannonBall= 0;//  flag for a fired cannonball
double cpos;      //  cannon ball position

// Light values
unsigned int texture[15];
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
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
float ylight  =   1;  // Elevation of light

//prototyping functions
void rail(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s);

void cannon(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s);

void wheel(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s);

static void ladder(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th);

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int t);

void rope(double x, double y, double z, double xangle,
		double rx, double yangle, double ry, double zangle, 
		double rz, double s, int t);

void cylinder(double x, double y, double z, double angle,
		double rx, double ry, double rz, 
		double s, double s1, double s2, int t);

void sail(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s);

void sphere(double x, double y, double z, double angle,
		double rx, double ry, double rz, 
		double s, double s1, double s2, int t);

//vertex and texture coordinates for circle
static void Vertex2(double th, double ph)
{
  glNormal3d(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
  glTexCoord2d(2*th/360.0,2*ph/180.0);
  glVertex3d(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
}

static void Vertex3(double th, double ph)
{
  glNormal3d(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
  glTexCoord2d(1.3*(th+70)/360.0,1.3*(ph+90)/180.0);
  glVertex3d(1.02*Sin(th)*Cos(ph), 1.02*Sin(ph), 1.02*Cos(th)*Cos(ph));
}


//places a boat in the scene
static void boat(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s)
{
   int th;
	int ph;

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s*2,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   
   glBindTexture(GL_TEXTURE_2D,texture[0]);
//drawing half circle and changing color
//I set the normal for the bottom half of the ship to be the same as the sphere
	glTexCoord2d(0,0);
   for (ph=180;ph<360;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      glColor3f(0.867,0.719,0.527);
      for (th=0;th<=360;th+=5)
      {
         Vertex2(th,ph);
         Vertex2(th,ph+5);
      }
      glEnd();
   }

   glPopMatrix();     
   glDisable(GL_TEXTURE_2D);

//back of boat
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
	glRotated(90,1,0,0);
   glScaled(s*2,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   
   glBindTexture(GL_TEXTURE_2D,texture[0]);
	
	glBegin(GL_QUAD_STRIP);
	for(int i = -62; i <= 60; i += 2)
	{
		glNormal3f(Cos(i),Sin(i),0);
		glTexCoord2f(0.01*i+0.5,0);
		glVertex3f(Cos(i),Sin(i),0);
		glTexCoord2f(0.01*i+0.5,1);
		glVertex3f(Cos(i),Sin(i),-0.6);
	}
	glEnd();

//top of back deck
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);
	glNormal3f(0,0,-1);
	glVertex3f(0.5,0,-0.6);
	for(int j = -62; j <= 60; j += 2)
	{
      glTexCoord2f(0.5*Cos(j)+0.5,0.5*Sin(j)+0.5);
		glVertex3f(Cos(j),Sin(j),-0.6);
	}
	glEnd();

//wall between upper and lower deck
   glColor3f(0.867,0.719,0.527);
   glBindTexture(GL_TEXTURE_2D,texture[11]);
	glBegin(GL_QUADS);
   glNormal3f( 0,-1,0);
   glTexCoord2f(0,0);
   glVertex3f(0.5,-0.85,-0.6);
   glTexCoord2f(1,0);
   glVertex3f(0.5,-0.85,0);
   glTexCoord2f(1,1);
   glVertex3f(0.5,0.85,0);
   glTexCoord2f(0,1); 
   glVertex3f(0.5,0.85,-0.6);
   glEnd();
	

	glPopMatrix();	

//drawing the railings around the ship
	glPushMatrix();
	glTranslated(x,y,z);
	glRotated(angle,rx,ry,rz);
	glRotated(90,1,0,0);
	glScaled(s*2,s,s);

	for(int j = 0; j <= 360; j += 5)
	{
		if((j > 200 && j < 216) || (j < 340 && j > 324))
		{
			continue;
		} 
		else if(j > 30 && j <= 150)
		{
			rail(Sin(j),Cos(j),-0.7,-j,0,0,1,1);
		}
		else
		{
			rail(Sin(j),Cos(j),-0.1,-j,0,0,1,1);
		}
	}

   glPopMatrix();

   glPushMatrix(); 
//drawing 2d circle which closes the top of the semi circle
//I set the normal straight upward for the top of the ship
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s*2,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[11]);

   glBegin(GL_TRIANGLE_FAN);
   //glColor3f(0.543,0.270,0.074);


   glNormal3d(0,1,0);
	glTexCoord2f(0.5,0.5);
   glVertex3f(0,0,0);
   for(int i = 0;i <= 360;i+=5) { 
      glTexCoord2f(0.5*Cos(i)*2+0.5,0.5*Sin(i)*2+0.5);
      glVertex3f(Cos(i),0,Sin(i));
   }
   glEnd();
   glPopMatrix();

//drawing cannons on ship
   glPushMatrix(); 
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s*0.1,s*0.1,s*0.1);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	cannon(-10,1.5,-7,0,0,0,0,1);
	cannon(-10,1.5,7,180,0,1,0,1);

   glPopMatrix();

//drawing mast and steering wheel and various objects on the ship
   glPushMatrix(); 
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s,s);

//the mast on the ship
   glColor3f(0.543,0.270,0.074);
	cylinder(-1,1.2,0,90,1,0,0,0.08,0.08,2,3);
	cylinder(1.1,1.5,0,90,1,0,0,0.05,0.05,1.8,3);

//horizontal cylinders for the sails
	cylinder(-1,1,0,0,0,0,0,0.05,0.05,0.9,3);
	cylinder(1.1,1.5,0,0,0,0,0,0.04,0.04,0.9,3);	

	cylinder(-1,2.5,0,0,0,0,0,0.05,0.05,0.9,3);	
	cylinder(1.1,3,0,0,0,0,0,0.04,0.04,0.9,3);	

//steering wheel as a wall decoration
	wheel(1,0.3,0.5,90,0,1,0,0.15);
	
//putting the sails on
	sail(0.3,1.7,0,0,0,0,0,1.6);
	sail(2.42,2.3,0,0,0,0,0,1.6); 

//the pole and steering wheel
	cube(1.2,0.8,0,0.02,0.3,0.02,0,2);
	wheel(1.23,1,0,90,0,1,0,0.1);

//ladder that leads up to the second deck
	ladder(0.99,0.3,-0.5,0.2,0.22,0.2,90);

//ladders up the second mast
	ladder(-0.9,0.3,0,0.2,0.24,0.2,90);
	ladder(-0.9,1,0,0.2,0.24,0.2,90);
	ladder(-0.9,1.7,0,0.2,0.24,0.2,90);
	ladder(-0.9,2.4,0,0.2,0.24,0.2,90);
	//ladder(-0.9,3.1,0,0.2,0.24,0.2,90);


//putting cannon balls on the ship
	glColor3f(0.5,0.5,0.5);
	sphere(-0.6,0.1,-0.75,0.0,0.0,0.0,0,0.08,0.08,0.08,1);

	sphere(-0.55,0.1,0.75,0.0,0.0,0.0,0,0.08,0.08,0.08,1);
	sphere(-0.70,0.1,0.75,0.0,0.0,0.0,0,0.08,0.08,0.08,1);
	sphere(-0.6,0.1,0.60,0.0,0.0,0.0,0,0.08,0.08,0.08,1);
	sphere(-0.6,0.22,0.7,0.0,0.0,0.0,0,0.08,0.08,0.08,1);

//putting crates on the ship
	cube(0.2,0.15,0.8,0.15,0.15,0.15,0,9);
	cube(0.6,0.15,0.8,0.15,0.15,0.15,0,9);
	cube(0.2,0.15,-0.8,0.15,0.15,0.15,0,9);

//putting ropes on the ship
//right side vertical ropes
	rope(-0.45,1.45,0.65,75,1,0,0,13,1,1.38,10);
	rope(-0.60,1.45,0.65,76,1,0,0,10,1,1.35,10);
	rope(-0.75,1.45,0.65,77,1,0,0,6,1,1.33,10);

//left side vertical ropes
	rope(-0.45,1.45,-0.65,-75,1,0,0,13,1,1.38,10);
	rope(-0.60,1.45,-0.65,-76,1,0,0,10,1,1.35,10);
	rope(-0.75,1.45,-0.65,-77,1,0,0,6,1,1.33,10);

//right side horizontal ropes
	glPushMatrix();
	glRotated(-15,1,0,0);
	for(double x = 0.2; x < 2.6; x += 0.2)
	{
		rope(-0.45-x*0.14,x,1,0,0,90,1,0,1,0.27-x*0.08,10);
	}
	glPopMatrix();

//left side horizontal ropes
	glPushMatrix();
	glRotated(15,1,0,0);
	for(double x = 0.2; x < 2.6; x += 0.2)
	{
		rope(-0.45-x*0.14,x,-1,0,0,90,1,0,1,0.27-x*0.08,10);
	}
	glPopMatrix();

//lookout station
   glColor3f(0.543,0.270,0.074);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[11]);
	glNormal3f(0,-1,0);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);
	glVertex3f(-1,2.7,0);
	for (int k=0;k<=360;k+=10)
	{   
		glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
		glVertex3f(0.4*Cos(k)-1,2.7,0.4*Sin(k));
	}   
	glEnd();

	glNormal3f(0,1,0);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);
	glVertex3f(-1,2.71,0);
	for (int k=0;k<=360;k+=10)
	{   
		glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
		glVertex3f(0.4*Cos(k)-1,2.71,0.4*Sin(k));
	}   
	glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUAD_STRIP);
   for (int k=0;k<=360;k+=10)
   {   
      glNormal3f(0.4*Cos(k-90)-1,3,0.4*Sin(k-90));
      glTexCoord2f(0,0.002*k); glVertex3f(0.4*Cos(k-90)-1,2.7,0.4*Sin(k-90));
      glTexCoord2f(1,0.002*k); glVertex3f(0.4*Cos(k-90)-1,3,0.4*Sin(k-90));
   }   
   glEnd();
	
	//drawing fired cannonball
	if(cannonBall == 1)
	{
		cpos += 0.5;
   	sphere(-1,0.1+cpos/4,-0.3-cpos,0,0,1,0,0.2,0.2,0.2,1);
   	sphere(-1,0.1+cpos/4,0.3+cpos,0,0,1,0,0.2,0.2,0.2,1);
	}	
	
	if(cpos > 30)
	{
		cannonBall = 0;
		cpos = 0;
	}

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void water()
{
//adding water
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glColor4f(1,1,1,0.8);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
	glBegin(GL_QUADS);
	
   glNormal3f( 0.0, 0.0, 1.0);
   glTexCoord2f(0,0); glVertex3f(-30,-0.6,30);
   glTexCoord2f(1,0); glVertex3f(30,-0.6,30);
   glTexCoord2f(1,1); glVertex3f(30.0,-0.6,-30);
   glTexCoord2f(0,1); glVertex3f(-30.0,-0.6,-30);
   glEnd();

	glDisable(GL_BLEND);

   glDisable(GL_TEXTURE_2D);

}

//draws a sail and puts the pirate flag texture on it
void sail(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s)
{

	glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[8]);
   glColor3f(1,1,1);

//drawing a small part of the edge of a cylinder for a slightly curved surface
   glBegin(GL_QUAD_STRIP);
   for (int k=150;k<=210;k+=10)
   {   
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.019*k); glVertex3f(Cos(k),Sin(k),0.5);
      glTexCoord2f(1,0.019*k); glVertex3f(Cos(k),Sin(k),-0.5);
   }   
   glEnd();
	

	glPopMatrix();

   glDisable(GL_TEXTURE_2D);
}

//draws a short ladder, sometimes stacked for a taller one
static void ladder(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   glPushMatrix(); 
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	cube(-0.5,0,0,0.05,1.5,0.05,0,3);
	cube(0.5,0,0,0.05,1.5,0.05,0,3);

	for(float i = -1.2; i <= 1.2;i += 0.6)
	{
		cube(0,i,0,0.5,0.05,0.05,0,3);
	}


	glPopMatrix();

   glDisable(GL_TEXTURE_2D);


}

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int t)
{
   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[t]);

   //  Front
   glBegin(GL_QUADS);
   glNormal3f( 0.0, 0.0, 1.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,-1.0, 1.0);
   glTexCoord2f(1,0); glVertex3f(1.0,-1.0, 1.0);
   glTexCoord2f(1,1); glVertex3f(1.0,1.0, 1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,1.0, 1.0);
   glEnd();

   //  Back
   glBegin(GL_QUADS);
   glNormal3f( 0.0, 0.0,-1.0);
   glTexCoord2f(0,0); glVertex3f(1.0,-1.0,-1.0);
   glTexCoord2f(1,0); glVertex3f(-1.0,-1.0,-1.0);
   glTexCoord2f(1,1); glVertex3f(-1.0,1.0,-1.0);
   glTexCoord2f(0,1); glVertex3f(1.0,1.0,-1.0);
   glEnd();

   //  Right
   glBegin(GL_QUADS);
   glNormal3f(1.0, 0.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(1.0,-1.0,1.0);
   glTexCoord2f(1,0); glVertex3f(1.0,-1.0,-1.0);
   glTexCoord2f(1,1); glVertex3f(1.0,1.0,-1.0);
   glTexCoord2f(0,1); glVertex3f(1.0,1.0,1.0);
   glEnd();

   //  Left
   glBegin(GL_QUADS);
   glNormal3f(-1.0, 0.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,-1.0,-1.0);
   glTexCoord2f(1,0); glVertex3f(-1.0,-1.0,1.0);
   glTexCoord2f(1,1); glVertex3f(-1.0,1.0,1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,1.0,-1.0);
   glEnd();

   //  Top
   glBegin(GL_QUADS);
   glNormal3f( 0.0,1.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,1.0,1.0);
   glTexCoord2f(1,0); glVertex3f(1.0,1.0,1.0);
   glTexCoord2f(1,1); glVertex3f(1.0,1.0,-1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,1.0,-1.0);
   glEnd();

   //  Bottom
   glBegin(GL_QUADS);
   glNormal3f( 0.0,-1.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,-1.0,-1.0);
   glTexCoord2f(1,0); glVertex3f(1.0,-1.0,-1.0);
   glTexCoord2f(1,1); glVertex3f(1.0,-1.0,1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,-1.0,1.0);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

//many of these interconnected makes up the railing around the ship
void rail(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s)
{
	

	glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(0.025*s,0.025*s,0.025*s*3);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(0.543,0.270,0.074);
   
   glBindTexture(GL_TEXTURE_2D,texture[2]);
	
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= 360; i++)
	{
		glNormal3f(Cos(i),Sin(i),0);
		glTexCoord2f(0,0.5*i);
		glVertex3f(Cos(i),Sin(i),1.1);
		glTexCoord2f(1,0.5*i);
		glVertex3f(Cos(i),Sin(i),-1.1);

	}	
	glEnd();
	
   glColor3f(0.543,0.270,0.074);
	cube(0,0,1.2,2,0.6,0.1,0,3);
	cube(0,0,-1.2,2,0.6,0.1,0,3);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


}

void rope(double x, double y, double z, double xangle,
		double rx, double yangle, double ry, double zangle, 
		double rz, double s, int t)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(zangle,0,0,rz);
   glRotated(yangle,0,ry,0);
   glRotated(xangle,rx,0,0);
   glScaled(0.015,0.015,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   
   glBindTexture(GL_TEXTURE_2D,texture[t]);
	//the ends of the cylinder

	glNormal3f(0,0,-1);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);
	glVertex3f(0,0,-1);
	for (int k=0;k<=360;k+=10)
	{   
		glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
		glVertex3f(-1*Cos(k),Sin(k),-1);
	}   
	glEnd();

	glNormal3f(0,0,1);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);
	glVertex3f(0,0,1);
	for (int k=0;k<=360;k+=10)
	{   
		glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
		glVertex3f(Cos(k),Sin(k),1);
	}   
	glEnd();

   //side
   glBindTexture(GL_TEXTURE_2D,texture[t]);
   glBegin(GL_QUAD_STRIP);
   for (int k=0;k<=360;k+=10)
   {   
      glNormal3f(Cos(k-90),Sin(k-90),0);
      glTexCoord2f(0,0.002*k); glVertex3f(Cos(k-90),Sin(k-90),+1);
      glTexCoord2f(1,0.002*k); glVertex3f(Cos(k-90),Sin(k-90),-1);
   }   
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

	

}


//draws a cylinder
void cylinder(double x, double y, double z, double angle,
		double rx, double ry, double rz, 
		double s, double s1, double s2, int t)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s1,s2);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   
   glBindTexture(GL_TEXTURE_2D,texture[t]);
	//the ends of the cylinder

	glNormal3f(0,0,-1);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);
	glVertex3f(0,0,-1);
	for (int k=0;k<=360;k+=10)
	{   
		glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
		glVertex3f(-1*Cos(k),Sin(k),-1);
	}   
	glEnd();

	glNormal3f(0,0,1);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5,0.5);
	glVertex3f(0,0,1);
	for (int k=0;k<=360;k+=10)
	{   
		glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
		glVertex3f(Cos(k),Sin(k),1);
	}   
	glEnd();

   //side
   glBindTexture(GL_TEXTURE_2D,texture[t]);
   glBegin(GL_QUAD_STRIP);
   for (int k=0;k<=360;k+=10)
   {   
      glNormal3f(Cos(k-90),Sin(k-90),0);
      glTexCoord2f(0,0.002*k); glVertex3f(Cos(k-90),Sin(k-90),+1);
      glTexCoord2f(1,0.002*k); glVertex3f(Cos(k-90),Sin(k-90),-1);
   }   
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

	

}


void wheel(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s)
{
	glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(0.543,0.270,0.074);
   
   glBindTexture(GL_TEXTURE_2D,texture[2]);

//a bunch of scaled cylinders rotated around a point
	for(int i = 0; i < 360; i += 36)
	{
		cylinder(0,0,0,i,0,0,1,0.15,1.5,0.15,2);
	}	

//making a ring to go through the cylinders
//top
	glBegin(GL_QUAD_STRIP);
	for(int j = 0; j <= 360; j += 10)
	{
		glNormal3f(Cos(j),Sin(j),0);
		glTexCoord2f(0,0.5*j);
		glVertex3f(Cos(j),Sin(j),0.2);
		glTexCoord2f(1,0.5*j);
		glVertex3f(Cos(j),Sin(j),-0.2);
		
	}
	glEnd();

//bottom
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glBegin(GL_QUAD_STRIP);
	for(int k = 0; k <= 360; k += 10)
	{
		glNormal3f(Cos(k),Sin(k),0);
		glTexCoord2f(0,0.5*k);
		glVertex3f(Cos(k)/1.3,Sin(k)/1.3,0.2);
		glTexCoord2f(1,0.5*k);
		glVertex3f(Cos(k)/1.3,Sin(k)/1.3,-0.2);
		
	}
	glEnd();

//front
	glBegin(GL_QUAD_STRIP);
	for(int a = 0; a <= 360; a += 10)
	{
		glNormal3f(0,0,1);
		glTexCoord2f(0,0.5*a);
		glVertex3f(Cos(a),Sin(a),0.2);
		glTexCoord2f(1,0.5*a);
		glVertex3f(Cos(a)/1.3,Sin(a)/1.3,0.2);
	}
	glEnd();

//back
	glBegin(GL_QUAD_STRIP);
	for(int b = 0; b <= 360; b += 10)
	{
		glNormal3f(0,0,-1);
		glTexCoord2f(0,0.5*b);
		glVertex3f(Cos(b),Sin(b),-0.2);
		glTexCoord2f(1,0.5*b);
		glVertex3f(Cos(b)/1.3,Sin(b)/1.3,-0.2);
	}
	glEnd();


	glPopMatrix();	
	glDisable(GL_TEXTURE_2D);

}

//draws the sides of the cannons
void sides(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s)

{
	glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[2]);

//the edge of the circle
	glColor3f(1,1,1);
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= 180; i += 10)
	{
		glNormal3f(Cos(i),Sin(i),0);
		glTexCoord2f(0,0.5*i);
		glVertex3f(Cos(i),Sin(i),0);
		glTexCoord2f(1,0.5*i);
		glVertex3f(Cos(i),Sin(i),-1);
	}
	glEnd();

//sides of the circle
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,0,-1);
	glVertex3f(0,0,-1);
	for(int j = 0; j <= 180; j += 10)
	{
		glTexCoord2f(0,0.5*j);
		glVertex3f(Cos(j),Sin(j),-1);
	}
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,0,1);
	glVertex3f(0,0,0);
	for(int j = 0; j <= 180; j += 10)
	{
		glTexCoord2f(0,0.5*j);
		glVertex3f(Cos(j),Sin(j),0);
	}
	glEnd();

//cube rest under the half circle
	cube(0,-1,-0.5,1,1,0.5,0,2);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

//draws the cannons
void cannon(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s)
{
//overall transformation
	glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

//transformation for sides of cannon
	glPushMatrix();
   glTranslated(-0.1,0,0);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	sides(2,0,0,90,0,1,0,0.8);
	sides(-1,0,0,90,0,1,0,0.8);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

//transformation for bottom of cannon
	glPushMatrix();
	glRotated(130,1,0,0);
   glScaled(1.1,1.1,1.1);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
   for (int i=0;i<180;i+=5)
   {
      glBegin(GL_QUAD_STRIP);
      //glColor3f(0.867,0.719,0.527);
      for (int j=180;j<=360;j+=5)
      {
         Vertex2(j,i);
         Vertex2(j,i+5);
      }
      glEnd();
   }
	
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

//transformation for cylinder part of cannon
	glPushMatrix();
   glTranslated(0,1.28,-1.53);
	glRotated(40,1,0,0);
   glScaled(1.1,1.1,2);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUAD_STRIP);
   for (int k=0;k<=360;k+=10)
   {   
      glNormal3f(Cos(k-90),Sin(k-90),0);
      glTexCoord2f(0,0.002*k); glVertex3f(Cos(k-90),Sin(k-90),+1);
      glTexCoord2f(1,0.002*k); glVertex3f(Cos(k-90),Sin(k-90),-1);
   }   
   glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//draws the skybox
void sky()
{
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
//doing the sides of the skybox
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-30,-30,-30);
   glTexCoord2f(0.25,0); glVertex3f(+30,-30,-30);
   glTexCoord2f(0.25,1); glVertex3f(+30,+30,-30);
   glTexCoord2f(0.00,1); glVertex3f(-30,+30,-30);

   glTexCoord2f(0.25,0); glVertex3f(+30,-30,-30);
   glTexCoord2f(0.50,0); glVertex3f(+30,-30,+30);
   glTexCoord2f(0.50,1); glVertex3f(+30,+30,+30);
   glTexCoord2f(0.25,1); glVertex3f(+30,+30,-30);

   glTexCoord2f(0.50,0); glVertex3f(+30,-30,+30);
   glTexCoord2f(0.75,0); glVertex3f(-30,-30,+30);
   glTexCoord2f(0.75,1); glVertex3f(-30,+30,+30);
   glTexCoord2f(0.50,1); glVertex3f(+30,+30,+30);

   glTexCoord2f(0.75,0); glVertex3f(-30,-30,+30);
   glTexCoord2f(1.00,0); glVertex3f(-30,-30,-30);
   glTexCoord2f(1.00,1); glVertex3f(-30,+30,-30);
   glTexCoord2f(0.75,1); glVertex3f(-30,+30,+30);
   glEnd();

//doing the top of the skybox
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-30,30,-30);
   glTexCoord2f(1,0); glVertex3f(30,30,-30);
   glTexCoord2f(1,1); glVertex3f(30,30,30);
   glTexCoord2f(0,1); glVertex3f(-30,30,30);

	glEnd();

//doing the bottom of the skybox
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-30,-30,-30);
   glTexCoord2f(1,0); glVertex3f(30,-30,-30);
   glTexCoord2f(1,1); glVertex3f(30,-30,30);
   glTexCoord2f(0,1); glVertex3f(-30,-30,30);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void sphere(double x, double y, double z, double angle,
		double rx, double ry, double rz, 
		double s, double s1, double s2, int t)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s1,s2);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   
   glBindTexture(GL_TEXTURE_2D,texture[t]);
   for (int i=0;i<360;i+=5)
   {
      glBegin(GL_QUAD_STRIP);
      //glColor3f(0.867,0.719,0.527);
      for (int j=180;j<=360;j+=5)
      {
         Vertex2(j,i);
         Vertex2(j,i+5);
      }
      glEnd();
   }

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void cone(double x, double y, double z, double angle,
		double rx, double ry, double rz, 
		double s, double s1, double s2, int t)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s1,s2);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[t]);
   
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0,1,0);
   glVertex3f(0,2,0);
   for (int a=0;a<360;a++) {
//surface normal calculations for cone
       float m = sqrt(sin(a)*sin(a) + cos(a)*cos(a));
       glNormal3f((sin(a)/m)*2,1/2,(cos(a)/m)*2);
       glTexCoord2f(sin(a),cos(a));
       glVertex3f(sin(a), 0,cos(a));
   }
   glEnd();

//creating the bottom of the cone
   glBindTexture(GL_TEXTURE_2D,texture[t]);
   glBegin(GL_TRIANGLE_FAN);

//this normal should be pointed straigt down
   glNormal3f(0,-1,0);
   glVertex3f(0, 0, 0);
   for (int b=0; b<360;b++) {
      glTexCoord2f(sin(b),cos(b));
      glVertex3f(sin(b), 0,cos(b));
   }
   glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

//draws the monster(in progress)
void monster(double x, double y, double z, double angle,
		double rx, double ry, double rz, double s)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(angle,rx,ry,rz);
   glScaled(s,s,s);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   
	glColor3f(0.29,0,0.50);
//body of monster
	sphere(0,0,0,0,0,0,0,1,1,1,3);
	cone(0,0,0,0,0,0,0,1,1,1,3);

//arms of monster
	for(int i=180; i <= 360; i+= 30)
	{
		cone(Cos(i)/1.2,Sin(i)/1.2,0,-90+i,0,0,1,0.4,1,0.4,3);
	}

//top arms
//another matrix to rotate them down toward the ship
   glPushMatrix();
   glRotated(8,1,0,0);
   glBindTexture(GL_TEXTURE_2D,texture[3]);

	for(int j=35; j <= 170; j+= 5)
	{
		cylinder(Sin(j)*2,1,Cos(j)*1.5+1.4,j+90,0,1,0,0.3,0.3,0.1,3);
	}

	for(int j=195; j <= 320; j+= 5)
	{
		cylinder(Sin(j)*2,1,Cos(j)*1.5+1.4,j+90,0,1,0,0.3,0.3,0.3,3);
	}
	

	glPopMatrix();

	glPushMatrix();
	glTranslated(1.07,0.6,2.80);
	glRotated(36,0,1,0);

	cone(0,0,0,100,0,0,1,0.3,0.3,0.3,1);

	glPopMatrix();

	glPushMatrix();
	glTranslated(-1.07,0.6,2.85);
	glRotated(-36,0,1,0);

	cone(0,0,0,264,0,0,1,0.3,0.3,0.3,1);

	glPopMatrix();

//half a sphere to draw the face on
	
   glBindTexture(GL_TEXTURE_2D,texture[12]);
	glTexCoord2d(0,0);
	glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   for (int x=-90;x<90;x+=5)
   {
      glBegin(GL_QUAD_STRIP);
      //glColor3f(0.867,0.719,0.527);
      for (int y=-90;y<=90;y+=5)
      {
         Vertex3(y,x);
         Vertex3(y,x+5);
      }
      glEnd();
   }


	glPopMatrix();
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
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        sphere(Position[0],Position[1],Position[2] , 0,0,0,0,0.1,0.1,0.1,0);
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
	//drawing skybox
	sky();

	//drawing boat
   boat(Sin(ro+90)*up,0,Cos(ro+90)*up,ro,0,1,0,1);


	//drawing monster
	if(m == 1)
	{
		monster(0,0.2,-4,0,0,0,0,1);
	}
	water();
	glDisable(GL_LIGHTING);
//tried to add fog but it would cause a lot of lag, also it had
//bad interaction with water could not debug in time

	//adding fog is the flag flag is set
	//if(f == 1)
	//{
	//	glEnable(GL_FOG);
	//	GLint fogMode = GL_EXP;

	//	glFogi(GL_FOG_MODE,fogMode);
	//	//gray fog color
	//	GLfloat fogC[4] = {0.5,0.5,0.5,1.0};
	//	glFogfv(GL_FOG_COLOR, fogC);
	//	glFogf(GL_FOG_DENSITY, 0.3);
	//	glClearColor(0.5,0.5,0.5,1.0);
	//}
	//else
	//{
	//	glDisable(GL_FOG);
	//}

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      //Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
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
      distance = (distance==1) ? 5 : 1;
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
   else if (ch == ' ')
      cannonBall = 1;
   else if (ch == 'o' || ch == 'O')
      m = 1 - m;
   else if (ch == 'f' || ch == 'F')
      f = 1 - f;
   else if (ch == 'a' || ch == 'A')
      ro += 2;
   else if (ch == 'd' || ch == 'D')
      ro -= 2;
   else if (ch == 'w' || ch == 'W')
      up -= 0.5;
   else if (ch == 's' || ch == 'S')
      up += 0.5;
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
   else if (ch=='j' && ambient>0)
      ambient -= 5;
   else if (ch=='J' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='k' && diffuse>0)
      diffuse -= 5;
   else if (ch=='K' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='h' && specular>0)
      specular -= 5;
   else if (ch=='H' && specular<100)
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
   glutInitWindowSize(400,400);
   glutCreateWindow("Hunter Haskins, Final Project");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   texture[0] = LoadTexBMP("hull.bmp");
   texture[1] = LoadTexBMP("cannon.bmp");
   texture[2] = LoadTexBMP("wood2.bmp");
   texture[3] = LoadTexBMP("sail.bmp");
   texture[4] = LoadTexBMP("water.bmp");

	texture[5] = LoadTexBMP("topSky.bmp");
	texture[6] = LoadTexBMP("sideSky.bmp");
	texture[7] = LoadTexBMP("bottomSky.bmp");

	texture[8] = LoadTexBMP("pirate.bmp");
	texture[9] = LoadTexBMP("crate.bmp");
	texture[10] = LoadTexBMP("rope.bmp");
	texture[11] = LoadTexBMP("floor.bmp");
	texture[12] = LoadTexBMP("face.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
