
#include"human.cpp"
#include"boxModel.cpp"
#include"Lights.cpp"
#include "room.cpp"

float x=-64.0f,y=20.0f,z=59.1f;//while providing gluPerspective near and far plane 1,1000 are actually -1,-1000

float lx=0.0f,ly=.000f,lz=-1.0f;//z +ve axis in out of the screen, -z into the screen
float mX=650,mY=400,th=0,phi=90;
int objFlag=0,numObj=15,animateFlag=0,i=0,j=0,count=0,showFlag=1,now=0,frameCnt=0,countKey=1;
bool record=false,bezier=false;
int lig1=0,lig2=0,lig0=0,time1=0,totTime=0;

Lights l1,l2;
float traj[100][3],tmp[3];
ofstream outfile;
ifstream infile;

unsigned int framenum=0;
unsigned char *pRGB;

int SCREEN_WIDTH=640*2;
int SCREEN_HEIGHT=360*2;

struct State
{
	Human human[2];
	int light[3];
	float cam[3];
	float th,phi;
	float boxAngle,doorAngle;
	int time;
} state[100],curState;
	
State interpolate(State s1,State s2,float a)
{
	State ans=s1;
	//printf("\na=%f\n",a);
	int k=0;
	for(k=0;k<3;k++)
	{
		ans.light[k]=(a>=float(0.9))?s1.light[k]:s2.light[k];
		ans.cam[k]=(1-a)*s1.cam[k]+a*s2.cam[k];
	}
	ans.th=(1-a)*s1.th+a*s2.th;
	ans.phi=(1-a)*s1.phi+a*s2.phi;
	
	ans.boxAngle=(1-a)*s1.boxAngle+a*s2.boxAngle;
	ans.doorAngle=(1-a)*s1.doorAngle+a*s2.doorAngle;
	
	for(k=0;k<2;k++)
	{
		ans.human[k].tx=(1-a)*s1.human[k].tx+(a)*s2.human[k].tx;
		ans.human[k].ty=(1-a)*s1.human[k].ty+(a)*s2.human[k].ty;
		ans.human[k].tz=(1-a)*s1.human[k].tz+(a)*s2.human[k].tz;
		
		ans.human[k].radioL.thz=(1-a)*s1.human[k].radioL.thz+(a)*s2.human[k].radioL.thz;
		ans.human[k].radioR.thz=(1-a)*s1.human[k].radioR.thz+(a)*s2.human[k].radioR.thz;
		
		ans.human[k].armL.thx=(1-a)*s1.human[k].armL.thx+(a)*s2.human[k].armL.thx;
		ans.human[k].armL.thy=(1-a)*s1.human[k].armL.thy+(a)*s2.human[k].armL.thy;
		ans.human[k].armL.thz=(1-a)*s1.human[k].armL.thz+(a)*s2.human[k].armL.thz;
		
		ans.human[k].armR.thx=(1-a)*s1.human[k].armR.thx+(a)*s2.human[k].armR.thx;
		ans.human[k].armR.thy=(1-a)*s1.human[k].armR.thy+(a)*s2.human[k].armR.thy;
		ans.human[k].armR.thz=(1-a)*s1.human[k].armR.thz+(a)*s2.human[k].armR.thz;
		
		ans.human[k].legL.thx=(1-a)*s1.human[k].legL.thx+(a)*s2.human[k].legL.thx;
		ans.human[k].legL.thy=(1-a)*s1.human[k].legL.thy+(a)*s2.human[k].legL.thy;
		ans.human[k].legL.thz=(1-a)*s1.human[k].legL.thz+(a)*s2.human[k].legL.thz;

		ans.human[k].legR.thx=(1-a)*s1.human[k].legR.thx+(a)*s2.human[k].legR.thx;
		ans.human[k].legR.thy=(1-a)*s1.human[k].legR.thy+(a)*s2.human[k].legR.thy;
		ans.human[k].legR.thz=(1-a)*s1.human[k].legR.thz+(a)*s2.human[k].legR.thz;
		
		ans.human[k].face1.thx=(1-a)*s1.human[k].face1.thx+(a)*s2.human[k].face1.thx;
		ans.human[k].face1.thy=(1-a)*s1.human[k].face1.thy+(a)*s2.human[k].face1.thy;
		ans.human[k].face1.thz=(1-a)*s1.human[k].face1.thz+(a)*s2.human[k].face1.thz;
		
		ans.human[k].hip1.thx=(1-a)*s1.human[k].hip1.thx+(a)*s2.human[k].hip1.thx;
		ans.human[k].hip1.thy=(1-a)*s1.human[k].hip1.thy+(a)*s2.human[k].hip1.thy;
		ans.human[k].hip1.thz=(1-a)*s1.human[k].hip1.thz+(a)*s2.human[k].hip1.thz;
		
		ans.human[k].tor.thx=(1-a)*s1.human[k].tor.thx+(a)*s2.human[k].tor.thx;
		ans.human[k].tor.thy=(1-a)*s1.human[k].tor.thy+(a)*s2.human[k].tor.thy;
		ans.human[k].tor.thz=(1-a)*s1.human[k].tor.thz+(a)*s2.human[k].tor.thz;
		
		ans.human[k].toeL.thz=(1-a)*s1.human[k].toeL.thz+(a)*s2.human[k].toeL.thz;
		ans.human[k].toeR.thz=(1-a)*s1.human[k].toeR.thz+(a)*s2.human[k].toeR.thz;
		
		ans.human[k].handL.thz=(1-a)*s1.human[k].handL.thz+(a)*s2.human[k].handL.thz;
		ans.human[k].handR.thz=(1-a)*s1.human[k].handR.thz+(a)*s2.human[k].handR.thz;
		
		ans.human[k].lowLegL.thz=(1-a)*s1.human[k].lowLegL.thz+(a)*s2.human[k].lowLegL.thz;
		ans.human[k].lowLegR.thz=(1-a)*s1.human[k].lowLegR.thz+(a)*s2.human[k].lowLegR.thz;
		
		
	}
	return ans;
}
void writeFile()
{
	int k=0;
	outfile.open("keyframes.txt",ios::out | ios::app);
	if(outfile.is_open())
	{
	outfile<<"Scene\n";
	outfile<<state[frameCnt].time<<" ";
	outfile<<lig0<<" "<<lig1<<" "<<lig2<<" ";
	
	outfile<<x<<" "<<y<<" "<<z<<" ";
	
	outfile<<th<<" ";
	outfile<<phi<<" ";
	
	outfile<<box.theta<<" ";
	outfile<<r1.doorAngle<<"\n";
	
	for(k=0;k<2;k++)
	{
		outfile<<"Human\n";
		outfile<<human[k].tx<<" ";
		outfile<<human[k].ty<<" ";
		outfile<<human[k].tz<<" ";
		
		outfile<<human[k].radioL.thz<<" ";
		outfile<<human[k].radioR.thz<<" ";
		
		outfile<<human[k].armL.thx<<" ";
		outfile<<human[k].armL.thy<<" ";
		outfile<<human[k].armL.thz<<" ";
		
		outfile<<human[k].armR.thx<<" ";
		outfile<<human[k].armR.thy<<" ";
		outfile<<human[k].armR.thz<<" ";
		
		outfile<<human[k].legL.thx<<" ";
		outfile<<human[k].legL.thy<<" ";
		outfile<<human[k].legL.thz<<" ";

		outfile<<human[k].legR.thx<<" ";
		outfile<<human[k].legR.thy<<" ";
		outfile<<human[k].legR.thz<<" ";
		
		outfile<<human[k].face1.thx<<" ";
		outfile<<human[k].face1.thy<<" ";
		outfile<<human[k].face1.thz<<" ";
		
		outfile<<human[k].hip1.thx<<" ";
		outfile<<human[k].hip1.thy<<" ";
		outfile<<human[k].hip1.thz<<" ";
		
		outfile<<human[k].tor.thx<<" ";
		outfile<<human[k].tor.thy<<" ";
		outfile<<human[k].tor.thz<<" ";
		
		outfile<<human[k].toeL.thz<<" ";
		outfile<<human[k].toeR.thz<<" ";
		
		outfile<<human[k].handL.thz<<" ";
		outfile<<human[k].handR.thz<<" ";
		
		outfile<<human[k].lowLegL.thz<<" ";
		outfile<<human[k].lowLegR.thz<<" ";
			
		outfile<<"\n";
	}
	}
	outfile.close();
	
}


