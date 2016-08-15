#include<GL/glut.h>
#include<fstream>
#include<math.h>
#include <iostream>
//#include"human.h"
//#include"boxModel.h"
#include "loadbmp.cpp"
using namespace std;

float a=0, b=0, c=0;
//#include "boxModel.cpp"

class room
{
public:
	float w;
	float h;
	float d;
	float doorx;
	float doory;
	float doorw;
float doorAngle;
GLuint tex12bed;
GLuint textable;
GLuint texsofa;
GLuint texdrawerFront;
GLuint texdoor;
GLuint texwall;
GLuint texfloor;
GLuint texTube;
GLuint texMirror;

	room()
	{
		w=150;
		h=70;
		d=100;
		doorx=20;
		doory=50;
		doorw=20;
		doorAngle=90;
	}
	void parallelepiped(float wp, float hp, float dp)
	{
		float x=wp;
		float y=hp;
		float z=-dp;

		glBegin(GL_QUADS);

		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
		glTexCoord2f(1,0); glVertex3f(x,0,0);
		glTexCoord2f(1,1); glVertex3f(x,y,0);
		glTexCoord2f(0,1); glVertex3f(0,y,0);

		glNormal3f(1,0,0);
		glTexCoord2f(0,0); glVertex3f(x,0,0);
		glTexCoord2f(1,0); glVertex3f(x,0,z);
		glTexCoord2f(1,1); glVertex3f(x,y,z);
		glTexCoord2f(0,1); glVertex3f(x,y,0);

		glNormal3f(0,0,-1);
		glTexCoord2f(0,0); glVertex3f(x,0,z);
		glTexCoord2f(1,0); glVertex3f(0,0,z);
		glTexCoord2f(1,1); glVertex3f(0,y,z);
		glTexCoord2f(0,1); glVertex3f(x,y,z);

		glNormal3f(-1,0,0);
		glTexCoord2f(0,0); glVertex3f(0,0,z);
		glTexCoord2f(1,0); glVertex3f(0,0,0);
		glTexCoord2f(1,1); glVertex3f(0,y,0);
		glTexCoord2f(0,1); glVertex3f(0,y,z);

		glNormal3f(0,1,0);
		glTexCoord2f(0,0); glVertex3f(0,y,0);
		glTexCoord2f(1,0); glVertex3f(x,y,0);
		glTexCoord2f(1,1); glVertex3f(x,y,z);
		glTexCoord2f(0,1); glVertex3f(0,y,z);

		glNormal3f(0,-1,0);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
		glTexCoord2f(1,0); glVertex3f(0,0,z);
		glTexCoord2f(1,1); glVertex3f(x,0,z);
		glTexCoord2f(0,1); glVertex3f(x,0,0);

		glEnd();

	}

	void drawTable(float length, float breadth, float leg, float thickness)
	{
		//table base:

		parallelepiped(breadth, thickness, length);
		
		//table legs:
		glPushMatrix();
		glTranslatef(0,-leg,0);

		parallelepiped(thickness, leg, thickness);

		glPushMatrix();
		glTranslatef(breadth,0,0);
		parallelepiped(-thickness, leg, thickness);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(breadth,0,-length);
		parallelepiped(-thickness, leg, -thickness);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,0,-length);
		parallelepiped(thickness, leg, -thickness);
		glPopMatrix();

