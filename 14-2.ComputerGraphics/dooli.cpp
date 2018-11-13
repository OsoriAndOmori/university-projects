/*
본 프로젝트는 2014년 2학기 고려대학교 정보대학 컴퓨터교육과
개설 과목인 컴퓨터그래픽스 과목의 김기훈, 김민형 텀프로젝트이다.

본 프로젝트는 윈도우7 운영체제에서 Microsoft Visual C++ 2010 Express를 이용하여 개발하였다.

본 프로젝트는 삽입된 음악이 나오게 하기 위해서는 "프로젝터 폴더"에
dooli.wav, run.wav, carrol.wav 파일을 추가하여야 한다.

본 프로젝트는 총 세 가지 장면으로 구성되어 있으며 마우스 오른쪽 버튼을 통한 메뉴를 이용하여 전환할 수 있다.

1번 화면은 둘리와 여자친구 공순이가 마치 영화의 시작 장면같은 역동적인 3D움직임을 묘사한것이다.
2번 화면은 둘리와 여자친구 공순이가 데이트를 하는 장면이다.
3번 화면은 크리스마스를 맞은 둘리와 공순이의 사랑이다.

추가적인 기능은 다음과 같다.
키보드의 s키와 w키를 이용하여 실리더 객체의 타입 solid와 wire를 바꿀 수 있다.
키보드의 z키, x키, c키와 shift키를 이용하여 시점을 회전시킬 수 있다.
3번장면에서 키보드의 방향키를 이용하여 둘리를 움직일 수 있다.
*/

#include<windows.h>
#include<MMSystem.h>
#include<glut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<math.h>



#define pi 3.14159265
#define ANGLE 0.2
#define SCALE 0.9
#define RAND 0.01
static double time=0;
static double time2=0;
static double time3=0;
static double time4=0;
static double time5=0;
static double time6=0;
static double snow1=0;
static double snow2=0;
static double snow3=0;
static double snow4=0;
static double snow5=0;
static double snow6=0;
GLfloat x = 0.0;
GLfloat y = 0.0;
GLfloat xpos = 0.0;
GLfloat ypos = 0.0;
GLfloat zpos = 0.0;
static GLfloat theta[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

GLfloat R_Arm_x=0;
GLfloat R_Arm_y=0;
GLfloat L_Arm_x=0;
GLfloat L_Arm_y=0;
GLfloat R_Leg_x=0;
GLfloat R_Leg_y=0;
GLfloat L_Leg_x=0;
GLfloat L_Leg_y=0;
GLfloat Tail=0;

GLfloat R_Arm_x_G=0;
GLfloat R_Arm_y_G=0;
GLfloat L_Arm_x_G=0;
GLfloat L_Arm_y_G=0;
GLfloat R_Leg_x_G=0;
GLfloat R_Leg_y_G=0;
GLfloat L_Leg_x_G=0;
GLfloat L_Leg_y_G=0;
GLfloat Tail_G=0;

GLfloat R=0;
GLfloat R2=0;

GLfloat roll=0;
GLfloat pitch=0;
GLfloat yaw=0;
GLfloat push=0;

GLUquadricObj* cyl;
int start=0;
int mon=0;
int a=0;
int b=0;
int c=0;
static int flag=0;
static int key=0;
static int prekey=0;

#define RUN    1
#define Chris  2
#define EXIT   3

//극좌표 시점 조정 함수
void View(GLfloat roll, GLfloat pitch, GLfloat yaw) {
	glRotatef(-roll, 0.0, 0.0, 1.0);
	glRotatef(-pitch, 1.0, 0.0, 0.0);
	glRotatef(-yaw, 0.0, 1.0, 0.0);
	glTranslatef(0.0,0.0,0.0);
}

//조명 및 설정 초기화 함수
void glInit(void){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	GLfloat ambientLight[]={0.3f,0.3f,0.3f,1.0f};
	GLfloat diffuseLight[]={0.7f,0.7f,0.7f,1.0f};
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat specref[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat position[]={400.0,300.0,-700.0,1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 50);

	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
}

//와이어모드 채움모드 바꾸는 함수
void Change_Wire_Or_Solid(int i){
	if(flag==1)
		gluQuadricDrawStyle(cyl,GLU_LINE);
	}

//오른팔
void DrawL_Arm(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f(0.0,0.9,0.0);
	cyl=gluNewQuadric();
	glTranslatef(-0.05,-0.1,0.0);
	glRotatef(x,a, b, c);
	glRotatef(90.0,1.0, 0.0, 0.0);
	glTranslatef(0.15,0.0,0.0);
	glRotatef(60.0,0.0, 1.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.06,0.03,0.2,50,1);
}
//오른손
void DrawL_Hand(int y,int a,int b, int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.22);
	glRotatef(y,a,b, c);
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl,0.03,10,10);
	glPopMatrix();
}

//왼팔
void DrawR_Arm(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f(0.0,0.9,0.0);
	cyl=gluNewQuadric();
	glTranslatef(0.05,-0.1,0.0);
	glRotatef(x,a,b,c);
	glRotatef(90.0,1.0, 0.0, 0.0);
	glTranslatef(-0.15,0.0,0.0);
	glRotatef(-60.0,0.0, 1.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.06,0.03,0.2,50,1);
}

