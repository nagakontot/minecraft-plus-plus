#include "Global.h"

sf::Window Window;
GLuint TEX;
#define AddTex(x, y) glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, x, TEXTURE_SIZE, TEXTURE_SIZE, 1, GL_RGBA, GL_UNSIGNED_BYTE, GetPixels(y));

void InitGraphics() {
	glewInit();
	glClearColor(0.3,0.6,1,1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glGenTextures(1,&TEX);
	glBindTexture(GL_TEXTURE_3D,TEX);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, TEXTURE_SIZE, TEXTURE_SIZE, MAX_TEXTURES, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	AddTex(0,"textures/stone.png");
	AddTex(1,"textures/dirt.png");
	AddTex(3,"textures/grass.png");
	AddTex(2,"textures/grass side.png");
}

GLuint GetTexture(string name) {
	if(textures.find(name)==textures.end()){
		if(!textures[name].LoadFromFile(name)){
			cout << "Failed to load texture " << name << endl;
			return 0;
		}
		textures[name].Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	} else {
		textures[name].Bind();
	}
	GLint t[1];
	glGetIntegerv(GL_TEXTURE_BINDING_2D,t);
	return *t;
}
uint8_t* GetPixels(string name) {
	sf::Image* image = new sf::Image;
	image->LoadFromFile(name);
	return (uint8_t*)image->GetPixelsPtr();
};

void BindTexture(GLuint tex) {
	if(tex!=bound){
		glBindTexture(GL_TEXTURE_2D,tex);
		bound = tex;
	}
}

map<string, sf::Image> textures;
GLuint bound = 0;
