#include "Planet.h"
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

void Planet::axis(){
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}
void Planet::animateSpin(){
    spin += 0.5f;
    if (spin > 360.0f) {
        spin -= 360.0f;
    }
}
void Planet::draw(u_int texture){
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(center.x + distance, 0.0f, 0.0f); 
    positon = {distance, 0.0f, 0.0f};
    glRotatef(tilt, 1.0f, 0.0f, 0.0f);
    glRotatef(spin, 0.0f, 1.0f, 0.0f);
    axis();
    glutSolidSphere(size,points,points);
    glPopMatrix();
}
void Planet::drawOrbit(){
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0;i<points *2;i++){
        float theta = 2.0f * 3.1415926f * float(i) / float(points * 2);
        float x = distance * cosf(theta);
        float y = distance * sinf(theta);
        glVertex3f(x+center.x,0.0f,y+center.y);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}
void Planet::setTilt(float degree){
    tilt = degree;
}
void Planet::rotateYAxis(float speed){

}
Planet::Planet(float newDistance,float newSize){
    distance = newDistance;
    size = newSize;
}
Planet::Planet() : points(50), size(1.0f), distance(0.0f) {}
void Planet::setDistance(float newDistance){
    distance = newDistance;
    positon = {distance, 0.0f, 0.0f};
}
void Planet::setCentre(point newCentre){
    center = newCentre;
}
point Planet::getPosition(){
    return positon;
}