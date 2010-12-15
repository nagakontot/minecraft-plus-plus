#include "Global.h"

sf::Window Window;

void InitGraphics() {
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(90,4/3,0,1000);
}