void loadFile()
{
	int k=0;
	char str[100];
	
	infile.open("keyframes.txt",ios::in);
	//printf("here");
	if(infile.is_open())
	{
	while(infile.good())
	{
	infile>>str;
	infile>>state[frameCnt].time;
	//cout<<state[frameCnt].time<<"\n";
	infile>>state[frameCnt].light[0]>>state[frameCnt].light[1]>>state[frameCnt].light[2];
	
	infile>>state[frameCnt].cam[0]>>state[frameCnt].cam[1]>>state[frameCnt].cam[2];
	
	traj[count][0]=state[frameCnt].cam[0];
	traj[count][1]=state[frameCnt].cam[1];
	traj[count][2]=state[frameCnt].cam[2];
	count++;
	
	//cout<<traj[count-1][0]<<"\n"<<traj[count-1][1]<<"\n";
	infile>>state[frameCnt].th;
	infile>>state[frameCnt].phi;
	
	infile>>state[frameCnt].boxAngle;
	infile>>state[frameCnt].doorAngle;
	
	for(k=0;k<2;k++)
	{
		state[frameCnt].human[k].initModel("shirt.bmp","skin.bmp","face.bmp","jeans.bmp","palm.bmp","hat.bmp");
		infile>>str;
		infile>>state[frameCnt].human[k].tx;
		infile>>state[frameCnt].human[k].ty;
		infile>>state[frameCnt].human[k].tz;
		
		infile>>state[frameCnt].human[k].radioL.thz ;
		infile>>state[frameCnt].human[k].radioR.thz ;
		
		infile>>state[frameCnt].human[k].armL.thx ;
		infile>>state[frameCnt].human[k].armL.thy ;
		infile>>state[frameCnt].human[k].armL.thz ;
		
		//cout<<state[frameCnt].human[k].armL.thx<<"\n"<<state[frameCnt].human[k].armL.thy<<"\n";
		
		
		infile>>state[frameCnt].human[k].armR.thx ;
		infile>>state[frameCnt].human[k].armR.thy ;
		infile>>state[frameCnt].human[k].armR.thz ;
		
		infile>>state[frameCnt].human[k].legL.thx ;
		infile>>state[frameCnt].human[k].legL.thy ;
		infile>>state[frameCnt].human[k].legL.thz ;

		infile>>state[frameCnt].human[k].legR.thx ;
		infile>>state[frameCnt].human[k].legR.thy ;
		infile>>state[frameCnt].human[k].legR.thz ;
		
		infile>>state[frameCnt].human[k].face1.thx ;
		infile>>state[frameCnt].human[k].face1.thy ;
		infile>>state[frameCnt].human[k].face1.thz ;
		
		infile>>state[frameCnt].human[k].hip1.thx ;
		infile>>state[frameCnt].human[k].hip1.thy ;
		infile>>state[frameCnt].human[k].hip1.thz ;
		
		infile>>state[frameCnt].human[k].tor.thx ;
		infile>>state[frameCnt].human[k].tor.thy ;
		infile>>state[frameCnt].human[k].tor.thz ;
		
		infile>>state[frameCnt].human[k].toeL.thz ;
		infile>>state[frameCnt].human[k].toeR.thz ;
		
		infile>>state[frameCnt].human[k].handL.thz ;
		infile>>state[frameCnt].human[k].handR.thz ;
		
		infile>>state[frameCnt].human[k].lowLegL.thz ;
		infile>>state[frameCnt].human[k].lowLegR.thz ;
			
		
	}
	frameCnt++;
	}
	
	}
	infile.close();
	
}
void capture_frame(unsigned int framenum)
{
  //global pointer float *pRGB
  pRGB = new unsigned char [3 * (SCREEN_WIDTH+1) * (SCREEN_HEIGHT + 1) ];


  // set the framebuffer to read
  //default for double buffered
  glReadBuffer(GL_BACK);

  glPixelStoref(GL_PACK_ALIGNMENT,1);//for word allignment
  
  glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pRGB);
  char filename[200];
  sprintf(filename,"frame_%04d.ppm",framenum);
  std::ofstream out(filename, std::ios::out);
  out<<"P6"<<std::endl;
  out<<SCREEN_WIDTH<<" "<<SCREEN_HEIGHT<<" 255"<<std::endl;
  out.write(reinterpret_cast<char const *>(pRGB), (3 * (SCREEN_WIDTH+1) * (SCREEN_HEIGHT + 1)) * sizeof(int));
  out.close();

  //function to store pRGB in a file named count
  delete pRGB;
}

