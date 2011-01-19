#include "Global.h"

sf::Image s1, s2, s3, s4, s5, s6;

vector <Cloud> Clouds;

void SkyInit() {
	s1.LoadFromFile("sky/1.png");
	s2.LoadFromFile("sky/2.png");
	s3.LoadFromFile("sky/3.png");
	s4.LoadFromFile("sky/4.png");
	s5.LoadFromFile("sky/5.png");
	s6.LoadFromFile("sky/6.png");
}

void SkyDraw() {
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_3D,0);
	s1.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(-1,-1,-1);
	glTexCoord2f(0,1);
	glVertex3f(-1,-1,1);
	glTexCoord2f(1,1);
	glVertex3f(1,-1,1);
	glTexCoord2f(1,0);
	glVertex3f(1,-1,-1);
	glEnd();
	s2.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(1,-1,-1);
	glTexCoord2f(0,1);
	glVertex3f(1,-1,1);
	glTexCoord2f(1,1);
	glVertex3f(1,1,1);
	glTexCoord2f(1,0);
	glVertex3f(1,1,-1);
	glEnd();
	s3.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(1,1,-1);
	glTexCoord2f(0,1);
	glVertex3f(-1,1,-1);
	glTexCoord2f(1,1);
	glVertex3f(-1,-1,-1);
	glTexCoord2f(1,0);
	glVertex3f(1,-1,-1);
	glEnd();
	s4.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(1,1,-1);
	glTexCoord2f(0,1);
	glVertex3f(1,1,1);
	glTexCoord2f(1,1);
	glVertex3f(-1,1,1);
	glTexCoord2f(1,0);
	glVertex3f(-1,1,-1);
	glEnd();
	s5.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(-1,1,-1);
	glTexCoord2f(0,1);
	glVertex3f(-1,1,1);
	glTexCoord2f(1,1);
	glVertex3f(-1,-1,1);
	glTexCoord2f(1,0);
	glVertex3f(-1,-1,-1);
	glEnd();
	s6.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(1,1,1);
	glTexCoord2f(0,1);
	glVertex3f(1,-1,1);
	glTexCoord2f(1,1);
	glVertex3f(-1,-1,1);
	glTexCoord2f(1,0);
	glVertex3f(-1,1,1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	glEnable(GL_DEPTH_TEST);
}

