class Shoulder
{

	GLuint list;
	GLuint texName;
	int numX,numY;
	Point mesh[41][20];
	float sx,sy,sz;
	float funcCalc(float x)
	{
		if(x==-1)
			return 0;
	
		
		if(x<-0.2)
			return (0.8-0.48*(x+1)*(x+1));
		else
			return (0.4928+x*0.18);//(0.32+x*0.18);for x<0
	}
	float fderv(float x)
	{
		if(x<-0.2)
			return -0.96*(x+1);
		else
			return (0.18);
	}
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
		printf("Image could not be opened\n"); return;
	}
	if ( fread(header, 1, 54, file)!=54 )//1-size of data in bytes, total 1x54 bytes read
	{ // If not 54 bytes read : problem
    		printf("Not a correct BMP file\n");
    		return;
    	}
    	if ( header[0]!='B' || header[1]!='M' ){
    		printf("Not a correct BMP file\n");
    		return;
    	}

    	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	
	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue
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
	
  	glGenTextures(1, &texName);
   	glBindTexture(GL_TEXTURE_2D, texName);

   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   	
   	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	delete data;

	}
	public:
	float thx,thy,thz;
	void val(float tx,float ty,float tz,float x,float y,float z)
	{
		thx=tx;
		thy=ty;
		thz=tz;
		sx=x;
		sy=y;
		sz=z;
	}
	void init(const char *str)
	{
		loadBMP_custom(str);
		numX=20;
		numY=19;
		int i=0,j=0;
		float rad=0;
		for(i=0;i<2*numX+1;i++)
		{
			rad=funcCalc((((i-numX)*1.0f)/(numX*1.0f)));
			for(j=0;j<numY+1;j++)
			{
			//filling the mesh x,z revolves at current radius,rad
			mesh[i][j].x=rad*cos(2*(3.141/numY)*j);
			mesh[i][j].y=(((i-numX)*1.0f)/(numX*1.0f));
			mesh[i][j].z=rad*sin(2*(3.141/numY)*j);
			
			//calc texture co-ord
			mesh[i][j].texx=(j*1.0f)/(numY*1.0f);
			mesh[i][j].texy=(mesh[i][j].y+1.0f)/2.0f;
			
			
			//Calculating normals;nx=cos(phi)*sin(th+pi/2);th=tan inverse(derivative at y)
			mesh[i][j].nx=sin((3.141/2)+atan(fderv(mesh[i][j].y)))*cos(2*(3.141/numY)*j);
			mesh[i][j].ny=cos((3.141/2)+atan(fderv(mesh[i][j].y)));
			mesh[i][j].nz=sin((3.141/2)+atan(fderv(mesh[i][j].y)))*sin(2*(3.141/numY)*j);
				
			}
		}
		
		list=glGenLists(1);
		glNewList(list,GL_COMPILE);
	
		glPushMatrix();//save current stack
		glPushAttrib(GL_CURRENT_BIT);//save current state
			
		//glColor3f(1,0,0);
		glBegin(GL_QUADS);
		for(int i=0;i<2*numX;i++)
		{
			for(int j=0;j<numY;j++)
			{
				
				glNormal3f(mesh[i][j].nx,mesh[i][j].ny,mesh[i][j].nz);
				glTexCoord2f(mesh[i][j].texx, mesh[i][j].texy);
				glVertex3f(mesh[i][j].x,mesh[i][j].y,mesh[i][j].z);
				
				glNormal3f(mesh[i+1][j].nx,mesh[i+1][j].ny,mesh[i+1][j].nz);
				glTexCoord2f(mesh[i+1][j].texx, mesh[i+1][j].texy);
				glVertex3f(mesh[i+1][j].x,mesh[i+1][j].y,mesh[i+1][j].z);
				
				glNormal3f(mesh[i+1][j+1].nx,mesh[i+1][j+1].ny,mesh[i+1][j+1].nz);
				glTexCoord2f(mesh[i+1][j+1].texx, mesh[i+1][j+1].texy);
				glVertex3f(mesh[i+1][j+1].x,mesh[i+1][j+1].y,mesh[i+1][j+1].z);
				
				glNormal3f(mesh[i][j+1].nx,mesh[i][j+1].ny,mesh[i][j+1].nz);
				glTexCoord2f(mesh[i][j+1].texx, mesh[i][j+1].texy);
				glVertex3f(mesh[i][j+1].x,mesh[i][j+1].y,mesh[i][j+1].z);
				
			}
		}		
		glEnd();
		glPopAttrib();
		glPopMatrix();
		//glDisable(GL_TEXTURE_2D);
		glEndList();
	
	}
	
	void drawArm()
	{
		glRotatef(thx,1,0,0);
		glRotatef(thy,0,1,0);
		glRotatef(thz,0,0,1);
		glTranslatef(0,sy,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName);
		glScalef(sx,sy,sz);
		
		glCallList(list);
		glDisable(GL_TEXTURE_2D);
		//init();
		//glDisable(GL_TEXTURE_2D);
		//glutSolidTeapot(1);	
		
	}
};

