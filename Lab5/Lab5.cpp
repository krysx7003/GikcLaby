#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Egg.hpp"
#include "Light.hpp"
using namespace std;
HWND consoleWindow;     
HWND glutWindow;

u_int textureID;
GLfloat deg = 0;       
int sx =0,sy = 0,sz = 0; 
bool drawTeapot = true;
int moveMode = 0;
float totalRotationX = 0.0f,totalRotationY = 0.0f,totalRotationZ = 0.0f;
float pix2angle,theta = 0.0f,phi = 0.0f;
int radius = 6,lastX = 0,lastY = 0;
float cameraRotationX = radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float cameraRotationY = radius * sinf((phi*(M_PI/180)));
float cameraRotationZ = radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
Light light1(GL_LIGHT0);
GLfloat light1Radius = 10;
unsigned char *texture1;
unsigned char *texture2;
unsigned char *texture3;

Egg egg(200);
void toggleFocusToConsole() {
	ShowWindow(glutWindow, SW_HIDE);  
    ShowWindow(consoleWindow, SW_SHOWNORMAL);  
    SetForegroundWindow(consoleWindow);        
}

void toggleFocusToGLUT() {
	ShowWindow(consoleWindow, SW_HIDE);
	ShowWindow(glutWindow, SW_SHOWNORMAL);   
    SetForegroundWindow(glutWindow);        
}
void reset_rotation(){
	theta = 0.0f;
	phi = 0.0f;
	lastX = 0;
	lastY = 0;
	cameraRotationX = radius * cosf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
	cameraRotationY = radius * sinf((phi*(M_PI/180.0f)));
	cameraRotationZ = radius * sinf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
}
void printControls(){
	cout<<"==============================\n";
	cout<<"F1 - tryb obrotu obiektu";
	cout<<"F2 - tryb obrotu kamery";
	cout<<"F3 - tryb obrotu swiatlem 1 (Czerwone)";
	cout<<"F4 - tryb obrotu swiatlem 2 (Zielone)";
	cout<<"ESC - Powrot do menu (okno konsolowe)\n";
	cout<<"Nalezy nacisnac i przytrzymac PPM\n";
	cout<<"Ruch myszy w osi X - Obrot osi X\n";
	cout<<"Ruch myszy w osi Y - Obrot osi Y\n";
	cout<<"Scroll up - Przybilizenie obiektu\n";
	cout<<"Scroll down - Oddalenie obiektu\n";
	cout<<"Nacisnij Enter zeby kontynuowac\n"<<flush;
	cin.get();
	cin.get();
}
void axis(){
	glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-5.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -5.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -5.0);
    glVertex3f(0.0, 0.0, 5.0);

    glEnd();
}
void printOptions();
void menu();
void printOptions(){
	int density = egg.getDensity();    
    bool color = egg.getColor(); 
    float scale = egg.getScale(); 
    float pointSize = egg.getPointSize();
	cout<<"==============================\n";
	cout<<"1.Skala obiektow: "<<scale<<"\n";
	cout<<"2.Ilosc punktow: "<<density<<"\n";
	cout<<"3.Promien kamery: "<<radius<<"\n";
	cout<<"4.Wroc do menu"<<"\n";
	cout<<"> ";
	int x;
	cin>>x;
	switch (x){
	case 1:
		cout<<"Nowa skala\n";
		cout<<"> ";
		cin>>scale;
		egg.setScale(scale);
		printOptions();
		break;
	case 2:
		cout<<"Nowa gestosc\n";
		cout<<"> ";
		cin>>density;
		egg.setDensity(density);
		printOptions();
		break;
	case 3:
		cout<<"Nowy promien kamery\n";
		cout<<"> ";
		cin>>radius;
		printOptions();
		break;
	case 4:
		menu();
		break;
	}
}
void menu(){
	toggleFocusToConsole();
	reset_rotation();
	cout<<"==============================\n";
	cout<<"1. Narysuj czajnik\n";
	cout<<"2. Narysuj jajko (trojkaty) \n";
	cout<<"3. Opcje\n";
	cout<<"4. Kontrola\n";
	cout<<"5. Zakoncz program\n";
	cout<<"> ";
	int x;
	cin>>x;
	switch (x){
	case 1:
		drawTeapot = true; 
		break;
	case 2:
		drawTeapot = false;
		break;
	case 3:
		printOptions();
		break;
	case 4:
		printControls();
		menu();
		break;
	case 5:
		exit(0);
		break;
	default:
		cout<<"Podano nieporawny znak\n";
		menu();
		break;
	}
	toggleFocusToGLUT();
	glutPostRedisplay();
}
void specialKey(int key,int x,int y){
	switch (key){
	//F1 - Ruch obiektu
	case GLUT_KEY_F1:
		moveMode = 0;
		break;
	//F2 - Ruch kamery
	case GLUT_KEY_F2:
		moveMode = 1;
		break;
	default:
		break;
	}
}
void normalKey(u_char key,int x,int y){
	switch (key)
	{
	case 27:
		menu();
		break;
	default:
		break;
	}
	if (sx == 0 && sy == 0 && sz == 0) {
        glutIdleFunc(nullptr); 
    }
}