		glPopMatrix();
	}


	void drawRoom()
	{
		float x=w;
		float y=h;
		float z=-d;

		float dx=doorx;
		float dy=doory;
		float dw=doorw;


		//door:
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texdoor);

		glPushMatrix();
		glTranslatef(dx,0,0);
		glRotatef(doorAngle,0,1,0);
		glTranslatef(-dx,0,0);
		glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3f(dx,0,0);
		glTexCoord2f(1,0); glVertex3f(dx+dw,0,0);
		glTexCoord2f(1,1); glVertex3f(dx+dw,dy,0);
		glTexCoord2f(0,1); glVertex3f(dx,dy,0);
		glEnd();
		glPopMatrix();
	glDisable(GL_TEXTURE_2D);
		

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texwall);
		glBegin(GL_QUADS);
		//front wall: 
		glNormal3f(0,0,-1);
		
		glTexCoord2f(0,0); glVertex3f(0,0,0);
		glTexCoord2f(1,0); glVertex3f(dx,0,0);
		glTexCoord2f(1,1); glVertex3f(dx,dy,0);
		glTexCoord2f(0,1); glVertex3f(0,dy,0);

		glTexCoord2f(0,0); glVertex3f(0,dy,0);
		glTexCoord2f(1,0); glVertex3f(x,dy,0);
		glTexCoord2f(1,1); glVertex3f(x,y,0);
		glTexCoord2f(0,1); glVertex3f(0,y,0);

		glTexCoord2f(0,0); glVertex3f(dx+dw,0,0);
		glTexCoord2f(1,0); glVertex3f(x,0,0);
		glTexCoord2f(1,1); glVertex3f(x,dy,0);
		glTexCoord2f(0,1); glVertex3f(dx+dw,dy,0);
		glEnd();

		glBegin(GL_QUADS);
		//side wall, etc.
		glNormal3f(-1,0,0);
		glTexCoord2f(0,0); glVertex3f(x,0,0);
		glTexCoord2f(1,0); glVertex3f(x,0,z);
		glTexCoord2f(1,1); glVertex3f(x,y,z);
		glTexCoord2f(0,1); glVertex3f(x,y,0);

		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3f(x,0,z);
		glTexCoord2f(1,0); glVertex3f(0,0,z);
		glTexCoord2f(1,1); glVertex3f(0,y,z);
		glTexCoord2f(0,1); glVertex3f(x,y,z);

		glNormal3f(1,0,0);
		glTexCoord2f(0,0); glVertex3f(0,0,z);
		glTexCoord2f(1,0); glVertex3f(0,0,0);
		glTexCoord2f(1,1); glVertex3f(0,y,0);
		glTexCoord2f(0,1); glVertex3f(0,y,z);

		glNormal3f(0,-1,0);
		glTexCoord2f(0,0); glVertex3f(0,y,0);
		glTexCoord2f(1,0); glVertex3f(x,y,0);
		glTexCoord2f(1,1); glVertex3f(x,y,z);
		glTexCoord2f(0,1); glVertex3f(0,y,z);
glEnd();
glDisable(GL_TEXTURE_2D);

glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBindTexture(GL_TEXTURE_2D, texfloor);

glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(0,0); glVertex3f(-x,0,-z);
		glTexCoord2f(1,0); glVertex3f(-x,0,2*z);
		glTexCoord2f(1,1); glVertex3f(2*x,0,2*z);
		glTexCoord2f(0,1); glVertex3f(2*x,0,-z);

		glEnd();
glDisable(GL_TEXTURE_2D);

	}

void drawSofa(float length, float breadth, float height, float leg, float thickness)
{
		parallelepiped(length, height,breadth );
		
		glPushMatrix();
		glTranslatef(0,-leg,0);

		parallelepiped(thickness, leg, thickness);

		glPushMatrix();
		glTranslatef(length,0,0);
		parallelepiped(-thickness, leg, thickness);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(length,0,-breadth);
		parallelepiped(-thickness, leg, -thickness);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,0,-breadth);
		parallelepiped(thickness, leg, -thickness);
		glPopMatrix();

		glPopMatrix();
//back rest:
glPushMatrix();
glTranslatef(0,height,-breadth+thickness);

parallelepiped(length, height, thickness);
glPopMatrix();
//cusions:


}

void drawBed(float length, float breadth, float leg, float back, float thickness)
{


parallelepiped(breadth,thickness, length);
		glPushMatrix();
		glTranslatef(0,-leg,0);

		parallelepiped(thickness, leg, thickness);

		glPushMatrix();
		glTranslatef(breadth,0,0);
		parallelepiped(-thickness, leg, thickness);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(breadth,0,-length);
		parallelepiped(-thickness, leg, -thickness);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,0,-length);
		parallelepiped(thickness, leg, -thickness);
		glPopMatrix();

		glPopMatrix();