void gInit()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glClearColor(0.0,0.0,0.0,0.0);


	glutWarpPointer(650, 400);
	
	GLfloat light_ambient[]={1, 1, 1, 1.0};

	GLfloat light_diffuse[]={0.7, 0.7, 0.7, 1.0};
	GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[]={-22.0, 55.0, 15.0, 1.0};
	
	GLfloat light_diffuse1[]={0.2, 0.2, 0.2, 1.0};
	GLfloat light_specular1[]={0.5, 0.5, 0.5, 1.0};
	GLfloat light_position1[]={-10.0, 20.0, -65.0, 1.0};
	
	GLfloat light_diffuse2[]={0.2, 0.2, 0.2, 1.0};
	GLfloat light_specular2[]={0.5, 0.5, 0.5, 1.0};
	GLfloat light_position2[]={-75.0, 20.0, -65.0, 1.0};
	
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);	

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);	

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);	

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);
}

int fact(int n)
{
	int ans=1;
	for(int i=n;i>=1;i--)
		ans*=i;
	return ans;
}
int nchoosek(int n,int k)
{
	return fact(n)/(fact(k)*fact(n-k));

}

void calcTraj(int time)
{	
	for(int k=0;k<countKey;k++)
		time+=state[k].time;
	if(time<totTime)
	{
		tmp[0]=0;
		tmp[1]=0;
		tmp[2]=0;
		for(i=count-1;i>=0;i--)
		{
			tmp[0]+=nchoosek(count-1,i)*traj[i][0]*pow(((float)(time-1)/(float)(totTime)),i)*pow((1-(float)(time-1)/(float)(totTime)),count-1-i);
			tmp[1]+=nchoosek(count-1,i)*traj[i][1]*pow(((float)(time-1)/(float)(totTime)),i)*pow((1-(float)(time-1)/(float)(totTime)),count-1-i);
			tmp[2]+=nchoosek(count-1,i)*traj[i][2]*pow(((float)(time-1)/(float)(totTime)),i)*pow((1-(float)(time-1)/(float)(totTime)),count-1-i);
			
			//glTranslatef(-x/2,-y/2,-z/2);
		}
		//printf("t=%f\nj=%d",((float)(j-1)/(float)(50.0)),nchoosek(count-1,0));
	}
}