void mouse(int x, int y){
	float dY = y - lastY;
	lastY = y;
	float dX = x - lastX;
	lastX = x;
	theta += dX * pix2angle;
	phi += dY * pix2angle;
	if (phi > 89.0f) {phi = 89.0f;}
    if (phi < -89.0f) {phi = -89.0f;}
	switch(moveMode){
		case 0:
			totalRotationX += dY;
			totalRotationY += dX;
			totalRotationZ += atan2f(dY,dX);
			break;
		case 1:
			cameraRotationX = radius * cosf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
			cameraRotationY = radius * sinf((phi*(M_PI/180.0f)));
			cameraRotationZ = radius * sinf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
			break;
	}
	lastX = x;
	lastY = y;
	glutPostRedisplay();
}
void mouseWheel(int button, int dir, int x, int y){
	if (dir > 0){
        radius -= 1;
    }else{
        radius += 1;
    }
	if(radius<=1){
		radius=1;
	}
	glutPostRedisplay();
}
void display() {
	GLfloat lPos1[] = {0,0,10,1};//x,y,z,czy światło jest odległe
	GLfloat col[] = {1,0,0,1};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraRotationX,cameraRotationY,cameraRotationZ,0,0,0,0,1,0);//Ustawienie kamery
	light1.setPosition(lPos1);
	glEnable(GL_COLOR_MATERIAL);
	
	glRotatef(totalRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(totalRotationY, 0.0f, 1.0f, 0.0f); 
    glRotatef(totalRotationZ, 0.0f, 0.0f, 1.0f);
	
	axis();
	glBindTexture(GL_TEXTURE_2D, textureID);
	if(drawTeapot){
		glColor3f(1.0, 1.0, 1.0); 
		glutSolidTeapot(1);
	}else{
		egg.initMaterial();
		egg.draw();
	}
	glutSwapBuffers();
}
void loadTexture(const char* fileName){
	int width,height,nrChannels;
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else{
		cout << "Failed to load texture!" <<stbi_failure_reason()<< endl;
		system("pause");
		exit(1);
	}
	stbi_image_free(data);
	egg.setTextureSize(height,width);
}	
void Init() {
	pix2angle = 360.0/800;
	
	glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	glFrustum(-1,1,-1,1,2,20);
	glMatrixMode(GL_MODELVIEW);
	// Ustawia kierunek frontowych ścianek jako przeciwny do ruchu wskazówek zegara
    glFrontFace(GL_CW);
	// Włącza culling, czyli pomijanie tylnych ścianek
	glEnable(GL_CULL_FACE);
    // Ustawia pomijanie tylnych ścianek
    glCullFace(GL_BACK);
	// Kolor stały
	light1.setColor(1.0,1.0,1.0);
	light1.initLight();
	//Drugie światło
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //Włączenie oświetlenia
	glEnable(GL_LIGHT0); //Dodanie źródła światła

	glEnable(GL_TEXTURE_2D); //Włącza teksturowanie
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenTextures(1, &textureID);	
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	loadTexture("tekstura.tga");
}
// Sprawko do 15 w pon
// Trzy tekstury
// 1(Głęboko) Strukturalna
// 1 Pośrednia
// 1 Bez ustrukturyzowania
// Bez punktów i linii
// Jedno światło ambientowe

int main(int argc, char** argv){
	consoleWindow = GetConsoleWindow();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Lab 5 - Tekstury");
	glutWindow = FindWindowW(NULL,L"Lab 5 - Tekstury");
	Init();
	glutDisplayFunc(display);
	glutIdleFunc(nullptr);
	glutKeyboardFunc(normalKey);
	glutSpecialFunc(specialKey);
	glutMotionFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	menu();
	glutMainLoop();
	
	
	return 0;
}