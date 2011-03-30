#include "Global.h"
GLuint texturelol;//make this an array soon plz :3

void loadTexture( const char* filename)
{
	sf::Image* image = new sf::Image;
	if(image->LoadFromFile(filename))
	{
		cout << "Loaded texture succesfully :D"<< endl;
	}
    glGenTextures(1, &texturelol);
    glBindTexture(GL_TEXTURE_2D, texturelol);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE,image->GetPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    free( image);
	glBindTexture(GL_TEXTURE_2D, 0);
	cout << glGetError() <<endl;
}
void initInterface()
{
	glEnable(GL_TEXTURE_2D);
	loadTexture("textures/grass_side.png");

}

void drawInterface()
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, texturelol);
	glBegin(GL_QUADS);
		glTexCoord2f(0.2, 0.85); glVertex3f(0.2, 0.85,0.0f);
		glTexCoord2f(0.2, 0.95); glVertex3f(0.2, 0.95,0.0f);
		glTexCoord2f(0.8, 0.95); glVertex3f(0.8, 0.95,0.0f);
		glTexCoord2f(0.8, 0.85); glVertex3f(0.8, 0.85,0.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}