void flyBy()
{
		
		curState=interpolate(state[countKey-1],state[countKey],(float)(1.0*time1/state[countKey].time));
		human[0]=curState.human[0];
		human[1]=curState.human[1];
	
		th=curState.th;
		phi=curState.phi;
	
		if(bezier)
		{
		calcTraj(time1);
		x=tmp[0];
		y=tmp[1];
		z=tmp[2];
		lx = -cos(phi)*cos(th);
		lz = -sin(phi)*cos(th);
		ly = -sin(th);
		}
		else
		{
		x=curState.cam[0];
		y=curState.cam[1];
		z=curState.cam[2];
		lx = -cos(phi)*cos(th);
		lz = -sin(phi)*cos(th);
		ly = -sin(th);
		}
	
		lig0=curState.light[0];
		lig1=curState.light[1];
		lig2=curState.light[2];
	
		box.theta=curState.boxAngle;
		r1.doorAngle=curState.doorAngle;
		
		time1++;
	//gluLookAt(x, y, z, x + lx,y + ly,z + lz,0.0f,1.0f,0.0f);
	if(time1>state[countKey].time)
	{
		time1=0;
		countKey++;
	}
	
}	
void Timer(int extra)
{
    if(animateFlag==1)
    {
    	flyBy();
    	
    }
   if(countKey>=(frameCnt-1))
   {
    	animateFlag=0;
    	//record=false;
    	//framenum=0;
    	record=false;
    	countKey=1;
    	totTime=0;
    }
    glutTimerFunc(80,Timer,0);
}
void changeSize(int w, int h)	{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	mX=w/2;
	mY=h/2;
	// Set the clipping volume
	gluPerspective(55,1.0f * w / h,1,500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		x + lx,y + ly,z + lz,
		0.0f,1.0f,0.0f);
}


