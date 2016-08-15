#include<iostream>
class box_t
{

public:

GLfloat x;
GLfloat y;
GLfloat z;
GLfloat tx;
GLfloat ty;
GLfloat tz;
GLfloat yl;
int theta;
GLuint tex12;

box_t(GLfloat width, GLfloat height, GLfloat depth, GLfloat thickness)
{

x=width;
y=height;
z=-depth;
tx=thickness;
ty=thickness;
tz=-thickness;
yl=height/3;

};

// Courtesy http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/#Loading__BMP_images_yourself (on 8th Oct 2013)
void loadBMP_custom(const char * imagepath)
{

// Data read from the header of the BMP file
unsigned char header[54],tmp; // Each BMP file begins by a 54-bytes header
unsigned int dataPos;     // Position in the file where the actual data begins
unsigned int width, height;
unsigned int imageSize;   // = width*height*3
// Actual RGB data
unsigned char * data;

// Open the file
FILE * file = fopen(imagepath,"rb");
if (!file)
{
std::cout<<"Image could not be opened\n";
return;
}

if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    std::cout<<"Not a correct BMP file\n";
    return;
}

if ( header[0]!='B' || header[1]!='M' ){
    std::cout<<"Not a correct BMP file\n";
    return;
}

// Read ints from the byte array
dataPos    = *(int*)&(header[0x0A]);
imageSize  = *(int*)&(header[0x22]);
width      = *(int*)&(header[0x12]);
height     = *(int*)&(header[0x16]);

// Some BMP files are misformatted, guess missing information
if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
if (dataPos==0)      dataPos=54; // The BMP header is done that way

// Create a buffer
data = new unsigned char [imageSize];
 
// Read the actual data from the file into the buffer
fread(data,1,imageSize,file);
 
//Everything is in memory now, the file can be closed
fclose(file);

for(int i=0;i<imageSize;i=i+3)
	{
		tmp=data[i];
		data[i]=data[i+2];
		data[i+2]=tmp;
	}
	
// Create one OpenGL texture
//GLuint textureID;
glGenTextures(1, &tex12);
 
// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, tex12);
 
// Give the image to OpenGL
glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}
void initTex()
{
	loadBMP_custom("wood2.bmp");
}
void drawHalfBox(GLfloat x, GLfloat y, GLfloat z, GLfloat tx, GLfloat ty, GLfloat tz)
{

glBegin(GL_QUADS);

//front face
glNormal3f(0,-0.1,1);
glTexCoord2f(0,0); glVertex3f(0,0,0);
glTexCoord2f(0,1); glVertex3f(x,0,0);
glTexCoord2f(1,1); glVertex3f(x,y,0);
glTexCoord2f(1,0); glVertex3f(0,y,0);

//right side face
glNormal3f(1,0,0);
glTexCoord2f(0,0); glVertex3f(x,0,0);
glTexCoord2f(0,1); glVertex3f(x,0,z);
glTexCoord2f(1,1); glVertex3f(x,y,z);
glTexCoord2f(1,0); glVertex3f(x,y,0);

//back face
glNormal3f(0,0,-1);
glTexCoord2f(0,0); glVertex3f(0,0,z);
glTexCoord2f(0,1); glVertex3f(x,0,z);
glTexCoord2f(1,1); glVertex3f(x,y,z);
glTexCoord2f(1,0); glVertex3f(0,y,z);

//left side face
glNormal3f(-1,0,0);
glTexCoord2f(0,0); glVertex3f(0,0,z);
glTexCoord2f(0,1); glVertex3f(0,0,0);
glTexCoord2f(1,1); glVertex3f(0,y,0);
glTexCoord2f(1,0); glVertex3f(0,y,z);

//bottom face
glNormal3f(0,-1,0);
glTexCoord2f(0,0); glVertex3f(0,0,0);
glTexCoord2f(0,1); glVertex3f(x,0,0);
glTexCoord2f(1,1); glVertex3f(x,0,z);
glTexCoord2f(1,0); glVertex3f(0,0,z);

//inner botttom face
glNormal3f(0,1,0);
glTexCoord2f(0,0); glVertex3f(0+tx,0+ty,0+tz);
glTexCoord2f(0,1); glVertex3f(x-tx,0+ty,0+tz);
glTexCoord2f(1,1); glVertex3f(x-tx,0+ty,z-tz);
glTexCoord2f(1,0); glVertex3f(0+tx,0+ty,z-tz);

//inner left side face
glNormal3f(1,0,0);
glTexCoord2f(0,0); glVertex3f(0+tx,0+ty,z-tz);
glTexCoord2f(0,1); glVertex3f(0+tx,0+ty,0+tz);
glTexCoord2f(1,1); glVertex3f(0+tx,y,0+tz);
glTexCoord2f(1,0); glVertex3f(0+tx,y,z-tz);

//inner back face
glNormal3f(0,0,1);
glTexCoord2f(0,0); glVertex3f(0+tx,0+ty,z-tz);
glTexCoord2f(0,1); glVertex3f(x-tx,0+ty,z-tz);
glTexCoord2f(1,1); glVertex3f(x-tx,y,z-tz);
glTexCoord2f(1,0); glVertex3f(0+tx,y,z-tz);

//inner right side face
glNormal3f(-1,0,0);
glTexCoord2f(0,0); glVertex3f(x-tx,0+ty,0+tz);
glTexCoord2f(0,1); glVertex3f(x-tx,0+ty,z-tz);
glTexCoord2f(1,1); glVertex3f(x-tx,y,z-tz);
glTexCoord2f(1,0); glVertex3f(x-tx,y,0+tz);

//front rim
glNormal3f(0,1,0);
glTexCoord2f(0,0); glVertex3f(0,y,0);
glTexCoord2f(0,1); glVertex3f(x,y,0);
glTexCoord2f(1,1); glVertex3f(x-tx,y,0+tz);
glTexCoord2f(1,0); glVertex3f(0+tx,y,0+tz);

//right side rim
glNormal3f(0,1,0);
glTexCoord2f(0,0); glVertex3f(x,y,0);
glTexCoord2f(0,1); glVertex3f(x,y,z);
glTexCoord2f(1,1); glVertex3f(x-tx,y,z-tz);
glTexCoord2f(1,0); glVertex3f(x-tx,y,0+tz);

//back rim
glNormal3f(0,1,0);
glTexCoord2f(0,0); glVertex3f(x,y,z);
glTexCoord2f(0,1); glVertex3f(0,y,z);
glTexCoord2f(1,1); glVertex3f(0+tx,y,z-tz);
glTexCoord2f(1,0); glVertex3f(x-tx,y,z-tz);

//left side rim
glNormal3f(0,1,0);
glTexCoord2f(0,0); glVertex3f(0,y,z);
glTexCoord2f(0,1); glVertex3f(0,y,0);
glTexCoord2f(1,1); glVertex3f(0+tx,y,0+tz);
glTexCoord2f(1,0); glVertex3f(0+tx,y,z-tz);

glEnd();

}