//backrest:
glPushMatrix();
glTranslatef(0,thickness, -(length-thickness));
parallelepiped(breadth, back, thickness);
glPopMatrix();


}
void drawSideTable(float length, float breadth, float height)
{
float x=length;
float y=height;
float z=-breadth;
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBindTexture(GL_TEXTURE_2D, texdrawerFront);

glBegin(GL_QUADS);

		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
		glTexCoord2f(1,0); glVertex3f(x,0,0);
		glTexCoord2f(1,1); glVertex3f(x,y/3,0);
		glTexCoord2f(0,1); glVertex3f(0,y/3,0);

		glTexCoord2f(0,0); glVertex3f(0,y/3,0);
		glTexCoord2f(1,0); glVertex3f(x,y/3,0);
		glTexCoord2f(1,1); glVertex3f(x,2*y/3,0);
		glTexCoord2f(0,1); glVertex3f(0,2*y/3,0);

		glTexCoord2f(0,0); glVertex3f(0,2*y/3,0);
		glTexCoord2f(1,0); glVertex3f(x,2*y/3,0);
		glTexCoord2f(1,1); glVertex3f(x,y,0);
		glTexCoord2f(0,1); glVertex3f(0,y,0);

glEnd();
glDisable(GL_TEXTURE_2D);

glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBindTexture(GL_TEXTURE_2D, tex12bed);
glBegin(GL_QUADS);

		glNormal3f(1,0,0);
		glTexCoord2f(0,0); glVertex3f(x,0,0);
		glTexCoord2f(1,0); glVertex3f(x,0,z);
		glTexCoord2f(1,1); glVertex3f(x,y,z);
		glTexCoord2f(0,1); glVertex3f(x,y,0);

		glNormal3f(0,0,-1);
		glTexCoord2f(0,0); glVertex3f(x,0,z);
		glTexCoord2f(1,0); glVertex3f(0,0,z);
		glTexCoord2f(1,1); glVertex3f(0,y,z);
		glTexCoord2f(0,1); glVertex3f(x,y,z);

		glNormal3f(-1,0,0);
		glTexCoord2f(0,0); glVertex3f(0,0,z);
		glTexCoord2f(1,0); glVertex3f(0,0,0);
		glTexCoord2f(1,1); glVertex3f(0,y,0);
		glTexCoord2f(0,1); glVertex3f(0,y,z);

		glNormal3f(0,1,0);
		glTexCoord2f(0,0); glVertex3f(0,y,0);
		glTexCoord2f(1,0); glVertex3f(x,y,0);
		glTexCoord2f(1,1); glVertex3f(x,y,z);
		glTexCoord2f(0,1); glVertex3f(0,y,z);

		glNormal3f(0,-1,0);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
		glTexCoord2f(1,0); glVertex3f(0,0,z);
		glTexCoord2f(1,1); glVertex3f(x,0,z);
		glTexCoord2f(0,1); glVertex3f(x,0,0);

		glEnd();
glDisable(GL_TEXTURE_2D);
}
void drawTube()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texTube);
	glTranslatef(-22,55,19);
	glScalef(4,0.5,0.2);
	glutSolidCube(4);
	glDisable(GL_TEXTURE_2D);
}
void drawMirror()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texMirror);
	glTranslatef(-90,34,-50);
	glScalef(1,4,4);
	glBegin(GL_QUADS);
		glTexCoord2f(1,1);
		glVertex3f(0,5,2);
		
		glTexCoord2f(1,0);
		glVertex3f(0,-5,2);
		
		glTexCoord2f(0,0);
		glVertex3f(0,-5,-2);
		
		glTexCoord2f(0,1);
		glVertex3f(0,5,-2);
	glEnd();
	//glScalef(4,0.5,0.2);
	//glutSolidCube(4);
	glDisable(GL_TEXTURE_2D);
}
void drawFullroom()
{

		
glColor3f(1,1,1);

glPushMatrix();
glTranslatef(-w/2-20,-5,20);
drawRoom();
glPopMatrix();

//Table starts:
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBindTexture(GL_TEXTURE_2D, textable);
glPushMatrix();
	drawTable(20,15,5,1);

	glTranslatef(8,1,-10);
	glScalef(0.1,0.1,0.1);
	
	glTranslatef(30/2, 24/2, 30/2);
	glScalef(2,2,2);
	glPushMatrix();
		glRotatef(-90,0,1,0);
		box.drawBox();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,3,0);
		glRotatef(-180,0,1,0);
		glScalef(1.5,1.5,1.5);
		human[0].drawHuman();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-64/0.25-130,20/0.5+4,59/0.5+100);
		glRotatef(90,0,1,0);
		glScalef(30,30,30);
		human[1].drawHuman();
	glPopMatrix();

glPopMatrix();
glDisable(GL_TEXTURE_2D);
//Table ends

//Sofa starts
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBindTexture(GL_TEXTURE_2D, texsofa);

glPushMatrix();
glTranslatef(25,-4,-25);
glRotatef(-90,0,1,0);
drawSofa(30,12,8,1,2);
glPopMatrix();
glDisable(GL_TEXTURE_2D);
//sofa ends

//Bed starts:
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glBindTexture(GL_TEXTURE_2D, tex12bed);
glPushMatrix();
glTranslatef(-60,0,-10);
drawBed(50,40,5,10,2);
glPopMatrix();
glDisable(GL_TEXTURE_2D);
//Bed ends

//sidetable starts:
glPushMatrix();
glTranslatef(-80,-5,-60);
drawSideTable(10,10,10);
glTranslatef(65,0,0);
drawSideTable(10,10,10);
glPopMatrix();
//sidetable ends
glPushMatrix();
drawTube();
glPopMatrix();
drawMirror();
}

}r1;