//왼손
void DrawR_Hand(int y,int a,int b, int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.22);
	glRotatef(y, a , b, c);
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl,0.03,10,10);
	glPopMatrix();
}
//꼬리
void DrawTail(){
	glColor3f(0.0,0.9,0.0);
	glTranslatef(0.0,0.04,0.35);
	glRotatef(Tail,0.0,1.0,0.0);
	glRotatef(10.0, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.08,0.05,0.3,50,1);
	glTranslatef(0.0,0.0,0.3);
	gluSphere(cyl,0.05,10,10);
	glTranslatef(0.0,0.0,-0.0001);
	glRotatef(10.0, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.05,0.03,0.25,50,1);
	glColor3f(0.0,0.9,0.0);
	glTranslatef(0.0,0.0,0.255);
	gluSphere(cyl,0.03,10,10);
	glPopMatrix();
}

//몸통
void DrawBody(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f(0.0,0.9,0.0);
	glTranslatef(-0.1,-0.1,0.0);
	gluSphere(cyl,0.06,10,10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.9,0.0);
	glTranslatef(0.1,-0.1,0.0);
	gluSphere(cyl,0.06,10,10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,0.9,0.0);
	cyl=gluNewQuadric();
	glRotatef(85.0, 1.0, 0.0, 0.0);
	glRotatef(x, a , b, c);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.15,0.1,0.3,50,1);
	glTranslatef(0.0,0.0,0.3);
	glScalef(1.0,1.0,0.8);
	gluSphere(cyl, 0.10, 30, 10);
	glColor3f(1.0,1.0,1.0); //흰색
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(1.0,1.2,0.5);
	glTranslatef(0.0,-0.1,-0.2);
	gluSphere(cyl, 0.10, 30, 10);
}

//오른 다리
void DrawL_Legs(int x, int a,int b,int c){
	glPushMatrix();
	glColor3f(0.0,0.9,0.0);
	cyl=gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	glTranslatef(0.06,-0.3,0.02);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(x, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.04,0.04,0.13,50,1);
}

//오른 발
void DrawL_foot(int y,int a,int b,int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.12);
	glScalef(0.9,0.6,1);
	glRotatef(y,a, b, c);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.02,0.15,50,1);
	glTranslatef(0,-0.01,0);
	glScalef(0.7,0.6,1);
	gluSphere(cyl, 0.06, 30, 10);
	glColor3f(1.0,1.0,1.0);
	glTranslatef(0.0,0.021,0.137);
	gluSphere(cyl,0.033,10,10);
	glPopMatrix();
}

//왼 다리
void DrawR_Legs(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f(0.0,0.9,0.0);
	cyl=gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	glTranslatef(-0.06,-0.3,0.02);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(x, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.04,0.04,0.13,50,1);
}

//왼 발
void DrawR_foot(int y,int a,int b,int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.12);
	glScalef(0.9,0.6,1);
	glRotatef(y,a, b, c);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.02,0.15,50,1);
	glTranslatef(0,-0.01,0);
	glScalef(0.7,0.6,1);
	gluSphere(cyl, 0.06, 30, 10);

	glColor3f(1.0,1.0,1.0);
	glTranslatef(0.0,0.021,0.137);
	gluSphere(cyl,0.033,10,10);
	glPopMatrix();
}

//머리 그리기
void DrawHead(){
	glPushMatrix();
	cyl=gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	glColor3f(0.0,0.9,0.0);
	//head
	glTranslatef(0.0,0.12,0.0);
	gluSphere(cyl, 0.19, 30, 10);
	//left eye
	glTranslatef(-0.1,0.01, 0.17);
	glRotatef(25.0, 0.0, 0.0, 1.0);
	glScalef(1.0,2.0,0.2);
	glColor3f(1.0,1.0,1.0);
	//glRotatef(-25.0,0.0, 1.0, 0.0);
	gluSphere(cyl, 0.03, 10, 10);
	glTranslatef(0.007,-0.01,0.019);
	glColor3f(0.0,0.0,0.0);
	gluSphere(cyl, 0.015, 10, 10);
	glPopMatrix();
	glPushMatrix();

	//right eye
	glTranslatef(0.0,0.12,0.0);
	glTranslatef(0.1,0.01,0.17);
	glRotatef(-25.0, 0.0, 0.0, 1.0);
	glScalef(1.0,2.0,0.2);
	glColor3f(1.0,1.0,1.0);
	gluSphere(cyl, 0.03, 10, 10);
	glTranslatef(-0.007,-0.01,0.019);
	glColor3f(0.0,0.0,0.0);
	gluSphere(cyl, 0.015, 10, 10);

	glPopMatrix();
	glPushMatrix();

	//nose
	glTranslatef(0.0,0.07,0.20);
	glScalef(1.0,1.0,1.0);
	glColor3f(1.0,1.0,1.0);
	gluSphere(cyl, 0.03, 10, 10);
	glPopMatrix();
	glPushMatrix();

	//mouth
	glTranslatef(0.0,-0.08,0.15);
	glScalef(4.0,4.0,2.0);
	glColor3f(1.0,0.69803,0.85098);
	gluSphere(cyl, 0.01, 10, 10);
	glPopMatrix();
	glPushMatrix();

	//머리카락
	glTranslatef(0.08,0.32,-0.05);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(-20.0, 0.0, 1.0, 0.0);
	glColor3f(0.0,0.0,0.0);
	gluCylinder(cyl,0.005,0.005,0.08,50,1);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0.1,0.3,-0.05);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(-60.0, 0.0, 1.0, 0.0);
	glColor3f(0.0,0.0,0.0);
	gluCylinder(cyl,0.005,0.005,0.08,50,1);
	glPopMatrix();
	glPushMatrix();

	//주댕이토러스
	glTranslatef(0.0,0.0,0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glColor3f(0.0,0.9,0.0);
	if(flag==0)	glutSolidTorus(0.08, 0.13, 30, 20);
	if(flag==1) glutWireTorus(0.08, 0.13, 30, 20);
	glPopMatrix();
}