void drawStage(GLfloat x,GLfloat y,GLfloat z)
{

glBegin(GL_QUADS);
//top
glNormal3f(0,1,0);
glTexCoord2f(0,0); glVertex3f(0,y,0);
glTexCoord2f(0,1); glVertex3f(x,y,0);
glTexCoord2f(1,1); glVertex3f(x,y,z);
glTexCoord2f(1,0); glVertex3f(0,y,z);

//bottom
glNormal3f(0,-1,0);
glTexCoord2f(0,0); glVertex3f(0,0,0);
glTexCoord2f(0,1); glVertex3f(x,0,0);
glTexCoord2f(1,1); glVertex3f(x,0,z);
glTexCoord2f(1,0); glVertex3f(0,0,z);

//right
glNormal3f(1,0,0);
glTexCoord2f(0,0); glVertex3f(x,y,0);
glTexCoord2f(0,1); glVertex3f(x,y,z);
glTexCoord2f(1,1); glVertex3f(x,0,z);
glTexCoord2f(1,0); glVertex3f(x,0,0);

//left
glNormal3f(-1,0,0);
glTexCoord2f(0,0); glVertex3f(0,0,z);
glTexCoord2f(0,1); glVertex3f(0,0,0);
glTexCoord2f(1,1); glVertex3f(0,y,0);
glTexCoord2f(1,0); glVertex3f(0,y,z);
glEnd();

}

void drawBox()
{
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, tex12);
 		
glTranslatef(-x/2, -y/2, -z/2); 

drawHalfBox(x, y, z, tx, ty, tz);

glPushMatrix();
glTranslatef(11, 5, -1);
drawStage(10, 1, -26);
glPopMatrix();

glTranslatef(0,y,0);

glTranslatef(0,0,z);
glRotatef(theta, 1,0,0);
glTranslatef(0,0,-z);
glTranslatef(x/2, yl/2, z/2);
glRotatef(180, 1,0,0);
glTranslatef(-x/2, -yl/2, -z/2);
drawHalfBox(x, yl, z, tx, ty, tz);
glDisable(GL_TEXTURE_2D);
		
}


}box(30, 15, 30, 1);

