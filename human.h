#include<math.h>
#include<GL/glut.h>
#include<fstream>

#include"armList.h"
#include"torso.h"
#include"shoulder.h"
#include"faceList.h"
#include"hat.h"
#include"hip.h"



class Human
{
	public:
	
	upperArm armL,radioL,radioR,armR;
	upperArm legL,legR,lowLegL,lowLegR;
	Torso tor;
	Shoulder shold;
	Face face1,handL,handR;
	Face toeL,toeR;
	Hat hat1;
	Hip hip1;
	void initModel(const char *shirt,const char *skin,const char *face,const char *leg,const char *palm,const char *hat)
	{
		
		armL.val(135,0,0);
		radioL.val(0,0,0);
		radioR.val(0,0,0);
		armR.val(-135,0,0);
		legL.val(170,0,0);
		legR.val(-170,0,0);
		lowLegL.val(0,0,0);
		lowLegR.val(0,0,0);
		tor.val(0,0,0,1,2,1.5);
		shold.val(0,0,0,1,0.4,1.5);
	        face1.val(0,180,0,1.3,1.3,1.3);
	        handL.val(0,180,0,0.3,0.6,0.6);
	        handR.val(0,180,0,0.3,0.6,0.6);
	   	toeL.val(0,180,90,0.25,0.5,0.5);
	   	toeR.val(0,180,90,0.25,0.5,0.5);
	 	hat1.val(0,0,0,0.6,0.6,0.6);
	 	hip1.val(0,0,0,0.8,0.2,0.8);
	
		
		armL.init(shirt);
		radioL.init(skin);
		armR.init(shirt);
		radioR.init(skin);
		shold.init(shirt);
		face1.init(face);
		legL.init(leg);
		legR.init(leg);
		lowLegL.init(leg);
		lowLegR.init(leg);
		handL.init(palm);
		handR.init(palm);
		toeR.init(palm);
		toeL.init(palm);
		hat1.init(hat);
		tor.init(shirt);
		hip1.init(leg);
	}
	void drawHuman()
	{
		glTranslatef(0,-0.5,0);
		glScalef(0.5,0.5,0.5);
		hip1.drawArm();
		
	
		glPushMatrix();//torso shoulder + torso
		glTranslatef(0,-0.2,0);
		tor.drawArm();
		
		//glScalef(1,2,1.5);
		glPushMatrix();
			glTranslatef(0,1.8,0);
			//glScalef(1,0.2,1);
			shold.drawArm();
		glPopMatrix();	
		
		glPushMatrix();//face
			glTranslatef(0,2,0);
			//glScalef(0.7,1.3,0.7);
			//glRotatef(180,0,1,0);
			face1.drawArm();
			//hat
			glTranslatef(0,0.4,0);
			hat1.drawArm();
			
		glPopMatrix();	
		
	
	
	glPushMatrix();//left arm total
	
	glTranslatef(0,1.8,1);
	//glRotatef(135,1,0,0);
	
	glPushMatrix();
		armL.drawArm();
		glPushMatrix();
			glTranslatef(0,0.9*0.95,0);
			glScalef(0.95,0.95,0.95);
			radioL.drawArm();
			glPushMatrix();
				glTranslatef(0,0.9*0.95,0);
				//glScalef(0.3,0.55,0.3);
				handL.drawArm();
			glPopMatrix();	
		glPopMatrix();
	glPopMatrix();
	
	glPopMatrix();
	
	
	glPushMatrix();//right arm total
	glTranslatef(0,1.8,-1);
	//glRotatef(-135,1,0,0);
	
	glPushMatrix();
		armR.drawArm();
		glPushMatrix();
			glTranslatef(0,0.9*0.95,0);
			glScalef(0.95,0.95,0.95);
			radioR.drawArm();
			glPushMatrix();
				glTranslatef(0,0.9*0.95,0);
				//glScalef(0.3,0.55,0.3);
				handR.drawArm();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	
	glPopMatrix();//for torso
		
	
	glPushMatrix();//right leg
	glTranslatef(0,0,-0.7);
	//glRotatef(-170,1,0,0);
	
	glPushMatrix();
		glScalef(1.5,1.5,1.5);
		legR.drawArm();
		glPushMatrix();
			glTranslatef(0,0.9*0.95,0);
			lowLegR.drawArm();
			glPushMatrix();
				glTranslatef(0,0.9*0.95,0);
				//glScalef(0.15,0.55,0.15);
				//glRotatef(90,1,0,0);
				toeR.drawArm();
			glPopMatrix();
	
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();//left leg
	glTranslatef(0,0,0.7);
	//glRotatef(170,1,0,0);
	
	glPushMatrix();
		glScalef(1.5,1.5,1.5);
		legL.drawArm();
		glPushMatrix();
			glTranslatef(0,0.9*0.95,0);
			lowLegL.drawArm();
			glPushMatrix();
				glTranslatef(0,0.9*0.95,0);
				//glScalef(0.15,0.55,0.15);
				//glRotatef(90,1,0,0);
				toeL.drawArm();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	}
};
Human human;