//전체 둘리 그리기
void DrawDooli(){
	DrawBody(0,0,0,0);
	DrawTail();
	DrawHead();
	DrawR_Arm(R_Arm_x,1,0,0);
	DrawR_Hand(R_Arm_y,1,0,0);
	DrawL_Arm(L_Arm_x,1,0,0);
	DrawL_Hand(L_Arm_y,1,0,0);
	DrawL_Legs(L_Leg_x,1,0,0);
	DrawL_foot(L_Leg_y,1,0,0);
	DrawR_Legs(R_Leg_x,1,0,0);
	DrawR_foot(R_Leg_y,1,0,0);
}
//////////////////////공실이/////////////////////

//오른팔
void DrawR_Arm_Gong(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	cyl=gluNewQuadric();
	glTranslatef(-0.05,-0.1,0.0);
	glRotatef(x,a, b, c);
	glRotatef(90.0,1.0, 0.0, 0.0);
	glTranslatef(0.15,0.0,0.0);
	glRotatef(60.0,0.0, 1.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.06,0.03,0.2,50,1);
}
//오른손
void DrawR_Hand_Gong(int y,int a,int b, int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.22);
	glRotatef(y,a,b, c);
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl,0.03,10,10);
	glPopMatrix();
}

//왼팔
void DrawL_Arm_Gong(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	cyl=gluNewQuadric();
	glTranslatef(0.05,-0.1,0.0);
	glRotatef(x,a,b,c);
	glRotatef(90.0,1.0, 0.0, 0.0);
	glTranslatef(-0.15,0.0,0.0);
	glRotatef(-60.0,0.0, 1.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.06,0.03,0.2,50,1);
}

//왼손
void DrawL_Hand_Gong(int y,int a,int b, int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.22);
	glRotatef(y, a , b, c);
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl,0.03,10,10);
	glPopMatrix();
}

//꼬리
void DrawTail_Gong(){
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	glTranslatef(0.0,0.04,0.35);
	glRotatef(Tail_G,0.0,1.0,0.0);
	glRotatef(10.0, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.08,0.05,0.3,50,1);
	glTranslatef(0.0,0.0,0.3);
	gluSphere(cyl,0.05,10,10);
	glTranslatef(0.0,0.0,-0.0001);
	glRotatef(10.0, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.05,0.03,0.25,50,1);
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	glTranslatef(0.0,0.0,0.255);
	gluSphere(cyl,0.03,10,10);
	glPopMatrix();
}

//몸통
void DrawBody_Gong(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	glTranslatef(-0.1,-0.1,0.0);
	gluSphere(cyl,0.06,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	glTranslatef(0.1,-0.1,0.0);
	gluSphere(cyl,0.06,10,10);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1,1,1);
	glTranslatef(0.0,-0.10,0.019);
	glScalef(1.0,0.15,1.0);
	gluSphere(cyl,0.138,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f((GLfloat)171/255,(GLfloat)242/255,(GLfloat)0/255);
	glTranslatef(0.0,-0.11,0.019);
	glScalef(1.0,0.15,1.0);
	gluSphere(cyl,0.138,10,10);
	glPopMatrix();
	glPushMatrix();
    glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	glTranslatef(0.0,-0.12,0.019);
	glScalef(1.0,0.15,1.0);
	gluSphere(cyl,0.138,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1,1,1);
	glTranslatef(0.0,-0.13,0.019);
	glScalef(1.0,0.15,1.0);
	gluSphere(cyl,0.133,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f((GLfloat)171/255,(GLfloat)242/255,(GLfloat)0/255);
	glTranslatef(0.0,-0.14,0.021);
	glScalef(1.0,0.15,1.0);
	gluSphere(cyl,0.13,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	glTranslatef(0.0,-0.15,0.022);
	glScalef(1.0,0.15,1.0);
	gluSphere(cyl,0.128,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f((GLfloat)171/255,(GLfloat)242/255,(GLfloat)0/255);
	glTranslatef(0.0,-0.14,0.05);
	glScalef(1.1,0.15,1.2);
	gluSphere(cyl,0.1,10,10);
	glPopMatrix();

	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	glTranslatef(0.0,-0.15,0.055);
	glScalef(1.17,0.15,1.2);
	gluSphere(cyl,0.1,10,10);
	glPopMatrix();
    glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	cyl=gluNewQuadric();
	glRotatef(85.0, 1.0, 0.0, 0.0);
	glRotatef(x, a , b, c);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.15,0.1,0.3,50,1);
	glTranslatef(0.0,0.0,0.3);
	glScalef(1.0,1.0,0.8);
	gluSphere(cyl, 0.10, 30, 10);
	glColor3f(1.0,1.0,1.0); //흰색
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(1.0,1.2,0.5);
	glTranslatef(0.0,-0.1,-0.2);
	gluSphere(cyl, 0.10, 30, 10);
}

//오른 다리
void DrawR_Legs_Gong(int x, int a,int b,int c){
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	cyl=gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	glTranslatef(0.06,-0.3,0.02);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(x, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.04,0.04,0.13,50,1);
}

//오른 발
void DrawR_foot_Gong(int y,int a,int b,int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.12);
	glScalef(0.9,0.6,1);
	glRotatef(y,a, b, c);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.02,0.15,50,1);
	glTranslatef(0,-0.01,0);
	glScalef(0.7,0.6,1);
	gluSphere(cyl, 0.06, 30, 10);
	glColor3f(1.0,1.0,1.0);
	glTranslatef(0.0,0.021,0.137);
	gluSphere(cyl,0.033,10,10);
	glPopMatrix();
}

//왼 다리
void DrawL_Legs_Gong(int x,int a,int b,int c){
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	cyl=gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	glTranslatef(-0.06,-0.3,0.02);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(x, 1.0, 0.0, 0.0);
	gluCylinder(cyl,0.04,0.04,0.13,50,1);
}

//왼 발
void DrawL_foot_Gong(int y,int a,int b,int c){
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	cyl=gluNewQuadric();
	glTranslatef(0.0,0.0,0.12);
	glScalef(0.9,0.6,1);
	glRotatef(y,a, b, c);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.02,0.15,50,1);
	glTranslatef(0,-0.01,0);
	glScalef(0.7,0.6,1);
	gluSphere(cyl, 0.06, 30, 10);
	glColor3f(1.0,1.0,1.0);
	glTranslatef(0.0,0.021,0.137);
	gluSphere(cyl,0.033,10,10);
	glPopMatrix();
}

