
class Lights
{

	public:
	GLuint lamp,stand;
	GLuint texName,texStand;
	int numX,numY;
	Point mesh1[41][20],mesh2[41][20];
	float sx,sy,sz;
	float funcCalc1(float x)
	{

		return -0.5*x+1;
	}
	float funcCalc2(float x)
	{
		if(x==1 || x==-1)
			return 0;

		return 0.1;
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

		for(int i=0;i<imageSize;i=i+3)
		{
			tmp=data[i];
			data[i]=data[i+2];
			data[i+2]=tmp;
		}


		//Everything is in memory now, the file can be closed
		fclose(file);

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		delete data;

	}
	
	float fderv1(float x)
	{
		return -0.5;
	}
	float fderv2(float x)
	{
		return 0;
	}
	
	
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
		float rad1=0,rad2=0;
		for(i=0;i<2*numX+1;i++)
		{
			rad1=funcCalc1((((i-numX)*1.0f)/(numX*1.0f)));
			rad2=funcCalc2((((i-numX)*1.0f)/(numX*1.0f)));
			
			for(j=0;j<numY+1;j++)
			{
				//filling the mesh x,z revolves at current radius,rad
				mesh1[i][j].x=rad1*cos(2*(3.141/numY)*j);
				mesh1[i][j].y=(((i-numX)*1.0f)/(numX*1.0f));
				mesh1[i][j].z=rad1*sin(2*(3.141/numY)*j);

				//calc texture co-ord
				mesh1[i][j].texx=(j*1.0f)/(numY*1.0f);
				mesh1[i][j].texy=(mesh1[i][j].y+1.0f)/2.0f;


				//Calculating normals;nx=cos(phi)*sin(th+pi/2);th=tan inverse(derivative at y)
				mesh1[i][j].nx=sin((3.141/2)+atan(fderv1(mesh1[i][j].y)))*cos(2*(3.141/numY)*j);
				mesh1[i][j].ny=cos((3.141/2)+atan(fderv1(mesh1[i][j].y)));
				mesh1[i][j].nz=sin((3.141/2)+atan(fderv1(mesh1[i][j].y)))*sin(2*(3.141/numY)*j);
				
				
				
				//filling the mesh x,z revolves at current radius,rad
				mesh2[i][j].x=rad2*cos(2*(3.141/numY)*j);
				mesh2[i][j].y=(((i-numX)*1.0f)/(numX*1.0f));
				mesh2[i][j].z=rad2*sin(2*(3.141/numY)*j);

				//calc texture co-ord
				mesh2[i][j].texx=(j*1.0f)/(numY*1.0f);
				mesh2[i][j].texy=(mesh2[i][j].y+1.0f)/2.0f;


				//Calculating normals;nx=cos(phi)*sin(th+pi/2);th=tan inverse(derivative at y)
				mesh2[i][j].nx=sin((3.141/2)+atan(fderv2(mesh2[i][j].y)))*cos(2*(3.141/numY)*j);
				mesh2[i][j].ny=cos((3.141/2)+atan(fderv2(mesh2[i][j].y)));
				mesh2[i][j].nz=sin((3.141/2)+atan(fderv2(mesh2[i][j].y)))*sin(2*(3.141/numY)*j);


			}
		}

		lamp=glGenLists(1);
		glNewList(lamp,GL_COMPILE);

		glPushMatrix();//save current stack
		glPushAttrib(GL_CURRENT_BIT);//save current state

		//glColor3f(1,0,0);
		glBegin(GL_QUADS);
		for(int i=0;i<2*numX;i++)
		{
			for(int j=0;j<numY;j++)
			{

				glNormal3f(mesh1[i][j].nx,mesh1[i][j].ny,mesh1[i][j].nz);
				glTexCoord2f(mesh1[i][j].texx, mesh1[i][j].texy);
				glVertex3f(mesh1[i][j].x,mesh1[i][j].y,mesh1[i][j].z);

				glNormal3f(mesh1[i+1][j].nx,mesh1[i+1][j].ny,mesh1[i+1][j].nz);
				glTexCoord2f(mesh1[i+1][j].texx, mesh1[i+1][j].texy);
				glVertex3f(mesh1[i+1][j].x,mesh1[i+1][j].y,mesh1[i+1][j].z);

				glNormal3f(mesh1[i+1][j+1].nx,mesh1[i+1][j+1].ny,mesh1[i+1][j+1].nz);
				glTexCoord2f(mesh1[i+1][j+1].texx, mesh1[i+1][j+1].texy);
				glVertex3f(mesh1[i+1][j+1].x,mesh1[i+1][j+1].y,mesh1[i+1][j+1].z);

				glNormal3f(mesh1[i][j+1].nx,mesh1[i][j+1].ny,mesh1[i][j+1].nz);
				glTexCoord2f(mesh1[i][j+1].texx, mesh1[i][j+1].texy);
				glVertex3f(mesh1[i][j+1].x,mesh1[i][j+1].y,mesh1[i][j+1].z);

			}
		}		
		glEnd();
		glPopAttrib();
		glPopMatrix();
		//glDisable(GL_TEXTURE_2D);
		glEndList();
		
		
		stand=glGenLists(1);
		glNewList(stand,GL_COMPILE);

		glPushMatrix();//save current stack
		glPushAttrib(GL_CURRENT_BIT);//save current state

		//glColor3f(1,0,0);
		glBegin(GL_QUADS);
		for(int i=0;i<2*numX;i++)
		{
			for(int j=0;j<numY;j++)
			{

				glNormal3f(mesh2[i][j].nx,mesh2[i][j].ny,mesh2[i][j].nz);
				//glTexCoord2f(mesh2[i][j].texx, mesh2[i][j].texy);
				glVertex3f(mesh2[i][j].x,mesh2[i][j].y,mesh2[i][j].z);

				glNormal3f(mesh2[i+1][j].nx,mesh2[i+1][j].ny,mesh2[i+1][j].nz);
				//glTexCoord2f(mesh2[i+1][j].texx, mesh2[i+1][j].texy);
				glVertex3f(mesh2[i+1][j].x,mesh2[i+1][j].y,mesh2[i+1][j].z);

				glNormal3f(mesh2[i+1][j+1].nx,mesh2[i+1][j+1].ny,mesh2[i+1][j+1].nz);
				//glTexCoord2f(mesh2[i+1][j+1].texx, mesh2[i+1][j+1].texy);
				glVertex3f(mesh2[i+1][j+1].x,mesh2[i+1][j+1].y,mesh2[i+1][j+1].z);

				glNormal3f(mesh2[i][j+1].nx,mesh2[i][j+1].ny,mesh2[i][j+1].nz);
				//glTexCoord2f(mesh2[i][j+1].texx, mesh2[i][j+1].texy);
				glVertex3f(mesh2[i][j+1].x,mesh2[i][j+1].y,mesh2[i][j+1].z);

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
		glTranslatef(0,1,0);
		
		glPushMatrix();
			glScalef(sx,sy,sz);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName);
		
			glCallList(lamp);
			glTranslatef(0,-1,0);
			glCallList(stand);
			glTranslatef(0,-1,0);
			glScalef(1,0.125,1);
			glutSolidCube(2);
		
		glPopMatrix();
		
		glDisable(GL_TEXTURE_2D);
		//init();
		//glDisable(GL_TEXTURE_2D);
		//glutSolidTeapot(1);	

	}
};

