#include "Global.h"

sf::Image s1, s2, s3, s4, s5, s6;

vector <Cloud> Clouds;

void SkyInit() {
	s1.LoadFromFile("sky/left.jpg");
	s2.LoadFromFile("sky/right.jpg");
	s3.LoadFromFile("sky/front.jpg");
	s4.LoadFromFile("sky/back.jpg");
	s5.LoadFromFile("sky/bottom.jpg");
	s6.LoadFromFile("sky/top.jpg");
}

void SkyDraw() {
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_3D,0);

	glBindTexture(GL_TEXTURE_2D,0);
	glEnable(GL_DEPTH_TEST);
}