void orientMe() 
{

	//x = k*cos(phi)*cos(th);
	//z = k*sin(phi)*cos(th);
	//y = k*sin(th);
	lx = -cos(phi)*cos(th);
	lz = -sin(phi)*cos(th);
	ly = -sin(th);

	glLoadIdentity();
	gluLookAt(x, y, z, 
		x+lx,y+ly,z+lz,
		0.0f,1.0f,0.0f);
}
void moveCamera(int flag)
{
	x=x+0.5*flag*lx;
	y=y+0.5*flag*ly;
	z=z+0.5*flag*lz;
	glLoadIdentity();
	gluLookAt(x, y, z, 
		x+lx,y+ly,z+lz,
		0.0f,1.0f,0.0f);
}
void sideCamera(int flag)
{
	x=x+0.5*flag*-cos(phi+3.141/2.0)*cos(th);
	//y=y+0.5*flag*-sin(th);
	z=z+0.5*flag*-sin(phi+3.141/2.0)*cos(th);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		x+lx,y+ly,z+lz,
		0.0f,1.0f,0.0f);
}

void inputKey(unsigned char c, int x1, int y1) 
{

	switch (c) 
	{
	case 27:outfile.close();exit(0);break;	
	//case 'a': phi+=0.05;break;//left
	//case 'd': phi-=0.05;break;//right
	//case 'w': th+=0.05;break;//up
	//case 's': th-=0.05;break;//down
	
	case 'w': moveCamera(1);break;//up
	case 's': moveCamera(-1);break;//down
	case 'a': sideCamera(-1);break;//left
	case 'd': sideCamera(1);break;//right
	
	
	case 'W': moveCamera(5);break;//up
	case 'S': moveCamera(-5);break;//down
	case 'A': sideCamera(-5);break;//left
	case 'D': sideCamera(5);break;//right
	
	case '5':record=(record?false:true);break;
	case 'p':bezier=!bezier;break;
	case 'f': showFlag=(showFlag+1)%2;break;
	
	case 'z':r1.doorAngle-=5;break;
	case 'Z':r1.doorAngle+=5;break;
	
	case 'q':objFlag=(objFlag+1)%numObj;break;
	case 'e':objFlag=((objFlag-1)<0?(objFlag-1)+numObj:(objFlag-1))%numObj;break;
	case 'r':human[0].initModel("shirt.bmp","skin.bmp","face.bmp","jeans.bmp","palm.bmp","hat.bmp");
	human[1].initModel("shirt.bmp","skin.bmp","face.bmp","jeans.bmp","palm.bmp","hat.bmp");break;
	
	case 'b':glutWarpPointer(mX, mY); break;
	
	/*case '1':
		traj[count][0]=x+2*lx;
		traj[count][1]=y+2*ly;
		traj[count][2]=z+2*lz;
		count++;
		
		
	break;
	*/
	case '1':
		lig0=(lig0+1)%2;
		(lig0==0)?glEnable(GL_LIGHT0):glDisable(GL_LIGHT0);
		if(lig0==0)
			loadBMP_custom("lampBright.bmp", r1.texTube);
		else
			loadBMP_custom("lamp2.bmp", r1.texTube);
		break;
	
	case '2':
		lig1=(lig1+1)%2;
		(lig1==0)?glDisable(GL_LIGHT1):glEnable(GL_LIGHT1);
		if(lig1==1)
			l1.init("lampBright.bmp");
		else
			l1.init("lamp2.bmp");
		break;
	
	case '3':
		lig2=(lig2+1)%2;
		(lig2==0)?glDisable(GL_LIGHT2):glEnable(GL_LIGHT2);
		if(lig2==1)
			l2.init("lampBright.bmp");
		else
			l2.init("lamp2.bmp");
		break;
	
		
	case '0':
		x=-64.0f;y=20.0f;z=59.1f;
		count=0;
		time1=0;
		th=0;phi=90;
		break;
	
	case 'v':
		loadFile();
		//cout<<"here in v";
		//if(frameCnt>=2)
		{	
			animateFlag=(animateFlag+1)%2;
			framenum=0;
			time1=1;
		
			for(i=0;i<frameCnt;i++)
				totTime+=state[i].time;
		}

	case 'o':
		if (box.theta>-90 && box.theta<=0)
		{
			box.theta-=2;
		}
		break;
	
	case 'x':human[now].tx+=.1;break;
	case 'X':human[now].tx-=.1;break;
	case 'g':human[now].ty+=.1;break;
	case 'G':human[now].ty-=.1;break;
	case 'n':human[now].tz+=.1;break;
	case 'N':human[now].tz-=.1;break;
	
	case '6':now=(now+1)%2;break;

	//capture state
	case 'm':
	cout<<"Enter time for frame no "<<(frameCnt+1)<<"(0 for 1st frame):";
	cin>>state[frameCnt].time;
	writeFile();
	
/*	state[frameCnt].human[0]=human[0];
	state[frameCnt].human[1]=human[1];
	
	state[frameCnt].th=th;
	state[frameCnt].phi=phi;
	
	state[frameCnt].light[0]=lig0;
	state[frameCnt].light[1]=lig1;
	state[frameCnt].light[2]=lig2;
	
	state[frameCnt].cam[0]=x;
	state[frameCnt].cam[1]=y;
	state[frameCnt].cam[2]=z;
	
	state[frameCnt].boxAngle=box.theta;
	state[frameCnt].doorAngle=r1.doorAngle;
	frameCnt++;
	break;
*/	
	
	}
	if(objFlag==1)//left arm low
	{
		switch (c)
		{
		case 'u': 
			if(human[now].radioL.thz+0.2<0 && human[now].radioL.thz+0.2>-150)
				human[now].radioL.thz+=02;
			break;//left
		case 'j': 
			if(human[now].radioL.thz-0.2<0 && human[now].radioL.thz-0.2>-150)
				human[now].radioL.thz-=02;
			break;//right

		}
	}
	else if(objFlag==0)//left arm 
	{
		switch (c)
		{
		case 'u': human[now].armL.thx+=02;break;//left
		case 'j': human[now].armL.thx-=02;break;//right
		case 'y': human[now].armL.thy+=2;break;//up
		case 'i':human[now].armL.thy-=2;break;//down
		case 'h': human[now].armL.thz+=2;break;//up
		case 'k':human[now].armL.thz-=2;break;//down
		}
	}
	else if(objFlag==2)//right arm upper
	{
		switch (c)
		{
		case 'u': 
			//if(human[now].armR.thx+02>0 && human[now].armR.thx+02<150)
			human[now].armR.thx+=02;break;//left
		case 'j': 
			//if(human[now].armR.thx-02>0 && human[now].armR.thx-02<150)
			human[now].armR.thx-=02;break;//right
		case 'y': human[now].armR.thy+=2;break;//up
		case 'i':human[now].armR.thy-=2;break;//down
		case 'h': human[now].armR.thz+=2;break;//up
		case 'k':human[now].armR.thz-=2;break;//down
		}

	}
	else if(objFlag==3)//right arm lower
	{
		switch (c)
		{
		case 'j': 
			if(human[now].radioR.thz+02<0 && human[now].radioR.thz+02>-150)
				human[now].radioR.thz+=02;
			break;//left
		case 'u': 
			if(human[now].radioR.thz-02<0 && human[now].radioR.thz-02>-150)
				human[now].radioR.thz-=02;
			break;

		}
	}
	if(objFlag==5)//left leg lower
	{
		switch (c)
		{
		case 'u': human[now].lowLegL.thz+=02;break;//left
		case 'j': human[now].lowLegL.thz-=02;break;//right

		}
	}
	else if(objFlag==4)//left leg upper
	{
		switch (c)
		{
		case 'h': human[now].legL.thx+=02;break;//left
		case 'k': human[now].legL.thx-=02;break;//right
		case 'i': human[now].legL.thy+=2;break;//up
		case 'y':human[now].legL.thy-=2;break;//down
		case 'u': human[now].legL.thz+=2;break;//up
		case 'j':human[now].legL.thz-=2;break;//down
		}
	}
	else if(objFlag==6)//right arm upper
	{
		switch (c)
		{
		case 'h': human[now].legR.thx+=02;break;//left
		case 'k': human[now].legR.thx-=02;break;//right
		case 'i': human[now].legR.thy+=2;break;//up
		case 'y':human[now].legR.thy-=2;break;//down
		case 'u': human[now].legR.thz+=2;break;//up
		case 'j':human[now].legR.thz-=2;break;//down
		}

	}

	else if(objFlag==7)//right arm lower
	{
		switch (c)
		{
		case 'u': human[now].lowLegR.thz+=02;break;//left
		case 'j': human[now].lowLegR.thz-=02;break;//right

		}
	}
	else if(objFlag==8)//left hand
	{
		switch (c)
		{
		case 'u': human[now].handL.thz+=02;break;//left
		case 'j': human[now].handL.thz-=02;break;//right

		}
	}
	else if(objFlag==9)//right hand
	{
		switch (c)
		{
		case 'u': human[now].handR.thz+=02;break;//left
		case 'j': human[now].handR.thz-=02;break;//right

		}
	}
	else if(objFlag==10)//left toe
	{
		switch (c)
		{
		case 'u': human[now].toeL.thz+=02;break;//left
		case 'j': human[now].toeL.thz-=02;break;//right

		}
	}
	else if(objFlag==11)//right toe lower
	{
		switch (c)
		{
		case 'u': human[now].toeR.thz+=02;break;//left
		case 'j': human[now].toeR.thz-=02;break;//right

		}
	}
	else if(objFlag==12)//right arm lower
	{
		switch (c)
		{
		case 'h': human[now].face1.thx+=02;break;//left
		case 'k': human[now].face1.thx-=02;break;//right
		case 'i': human[now].face1.thy+=2;break;//up
		case 'y':human[now].face1.thy-=2;break;//down
		case 'u': human[now].face1.thz+=2;break;//up
		case 'j':human[now].face1.thz-=2;break;//down

		}
	}
	else if(objFlag==13)//right arm lower
	{
		switch (c)
		{
		case 'h': human[now].tor.thx+=02;break;//left
		case 'k': human[now].tor.thx-=02;break;//right
		case 'i': human[now].tor.thy+=2;break;//up
		case 'y': human[now].tor.thy-=2;break;//down
		case 'u': human[now].tor.thz+=2;break;//up
		case 'j': human[now].tor.thz-=2;break;//down

		}
	}
	else if(objFlag==14)//right arm lower
	{
		switch (c)
		{
		case 'h': human[now].hip1.thx+=02;break;//left
		case 'k': human[now].hip1.thx-=02;break;//right
		case 'i': human[now].hip1.thy+=2;break;//up
		case 'y': human[now].hip1.thy-=2;break;//down
		case 'u': human[now].hip1.thz+=2;break;//up
		case 'j': human[now].hip1.thz-=2;break;//down

		}
	}


	else{}

	phi=(phi>360)?0:phi;
	th=(th>360)?0:th;
	orientMe();
	if(c=='x')
	//printf("x=%f\ny=%f\nz=%f\nframCount=%d\n",human[now].tx,human[now].ty,human[now].tz,frameCnt);
	glutPostRedisplay();

}
void passiveMouse(int x,int y)
{
	//if(animateFlag==0)
	{
	
	th=(float((y)-mY)/float(mY))*3.141;
	phi=90+(float((x)-mX)/float(mX))*3.141;
	
	lx = -cos(phi)*cos(th);
	lz = -sin(phi)*cos(th);
	ly = -sin(th);
	}
}
void mouse(int button, int state, int x1, int y1) 
{
	
  /* if (state == GLUT_DOWN)
   {
   		traj[count][0]=x+2*lx;
		traj[count][1]=y+2*ly;
		traj[count][2]=z+2*lz;
		count++;
		//calcTraj();
		
   }*/
}
void renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(x, y, z, 
		x+lx,y+ly,z+lz,
		0.0f,1.0f,0.0f);

	//render_bitmap_string(0,0.25*k, -0.41*k, GLUT_BITMAP_HELVETICA_18, str);

	glPushMatrix();
	glTranslatef(-10,13.5,-65);
	glScalef(4,4,4);
	l1.drawArm();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-75,13.5,-65);
	glScalef(4,4,4);
	l2.drawArm();
	glPopMatrix();
	

	/*if(animateFlag==0 && showFlag==1)
	{
	for(i=0;i<count;i++)
	{
		glTranslatef(traj[i][0],traj[i][1],traj[i][2]);
		//glTranslatef(x/2,y/2,z/2);
		
		glutSolidSphere(0.02,10,10);
		glTranslatef(-traj[i][0],-traj[i][1],-traj[i][2]);
		
		//glTranslatef(-x/2,-y/2,-z/2);
	}
	glPopMatrix();
	
	
	if(count!=0)
	{
		glPushMatrix();
		glPointSize(3.0);
	
		glBegin(GL_LINE_STRIP);
		for(j=1;j<=101;j++)
		{
		
			glVertex3f(tmp[j][0],tmp[j][1],tmp[j][2]);
			//printf("t=%f\nj=%d",((float)(j-1)/(float)(50.0)),nchoosek(count-1,0));
		}
		glEnd();
	
		glPopMatrix();
	}
	}*/
		
			
	r1.drawFullroom();

	//human[now].drawhuman[now]();
	//glutSolidTeapot(1);
	
	if(record)
		capture_frame(framenum++);
	glutSwapBuffers();

}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1300,800);
	glutCreateWindow("Assignment 2");


	
	gInit();
	human[0].initModel("shirt.bmp","skin.bmp","face.bmp","jeans.bmp","palm.bmp","hat.bmp");
	human[1].initModel("shirt.bmp","skin.bmp","face.bmp","jeans.bmp","palm.bmp","hat.bmp");
	box.initTex();
	
	l1.val(0,0,0,1,1.5,1);
	l1.init("lamp2.bmp");
	
	l2.val(0,0,0,1,1.5,1);
	l2.init("lamp2.bmp");
	
	loadBMP_custom("bedwood.bmp", r1.tex12bed);
	loadBMP_custom("table.bmp", r1.textable);
	loadBMP_custom("sofa.bmp", r1.texsofa);
	loadBMP_custom("drawerFront.bmp", r1.texdrawerFront);
	loadBMP_custom("door.bmp", r1.texdoor);
	loadBMP_custom("wall.bmp", r1.texwall);
	loadBMP_custom("floor3.bmp", r1.texfloor);
	loadBMP_custom("lampBright.bmp", r1.texTube);
	loadBMP_custom("mirror.bmp", r1.texMirror);
	
	
	//x = k*cos(phi)*cos(th);
	//z = k*sin(phi)*cos(th);
	//y=k*sin(th);
	lx = -cos(phi)*cos(th);
	lz = -sin(phi)*cos(th);
	ly=-sin(th);
	//armL.initList();
	glutKeyboardFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc( mouse );
	glutTimerFunc(80,Timer,0);

	glutReshapeFunc(changeSize);
	glutPassiveMotionFunc(passiveMouse);
	glutMainLoop();

	return(0);
}

