#include "Global.h"

sf::Window Window;

void InitGraphics() {
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
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
void BindTexture(GLuint tex) {
	if(tex!=bound){
		glBindTexture(GL_TEXTURE_2D,tex);
	}
}

map<string, sf::Image> textures;
GLuint bound = 0;