//머리 그리기
void DrawHead_Gong(){
	glPushMatrix();
	cyl=gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);

	//head
	glTranslatef(0.0,0.12,0.0);
	gluSphere(cyl, 0.19, 30, 10);

	//left eye
	glTranslatef(-0.1,0.01, 0.17);
	glRotatef(25.0, 0.0, 0.0, 1.0);
	glScalef(1.0,2.0,0.2);
	glColor3f(1.0,1.0,1.0);
	gluSphere(cyl, 0.03, 10, 10);
	glTranslatef(0.007,-0.01,0.019);
	glColor3f(0.0,0.0,0.0);
	gluSphere(cyl, 0.015, 10, 10);
	glColor3f(0.0,0.0,0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(20.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0,-0.03,-0.058);
	gluCylinder(cyl,0.005,0.005,0.022,50,1);
	glRotatef(10.0, 0.0, 1.0, 0.0);
	glColor3f(0.0,0.0,0.0);
	glTranslatef(-0.015,0.0,0.0);
	gluCylinder(cyl,0.005,0.005,0.022,50,1);
	glPopMatrix();
	glPushMatrix();

	//right eye
	glTranslatef(0.0,0.12,0.0);
	glTranslatef(0.1,0.01,0.17);
	glRotatef(-25.0, 0.0, 0.0, 1.0);
	glScalef(1.0,2.0,0.2);
	glColor3f(1.0,1.0,1.0);
	gluSphere(cyl, 0.03, 10, 10);
	glTranslatef(-0.007,-0.01,0.019);
	glColor3f(0.0,0.0,0.0);
	gluSphere(cyl, 0.015, 10, 10);
	glColor3f(0.0,0.0,0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(-20.0, 0.0, 1.0, 0.0);
	glTranslatef(0.01,-0.015,-0.058);
	gluCylinder(cyl,0.005,0.005,0.022,50,1);
	glRotatef(10.0, 0.0, 1.0, 0.0);
	glColor3f(0.0,0.0,0.0);
	glTranslatef(-0.015,0.0,0.0);
	gluCylinder(cyl,0.005,0.005,0.022,50,1);
	glPopMatrix();
	glPushMatrix();

	//nose
	glTranslatef(0.0,0.07,0.20);
	glScalef(1.0,1.0,1.0);
	glColor3f(1.0,1.0,1.0);
	gluSphere(cyl, 0.03, 10, 10);
	glPopMatrix();
	glPushMatrix();

	//mouth
	glTranslatef(0.0,-0.08,0.15);
	glScalef(4.0,4.0,2.0);
	glColor3f(1.0,0.69803,0.85098);
	gluSphere(cyl, 0.01, 10, 10);
	glPopMatrix();
	glPushMatrix();

	//리본
	glTranslatef(0.0,0.32,0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	gluCylinder(cyl,0.02,0.045,0.08,50,1);
	glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	glTranslatef(0.0,0.0,-0.075);
	glScalef(1.0,1.0,0.38);
	gluSphere(cyl,0.046,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	glTranslatef(0.0,0.32,0.0);
	gluSphere(cyl,0.032,10,10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.32,0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	gluCylinder(cyl,0.02,0.045,0.08,50,1);
	glColor3f((GLfloat)255/255,(GLfloat)0/255,(GLfloat)127/255);
	glTranslatef(0.0,0.0,-0.075);
	glScalef(1.0,1.0,0.38);
	gluSphere(cyl,0.046,10,10);
	glPopMatrix();
	glPushMatrix();

	//주댕이토러스
	glTranslatef(0.0,0.0,0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glColor3f((GLfloat)255/255,(GLfloat)133/255,(GLfloat)202/255);
	if(flag==0)	glutSolidTorus(0.08, 0.13, 30, 20);
	if(flag==1) glutWireTorus(0.08, 0.13, 30, 20);
	glPopMatrix();
}

//전체 공실이 그리기
void DrawDooli_Gong(){
	DrawBody_Gong(0,0,0,0);
	DrawTail_Gong();
	DrawHead_Gong();
	DrawL_Arm_Gong(L_Arm_x_G,1,0,0);
	DrawL_Hand_Gong(L_Arm_y_G,1,0,0);
	DrawR_Arm_Gong(R_Arm_x_G,1,0,0);
	DrawR_Hand_Gong(R_Arm_y_G,1,0,0);
	DrawR_Legs_Gong(R_Leg_x_G,1,0,0);
	DrawR_foot_Gong(R_Leg_y_G,1,0,0);
	DrawL_Legs_Gong(L_Leg_x_G,1,0,0);
	DrawL_foot_Gong(L_Leg_y_G,1,0,0);

}

//땅 그리기
void DrawGround(){
	glPushMatrix();
	glColor3f((GLfloat)83/255,1.0,(GLfloat)76/255);
	glTranslatef(0.0,-2.0,0.0);
	glRotatef(0.0-time4+15,1.0, 0.0, 0.0);
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl, 2.0, 30, 90); // grass
	glColor3f((GLfloat)156/255,(GLfloat)91/255,(GLfloat)54/255);
	glScalef(0.9,1.0,1.0);
	gluSphere(cyl, 2.03, 30, 90); // road
	glPopMatrix();
	}
void DrawTree(){
   cyl=gluNewQuadric();
   Change_Wire_Or_Solid(flag);
   glColor3f((GLfloat)156/255,(GLfloat)91/255,(GLfloat)54/255);
   gluCylinder(cyl,0.1,0.1,0.3,50,1);

   //나뭇잎
   glTranslatef(0.0,0.0,0.27);
   glColor3f((GLfloat)34/255,(GLfloat)116/255,(GLfloat)28/255);
   gluCylinder(cyl,0.36,0.1,0.22,50,1);
   glTranslatef(0.0,0.0,0.2);
   glColor3f((GLfloat)47/255,(GLfloat)157/255,(GLfloat)39/255);
   gluCylinder(cyl,0.325,0.08,0.22,50,1);
   glTranslatef(0.0,0.0,0.2);
   glColor3f((GLfloat)53/255,(GLfloat)182/255,(GLfloat)44/255);
   gluCylinder(cyl,0.29,0.06,0.22,50,1);


   //색깔공
   glPushMatrix();
      glColor3f((sin(time)/2+1),(cos(time)/2+1),(sin(time)/2+1));
      glTranslatef(0.0,0.2,0.1);
      gluSphere(cyl,0.044,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f(((cos(time)/2+1))*1.0, 0.0, 0.0);
      glTranslatef(0.15,0.26,-0.15);
     gluSphere(cyl,0.04,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f((GLfloat)((cos(time)/2+1))*255/255,(GLfloat)((sin(time)/2+1))*187/255,(GLfloat)0/255);
      glTranslatef(-0.2,0.2,-0.3);
      gluSphere(cyl,0.03,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f((GLfloat)(sin(time)/2+1)*128/255,(GLfloat)65/255,(GLfloat)(cos(time)/2+1)*217/255);
      glTranslatef(0.1,0.25,-0.3);
      gluSphere(cyl,0.03,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f(0.0, 0.0, (sin(time)/2+1)*1.0);
      glTranslatef(-0.2,0.2,-0.13);
     gluSphere(cyl,0.035,10,10);
   glPopMatrix();


   glPushMatrix();
      glColor3f(((sin(time)/2+1))*1.0,0.0,0.0);
      glTranslatef(0.03,-0.2,0.1);
      gluSphere(cyl,0.044,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f((GLfloat)((sin(time)/2+1))*128/255,(GLfloat)65/255,(GLfloat)((cos(time)/2+1))*217/255);
      glTranslatef(0.15,-0.26,-0.15);
     gluSphere(cyl,0.04,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f((GLfloat)((sin(time)/2+1))*255/255,(GLfloat)((sin(time)/2+1))*187/255,(GLfloat)0/255);
      glTranslatef(-0.2,-0.2,-0.3);
      gluSphere(cyl,0.03,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f(((sin(time)/2+1))*1,((sin(time)/2+1))*1,0);
      glTranslatef(0.1,-0.25,-0.3);
      gluSphere(cyl,0.03,10,10);
   glPopMatrix();
   glPushMatrix();
      glColor3f((GLfloat)0/255,(GLfloat)((sin(time)/2+1))*216/255,(GLfloat)((sin(time)/2+1))*255/255);
      glTranslatef(-0.2,-0.2,-0.13);
     gluSphere(cyl,0.035,10,10);
   glPopMatrix();

   //왕별

    glPushMatrix();
	  glColor3f((GLfloat)0/255,(GLfloat)((sin(time)/2+1))*216/255,(GLfloat)((cos(time)/2+1))*255/255);
      glColor3f(1.0,1.0,0.0);
      glTranslatef(0.0,0.0,0.27);
      glScalef(0.7,1.0,1.0);
     gluSphere(cyl,0.07,10,10);
        glTranslatef(0.0,0.0,0.01);
     gluCylinder(cyl,0.06,0.001,0.18,50,1);
     glRotatef(72.0, 1, 0, 0);
     gluCylinder(cyl,0.06,0.001,0.18,50,1);
     glRotatef(72.0, 1, 0, 0);
     gluCylinder(cyl,0.06,0.001,0.18,50,1);
     glRotatef(72.0, 1, 0, 0);
     gluCylinder(cyl,0.06,0.001,0.18,50,1);
     glRotatef(72.0, 1, 0, 0);
     gluCylinder(cyl,0.06,0.001,0.18,50,1);
    glPopMatrix();
}


void DrawSnow()
{
	cyl=gluNewQuadric();
    Change_Wire_Or_Solid(flag);
	glPushMatrix();
		glColor3f(1.0,1.0,1.0);
		glTranslatef(0.0,0.0,0.5);
		gluSphere(cyl,0.05,30,30);
		glFlush();
		//glutSwapBuffers();
	glPopMatrix();
}

//4번 장면의 배경 중 별들 전체
void DrawSnowBackGround(){

	glPushMatrix();
		glTranslatef(0.7,2-0.05*snow1,-0.3);
		DrawSnow();
		if(2-0.05*snow1<0)
			snow1 = 0;
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.7,1.5-0.05*snow2,-0.8);
		DrawSnow();
		if(1.5-0.05*snow2<0)
			snow2 = 0;
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,1.7-0.05*snow3,-0.7);
		DrawSnow();
		if(1.7-0.05*snow3<0)
			snow3 = 0;
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.7,1.6-0.07*snow4,0.3);
		DrawSnow();
		if(1.6-0.07*snow4<0)
			snow4 = 0;
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.4,1.9-0.07*snow5,0.5);
		DrawSnow();
		if(1.9-0.07*snow5<0)
			snow5 = 0;
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.8,1.6-0.07*snow6,-0.5);
		DrawSnow();
		if(1.6-0.07*snow6<0)
			snow6 = 0;
	glPopMatrix();

	snow1=snow1+0.7;
	snow2=snow2+0.7;
	snow3=snow3+0.7;
	snow4=snow4+0.7;
	snow5=snow5+0.7;
	snow6=snow6+0.7;
}

/////////////////////////////////////////////////////////////////////////////////

//맨 처음 시작하면 나오는 장면 (다른장면 도중에 q누르면 다시 나오는 장면)
//9마리의 둘리
void ex(){
	glClearColor((GLfloat)206/255,(GLfloat)251/255,(GLfloat)201/255,1.0); //흰색 배경
	glLoadIdentity();
	L_Arm_x=(-40)+sin(time2)*60;
	R_Arm_x=(-80)-L_Arm_x;
	R_Arm_y=-abs(cos(time2)*10);
	L_Arm_y=-abs(-cos(time2)*10);
	R_Leg_y=abs(-sin(time)*30-30)*0.3;
	L_Leg_y=abs(sin(time)*30-30)*0.3;
	R_Leg_x=sin(time)*30;
	L_Leg_x=-R_Leg_x;
	Tail = sin(time)*20;

	L_Arm_x_G=(-40)+sin(time2)*60;
	R_Arm_x_G=(-80)-L_Arm_x;;
	R_Arm_y_G=-abs(cos(time2)*10);
	L_Arm_y_G=-abs(-cos(time2)*10);
	R_Leg_y_G=abs(-sin(time)*30-30)*0.3;
	L_Leg_y_G=abs(sin(time)*30-30)*0.3;
	R_Leg_x_G=sin(time)*30;
	L_Leg_x_G=-R_Leg_x_G;
	Tail_G = sin(time)*20;



	cyl=gluNewQuadric();
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	View(roll, pitch, yaw);   //시점변환 함수
	glRotatef(-180,0,1,0); // 둘리가 정면보게 함
	glRotatef(time5,0,0,1); //둘리가 빙글빙글 돌게 함
	glScalef(0.4/(sin(time3)),0.4/(sin(time3)),0.4/(sin(time3))); // 둘리가 앞으로 왔다가 뒤로 갔다가 반복

	//3x3 정사각형 모양으로 정렬된 둘리 그리기
	glPushMatrix();
	glTranslatef(0.0,0.0,0.0);
	DrawDooli();
	glTranslatef(0.0,1.0,0.0);
	DrawDooli_Gong();
	glTranslatef(0.0,-2.0,0.0);
	DrawDooli_Gong();
	glTranslatef(1.0,0.0,0.0);
	DrawDooli();
	glTranslatef(0.0,1.0,0.0);
	DrawDooli_Gong();
	glTranslatef(0.0,1.0,0.0);
	DrawDooli();
	glTranslatef(-2.0,0.0,0.0);
	DrawDooli();
	glTranslatef(0.0,-1.0,0.0);
	DrawDooli_Gong();
	glTranslatef(0.0,-1.0,0.0);
	DrawDooli();
	glutSwapBuffers();
}
//산책하는 둘리
void Run(){
	glClearColor((GLfloat)187/255,(GLfloat)255/255,(GLfloat)255/255,1.0); //하늘색 배경화면
	glLoadIdentity();
	L_Arm_x=sin(time)*80;
	R_Arm_x=-L_Arm_x;
	R_Arm_y=-abs(sin(time)*60+50);
	L_Arm_y=-abs(-sin(time)*60+50);
	R_Leg_y=abs(-sin(time)*30-30)*0.5;
	L_Leg_y=abs(sin(time)*30-30)*0.5;
	R_Leg_x=sin(time)*30;
	L_Leg_x=-R_Leg_x;
	Tail = sin(time)*20;

	L_Arm_x_G=sin(time)*80;
	R_Arm_x_G=-L_Arm_x_G;
	R_Arm_y_G=-abs(sin(time)*60+50);
	L_Arm_y_G=-abs(-sin(time)*60+50);
	R_Leg_y_G=abs(-sin(time)*30-30)*0.5;
	L_Leg_y_G=abs(sin(time)*30-30)*0.5;
	R_Leg_x_G=sin(time)*30;
	L_Leg_x_G=-R_Leg_x_G;
	Tail_G = sin(time)*20;

	cyl=gluNewQuadric();
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	View(roll, pitch, yaw);   //시점 변환 함수
	glRotatef(180.0,0,1,0); //땅, 둘리가 정면을 보게 함
	DrawGround();    //땅 그리기

	//뒤뚱거리는 둘리 표현
	glRotatef(abs(sin(time)*16),1.0,0.0,0.0);
	glRotatef(sin(time)*16,0.0,1.0,0.0);
	float i=0;
	i=abs(sin(time)*0.08);
	glPushMatrix();
	glTranslatef(0.0,i,0.0);
	glTranslatef(0.0,0.5,0.0);
	glRotatef(-5 ,1.0,0.0,0.0);
	glTranslatef(0.35,0.0,0.0);
	DrawDooli(); //둘리 그리기
	glTranslatef(-0.6,0.0,0.0);
	DrawDooli_Gong();
	glutSwapBuffers();
}
void Christmas(){
	cyl=gluNewQuadric();
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	View(roll, pitch, yaw);   //시점변환 함수
	glClearColor((GLfloat)0/255,(GLfloat)0/255,(GLfloat)0/255,1.0);
	//Floor
	glBegin(GL_QUADS);
		glColor3f((GLfloat)0.5*206/255,(GLfloat)0.5*100/255,(GLfloat)0.5*30/255);
		glVertex3f(1,0,1);
		glVertex3f(1,0,-1);
		glVertex3f(-1,0,-1);
		glVertex3f(-1,0,1);
	glEnd();
	glPushMatrix();
	glRotatef(-90,1.0, 0.0, 0.0);
    glTranslatef(-0.5,-0.5,0.0);
	DrawTree(); //나무 그리기
	glPopMatrix();

	DrawSnowBackGround();

	glPushMatrix();
	glTranslatef(0.5,0.51+0.05*sin(time6),0.3);
	glRotatef(180, 0.0, 1.0, 0.0);

	L_Arm_x_G=sin(time6)*80*0.3;
	R_Arm_x_G=L_Arm_x_G*0.3;
	R_Arm_y_G=-abs(sin(time6)*60+50)*0.3;
	L_Arm_y_G=-abs(sin(time6)*60+50)*0.3;
	R_Leg_y_G=abs(sin(-time6)*30-30)*0.3;
	L_Leg_y_G=abs(sin(-time6)*30-30)*0.3;
	R_Leg_x_G=sin(-time6)*30*0.3;
	L_Leg_x_G=sin(-time6)*30*0.3;
	Tail_G = sin(time6)*20;
	DrawDooli_Gong();
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(0.0,0.5,0.0);
	glTranslatef (xpos, ypos, zpos);
	glRotatef(theta[0], 0.0, 1.0, 0.0);
	DrawDooli(); //둘리 그리기

	glutSwapBuffers();
}


	//키보드 콜백 함수
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'w': //와이어모드로 변경
			flag=1;
			break;
		case 's': //채움모드로 변경
			flag=0;
			break;
		case 'z': //극좌표 시점 roll각도 증가
			roll = roll + 2.0;
			glutPostRedisplay();
			break;
		case 'x': //극좌표 시점 pitch각도 증가
			if( key == RUN && pitch < 150) //둘리가 산책하는 장면에서는 150보다 커지면 땅속이 보여서 한계설정
	            pitch = pitch + 2.0;
			else if (key!=RUN)
	            pitch = pitch + 2.0;
			glutPostRedisplay();
			break;
		case 'c': //극좌표 시점 yaw각도 증가
			yaw = yaw + 2.0;
			glutPostRedisplay();
			break;
		case 'Z': //극좌표 시점 roll각도 감소 (시프트 z)
			roll = roll - 2.0;
			glutPostRedisplay();
			break;
		case 'X': //극좌표 시점 pitch각도 감소 (시프트 x)
			if( key == RUN && pitch > 30) //둘리가 산책하는 장면에서는 30보다 작아지면 땅속이 보여서 한계설정
	            pitch = pitch - 2.0;
			else if (key!=RUN)
	            pitch = pitch - 2.0;
			glutPostRedisplay();
			break;
		case 'C': //극좌표 시점 yaw각도 감소 (시프트c)
			yaw = yaw - 2.0;
			glutPostRedisplay();
			break;
	}
}

void MyKeyboard2(int KeyPressed, int x, int y) {
	switch(KeyPressed) {
		case GLUT_KEY_UP:
			push += 0.1;
			L_Arm_x=sin(push)*80;
			R_Arm_x=-L_Arm_x;
			R_Arm_y=-abs(sin(push)*60+50);
			L_Arm_y=-abs(-sin(push)*60+50);
		    R_Leg_y=abs(-sin(time)*30-30)*0.5;
			L_Leg_y=abs(sin(time)*30-30)*0.5;
	        R_Leg_x=sin(push)*30;
			L_Leg_x=-R_Leg_x;
			Tail = sin(push)*20;
	        xpos -= 0.01*cos(90 * pi/180 + theta[0] * pi/180);
			zpos += 0.01*sin(90 * pi/180 + theta[0] * pi/180);
			if (xpos > 0.7)
				xpos = 0.7;
			if (zpos > 0.7)
				zpos = 0.7;
			if (xpos < -0.7)
				xpos = -0.7;
			if (zpos < -0.7)
				zpos = -0.7;
			break;
		case GLUT_KEY_DOWN:
			push -= 0.1;
            L_Arm_x=sin(push)*80;
            R_Arm_x=-L_Arm_x;
            R_Arm_y=-abs(sin(push)*60+50);
            L_Arm_y=-abs(-sin(push)*60+50);
            R_Leg_y=abs(-sin(time)*30-30)*0.5;
            L_Leg_y=abs(sin(time)*30-30)*0.5;
            R_Leg_x=sin(push)*30;
            L_Leg_x=-R_Leg_x;
            Tail = sin(push)*20;
            xpos += 0.01*cos(90 * pi/180 + theta[0] * pi/180);
            zpos -= 0.01*sin(90 * pi/180 + theta[0] * pi/180);
            if (xpos > 0.7)
               xpos = 0.7;
            if (zpos > 0.7)
               zpos = 0.7;
            if (xpos < -0.7)
               xpos = -0.7;
            if (zpos < -0.7)
               zpos = -0.7;
			break;
		case GLUT_KEY_LEFT: theta[0] -= 5.0; break;
		case GLUT_KEY_RIGHT: theta[0] += 5.0; break;
		}
	glutPostRedisplay();
}

//디스플레이 함수
void MyDisplay(){
	if(start==0){
		PlaySound(TEXT("dooli.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);start++;
	}
	if(key==0){ // 기본화면
		//다른장면에서 넘어온거면 시점 초기화
		if(key != prekey){
			roll=0;
			pitch=0;
			yaw=0;
			PlaySound(TEXT("dooli.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		}
		glLoadIdentity();
		gluLookAt(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		prekey = key; //prekey에 현재장면key값 저장

		ex();
		glPopMatrix( );
	}

	if(key==RUN){// 산책중인 둘리

		//다른장면에서 넘어온거면 시점 초기화
		if(key != prekey){
			PlaySound(NULL, NULL, NULL);
			roll=0;
			pitch=30;
			yaw=30;
			PlaySound(TEXT("run.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		}
		prekey = key; //prekey에 현재장면key값 저장
		Run();
		glPopMatrix( );
	}
	else if(key==Chris){

		//다른장면에서 넘어온거면 시점 초기화
		if(key != prekey){
			PlaySound(NULL, NULL, NULL);
			roll=0;
			pitch=20;
			yaw=20;
			xpos=0;
			ypos=0;
			zpos=0;
			R_Arm_x=0;
			R_Arm_y=0;
			L_Arm_x=0;
			L_Arm_y=0;
			R_Leg_x=0;
			R_Leg_y=10;
			L_Leg_x=0;
			L_Leg_y=10;
			PlaySound(TEXT("carrol.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_LOOP);

		}
      	glLoadIdentity();
		gluLookAt(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		prekey = key; //prekey에 현재장면key값 저장
		Christmas();
		glPopMatrix( );
	}
	else if(key==3){	// stop
		prekey = key;//prekey에 멈춤key값 저장
		PlaySound(NULL, NULL, NULL);
	}
	else if(key==EXIT){ // 기본화면으로 돌아가기

		//다른장면에서 넘어온거면 시점 초기화
		if(key != prekey){
			roll=0;
			pitch=0;
			yaw=0;
		}
		prekey = key; //prekey에 현재장면key값 저장
		ex();
		glPopMatrix( );
	}
}

//타이머콜백함수
void MyTimer(int Value){
	//쓰임에 따라 time 값들을 증가 시킴
	time=time+0.08;
	time2=time2+0.5;
	time3=time3+0.01;
	time4=time4+1.0;
	time5=time5+2.0;
	time6=time6+0.25;
	glutPostRedisplay();
	glutTimerFunc(40,MyTimer,1);
}
//우클릭하여 메뉴선택하면 key값에 각각의 장면 ID 값을 넘겨줌
void MyMainMenu(int entryID){
	key=entryID;
}

//메인함수
int main(int argc, char** argv){
	 //초기화 설정 함수모음
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Dooli");
	glInit();

	//메뉴콜백설정
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddMenuEntry("Intro",0);
	glutAddMenuEntry("Take A Walk",1);
	glutAddMenuEntry("Christmas",2);
	glutAddMenuEntry("Stop",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutSpecialFunc (MyKeyboard2);
	glutTimerFunc(40,MyTimer,1); //타이커 콜백
	glutDisplayFunc(MyDisplay); //디스플레이 함수 실행
	glutMainLoop( );
	return 0;
}